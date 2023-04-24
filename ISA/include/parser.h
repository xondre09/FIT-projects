/*!
 * \file    parser.h
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   Samotna anlyza paketu
 */

#ifndef _PARSER_H_
#define _PARSER_H_

#include <algorithm>
#include <iostream>

#include "ethernet.h"

#include <pcap.h>

/*
 * Typ podle ktereho se ma agregovat
 */
#define SRC_MAC  1
#define DST_MAC  2
#define SRC_IP   3
#define DST_IP   4
#define SRC_PORT 5
#define DST_PORT 6

/*
 * Klic pro razeni
 */
#define NO_SORT     0
#define PACKET_SORT 1
#define LEN_SORT    2

/*
 * Trida pro analyzu paketu
 */
class Parser
{
public:
    /*
     * Skupina pro agregaci
     */
    struct Group
    {
        std::string name;  // polozka podle ktere se agreguje
        int length;        // celkova delka paketu
        int count;         // pocet paketu

        /*
         * Reprezentace agregovane skupiny jako retezce
         */
        std::string toString();
    };

    Parser( int sortType );
    ~Parser();

    int parse( const struct pcap_pkthdr pkthdr, const u_char* packet );

    /*
     * Vraci vsechny pakety
     */
    std::vector<Packet*> getPackets();

    /*
     * Vraci agregovane pakety do skupin
     */
    std::vector<Parser::Group*> groupBy( int type );
protected:
    /****************************************
     * Zpracovávání jednotlivých protokolů
     ****************************************/
    int parseEthernet( Packet *packet, const u_char* bytes, int length );
    int parseIPv6( Packet *packet, const u_char* bytes, int length );
    int parseIPv4( Packet *packet, const u_char* bytes, int length );
    int parseICMPv4( Packet *packet, const u_char* bytes, int length );
    int parseICMPv6( Packet *packet, const u_char* bytes, int length );
    int parseTCP( Packet *packet, const u_char* bytes, int length );
    int parseUDP( Packet *packet, const u_char* bytes, int length );

private:
    /*
     * Třída pro řazení agregovaných skupin nebo paketů
     */
    struct Sort {
        int type = 0;

        Sort( int type ) : type( type ) {}

        bool operator() ( Packet *packet1, Packet *packet2 );
        bool operator() ( Parser::Group *group1, Parser::Group *group2 );
    };

    std::vector<Packet*> packets;
    Sort sort;

    /*
     * Klíč identifikující který fragment patří ke kterému datagramu
     */
    struct FragmentKey
    {
        in_addr src;
        in_addr dst;
        uint32_t identification;
        uint8_t protokol;

        bool operator==(const FragmentKey &o) const;
        bool operator<(const FragmentKey &o) const;
    };

    /*
     * Fragment u IPv4
     */
    struct Fragment
    {
        int first;
        int last;
        u_char* data;

        Fragment();
        Fragment( int first, int last, u_char* data );
        Fragment( const Fragment &f );
        ~Fragment();
    };

    /*
     * Vektor všech děr
     */
    static std::map<FragmentKey, std::vector<Fragment>> holes;
    
    /*
     * Vektor fragmentů
     */
    static std::map<FragmentKey, std::vector<Fragment>> fragments;
};

#endif