/* 
 * Name:    ipk-scanner.c
 * Author:  Karel Ondřej
 * Email:   xondre09@stud.fit.vutbr.cz
 * Login:   xondre09
 * Date:    23. 04. 2017
 * 
 * Description: ARP scanner.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

#include <arpa/inet.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>     // close()
#include <time.h>

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>

#include "xml.hh"
#include <iostream>
#include <fstream>

// prevod adresy v poli charu na cislo
#define addr2long(i) (((i)[0] << 24) + ((i)[1] << 16) + ((i)[2] << 8) + (i)[3])

// struktury ARP packetu
typedef struct ARP_packet {
    uint16_t htype;
    uint16_t ptype;
    uint8_t  hlen;
    uint8_t  plen;
    uint16_t oper;
    uint8_t  sha[6];
    uint8_t  spa[4];
    uint8_t  tha[6];
    uint8_t  tpa[4];
}ARP_packet_t;

// ziskani MAC adresy na danem rozhrani
int get_mac_addres(uint8_t mac[6], char* port) {
    int soc;
    struct ifreq ifr;
    soc = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strcpy(ifr.ifr_name, port);

    if(ioctl(soc, SIOCGIFHWADDR, &ifr) == -1) {
        close(soc);
        return -1;
    }

    for (int i = 0; i < 6; i++) {
        mac[i] = ifr.ifr_hwaddr.sa_data[i];
    }

    close(soc);
    return 0;
}
// ziskani IP adresy na danem rozhrani
int get_IP_addres(uint8_t ip[4], char* port) {
    int soc;
    struct ifreq ifr;
    soc = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strcpy(ifr.ifr_name, port);

    if(ioctl(soc, SIOCGIFADDR, &ifr) == -1) {
        close(soc);
        return -1;
    }

    for (int i = 0; i < 4; i++) {
        ip[i] = (uint8_t)ifr.ifr_addr.sa_data[i+2];
    }
    close(soc);
    return 0;
}
// ziskani Masky site na danem rozhrani
int get_mask(uint8_t netMask[4], char* port) {
    int soc;
    struct ifreq ifr;
    soc = socket(AF_INET, SOCK_DGRAM, 0);

    strcpy(ifr.ifr_name, port);

    if(ioctl(soc, SIOCGIFNETMASK, &ifr) == -1) {
        close(soc);
        return -1;
    }

    for (int i = 0; i < 4; i++) {
        netMask[i] = ifr.ifr_netmask.sa_data[i+2];
    }
    close(soc);
    return 0;
}
// ziskani adresy site na danem rozhrani
void get_site(uint8_t site[4], uint8_t broadcast[4], uint8_t addr[4], uint8_t mask[4]) {
    for (int i = 0; i < 4; i++) {
        site[i] = addr[i] & mask[i];
        broadcast[i] = (addr[i] & mask[i]) | ~mask[i];
    }
}
// nastavi broadcast
void set_broadcast(uint8_t mac[6]) {
    mac[0] = 0xff;
    mac[1] = 0xff;
    mac[2] = 0xff;
    mac[3] = 0xff;
    mac[4] = 0xff;
    mac[5] = 0xff;
}

// inicializace ARP packetu a Ethernet frame
ARP_packet_t* init_ARP_request(unsigned char* et_ptr, char* port) {

    get_mac_addres(((uint8_t*)et_ptr)+6, port);
    set_broadcast((uint8_t*)et_ptr);
    // ARP
    et_ptr[12] = 0x08;
    et_ptr[13] = 0x06;

    ARP_packet_t* arp_ptr = (ARP_packet_t*)(et_ptr + 14);
    // Big indian
    arp_ptr->htype = 0x0100;    // Ethernet 1
    arp_ptr->ptype = 0x0008;    // IPv4 0800
    arp_ptr->hlen  = 0x06;      // delka MAC
    arp_ptr->plen  = 0x04;      // delka IPv4
    arp_ptr->oper  = 0x0100;    // zadost 1

    if(get_mac_addres(arp_ptr->sha, port) == -1 ||  get_IP_addres(arp_ptr->spa,  port) == -1 ){

        return NULL;
    }

    return arp_ptr;
}

int main(int argc, char *argv[])
{
    char* interface = NULL;
    std::string file("");
    int ch;
    // parosvani parametru
    while ((ch = getopt(argc, argv, "i:f:")) != -1) {
        switch (ch) {
        case 'i':
            interface = optarg;
            break;
        case 'f':
            file = std::string(optarg);
            break;
        default:
            return EXIT_FAILURE;
            break;
        }
    }
    argc -= optind;
    argv += optind; 

    if(file == "") {
        std::cerr << "Miss -f <file>" << std::endl;
        return EXIT_FAILURE;
    }

    if(interface == NULL) {
        std::cerr << "Miss -i <inteface>" << std::endl;
        return EXIT_FAILURE;
    }

    unsigned char buffer_request[42];
    unsigned char buffer_response[1024];

    ARP_packet_t* arp_request  = init_ARP_request(buffer_request, interface);
    ARP_packet_t* arp_response = (ARP_packet_t*)(buffer_response + 14);

    if(arp_request == NULL) {
        perror ("Init ARP Request failed.");
        exit (EXIT_FAILURE);
    }

    uint8_t mask[4];
    uint8_t site[4];
    uint8_t broadcast[4];

    if( get_mask(mask, interface) == -1 ) {
        perror ("Get mask failed.");
        exit (EXIT_FAILURE);     
    }
    
    get_site(site, broadcast, arp_request->spa, mask);

    // XML
    Devices XML;

    struct ifreq ifr;
    struct sockaddr_ll socket_address;
    // RAW socket pro prijem
    int s;

    if( (s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)) ) < 0) {
        perror ("socket() failed ");
        exit (EXIT_FAILURE);
    }
    
    strncpy(ifr.ifr_name, interface, IFNAMSIZ);

    if (ioctl(s, SIOCGIFINDEX, &ifr) == -1) {
        perror("ireq SIOCGIFINDEX");
        exit(1);
    }

    // nastaveni socketu pro odeslani
    socket_address.sll_family = PF_PACKET;
    socket_address.sll_protocol = htons(ETH_P_IP);
    socket_address.sll_ifindex = ifr.ifr_ifindex;
    socket_address.sll_hatype = ARPHRD_ETHER;
    socket_address.sll_pkttype = PACKET_OTHERHOST;
    socket_address.sll_halen = 0;
    socket_address.sll_addr[6] = 0x00;
    socket_address.sll_addr[7] = 0x00;

    long long int start = addr2long(site) + 1;  // prvni adresa
    long long int end = addr2long(broadcast);   // broadcast

    for(long long int addr = start; addr < end;) {

        for(int i = addr; (addr - i) < 20 && addr < end; ++addr) {
            // prevod z cisla na pole
            arp_request->tpa[0] = (addr >> 24) & 0xff;
            arp_request->tpa[1] = (addr >> 16) & 0xff;
            arp_request->tpa[2] = (addr >> 8)  & 0xff;
            arp_request->tpa[3] = (addr)       & 0xff;
            // odeslani ARP zadosti
            int n = sendto(s, buffer_request, 60, 0, (struct sockaddr*)&socket_address, sizeof(socket_address));
            if( n < 0 ) {
                perror("sendto ");
                break; 
            }
        }

        int read = -1;

        time_t start_t, end_t;

        time(&start_t);
        time(&end_t);        
        // vycisteni bufferu
        memset(buffer_response, 0, 1024);
        
        while(difftime(end_t, start_t) < 2) {
            memset(buffer_response, 0, 100);
            read = recvfrom(s, buffer_response, 1024, MSG_DONTWAIT, NULL, NULL);

            if(read < 0) {
                if(errno != EAGAIN && errno != EWOULDBLOCK) {
                    // chyba
                    perror("recvfrom");
                    break;
                }
            } else if((buffer_response[12] == 0x08 && buffer_response[13] == 0x06 && (arp_response->oper == 512))) { 
                // ARP odpoved
                time(&start_t);
                XML.addIPv4(arp_response->sha, arp_response->spa);
            }
            
            time(&end_t);
        }

    }

    close(s);
    // tisk
    std::ofstream myfile;
    myfile.open (file);
    myfile << XML.str();
    myfile.close();

    return 0;
}