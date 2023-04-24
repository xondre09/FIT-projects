/*!
 * \file    parser.cc
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   Analyzator paketu
 */

#include "parser.h"

/*
 * Vektor der pro fragmentaci IPv4
 */
std::map<Parser::FragmentKey, std::vector<Parser::Fragment>> Parser::holes;
/*
 * Vektor fragmentu pro fragmentaci IPv4
 */
std::map<Parser::FragmentKey, std::vector<Parser::Fragment>> Parser::fragments;

/*
 * Konstruktor analyzatoru
 *
 * \param sortType Razeni paketu
 */
Parser::Parser( int sortType ) :
    sort( sortType )
{

}

/*
 * Destruktor analyzatoru
 */
Parser::~Parser()
{
    for( auto item: packets ) delete item;
}

/*
 * Zahajeni parsovani paketu
 */
int Parser::parse( const struct pcap_pkthdr pkthdr, const u_char* bytes )
{
    static unsigned idx = 1;
    Packet *packet = new Packet( idx, pkthdr.ts, pkthdr.caplen );
    // parsovani ethernetu
    int ret = parseEthernet( packet, bytes, pkthdr.caplen );

    if( ret == 0 ) {    // vse probehlo v poradku
        idx++;
        packets.push_back( packet );
    } else {    // zahozeni paketu
        delete packet;
    }

    return ret;
}

/*
 * Parsovani ethernetu
 *
 * \param packet Kam se ma ulozt vysledek zpracovani
 * \param bytes Paket pro zpracovani
 * \param length Delka paketu
 */
int Parser::parseEthernet( Packet *packet, const u_char* bytes, int length )
{
    unsigned int offset = 0;

    uint16_t ether_type;
    L2 *link;
    ether_header *eth = (struct ether_header *) bytes;
    struct vlan_tag *vlan1 = NULL;
    struct vlan_tag *vlan2 = NULL;

    switch( ntohs( eth->ether_type ) ) {
        case ETHERTYPE_VLAN:    // IEEE 802.1Q
            vlan1 = (struct vlan_tag *) (bytes + 2*ETH_ALEN);
            ether_type = ntohs( *(uint16_t*)(bytes + 2*ETH_ALEN + VLAN_TAG_LEN) );
           
            link = new Ethernet( eth->ether_shost, eth->ether_dhost, ntohs( vlan1->vlan_tci ) );
            offset += ETH_HLEN + VLAN_TAG_LEN;
            break;
        case ETH_P_8021AD:  // IEEE 802.1ad
            vlan1 = (struct vlan_tag *) (bytes + 2*ETH_ALEN);
            vlan2 = (struct vlan_tag *) (bytes + 2*ETH_ALEN + VLAN_TAG_LEN);
            ether_type = ntohs( *(uint16_t*)(bytes + 2*ETH_ALEN + 2*VLAN_TAG_LEN) );
            link = new Ethernet( eth->ether_shost, eth->ether_dhost, ntohs( vlan1->vlan_tci ), ntohs( vlan2->vlan_tci ) );
            offset += ETH_HLEN + 2*VLAN_TAG_LEN;
            break;
        default:    // standardni ethernet
            ether_type = ntohs( eth->ether_type );
            link = new Ethernet( eth->ether_shost, eth->ether_dhost );            
            offset += ETH_HLEN;
            break;
    }

    packet->setLink( link );
    // dalsi protokol
    switch( ether_type ) {
        case ETHERTYPE_IP:
            return parseIPv4( packet, bytes + offset, length - offset );
        case ETHERTYPE_IPV6:
            return parseIPv6( packet, bytes + offset, length - offset );
        default:
            std::cerr << "Unknown protocol: 0x" << std::hex << std::setw(4) << std::setfill('0') << ether_type << std::endl;
            return 1;
    }
}

/*
 * Parsovani IPv4
 *
 * \param packet Kam se ma ulozt vysledek zpracovani
 * \param bytes Paket pro zpracovani
 * \param length Delka paketu
 */
int Parser::parseIPv4( Packet *packet, const u_char* bytes, int length )
{
    struct ip *ip4 = (struct ip *)(bytes);
    unsigned int offset = ip4->ip_hl*4;

    u_char* next = NULL;
    int max = 0;
    packet->setNet( new IPv4( ip4->ip_src, ip4->ip_dst, ip4->ip_ttl ) );
    
    /* FRAGMENTACE */
    if( ntohs( ip4->ip_off ) & IP_MF || ntohs( ip4->ip_off ) & IP_OFFMASK ) {
        // Vytvoreni fragmentu
        Fragment fragment = Fragment();
        fragment.first = int( ntohs( ip4->ip_off ) & IP_OFFMASK )*8;
        fragment.last  = int( ntohs( ip4->ip_off ) & IP_OFFMASK )*8 + ( length - offset );
        next = new u_char[fragment.last - fragment.first];
        memcpy( next, bytes+offset, fragment.last - fragment.first );
        fragment.data = next;
        next = NULL;

        FragmentKey key = FragmentKey{ ip4->ip_src, ip4->ip_dst, ip4->ip_id, ip4->ip_p };
        
        // Novy datagram
        if( Parser::holes.find( key ) == Parser::holes.end() ) {
            Fragment fra = Fragment{ 0, 99999, NULL };
            Parser::holes[ key ] = std::vector<Fragment>{};
            Parser::fragments[ key ] = std::vector<Fragment>{};
            Parser::holes[ key ].push_back( fra );
        } 
        
        Parser::fragments[ key ].push_back( std::move( fragment ) );

        /*
         * Implementace algoritmu z RFC 815 kapitola 3. Fragment Processing Algorithm
         */

        // step #1 -- Vybrani dalsi diry. Pokud uz zadna dalsi neni -> goto step #8
        for( unsigned i = 0; i < Parser::holes[ key ].size(); i++) {        
            Fragment hole = Parser::holes[ key ][ i ];

            // step #2
            if( fragment.first > hole.last ) continue;

            // step #3
            if( fragment.last < hole.first ) continue;

            // step #4 -- smazani diry
            Parser::holes[key].erase( Parser::holes[key].begin() + i );
            i--;        // další iterace by jinak přeskočila další prvek

            // step #5
            if( fragment.first > hole.first ) {
                Parser::holes[key].push_back( Fragment{ hole.first, fragment.first - 1, NULL } );
            }

            // step #6
            if( fragment.last < hole.last && ntohs(ip4->ip_off) & IP_MF ) {
                Parser::holes[key].push_back( Fragment{ fragment.last + 1, hole.last, NULL } );
            }

            // step #7 -- goto step #1
        }

        // step #8 -- vsechny diry vyplneny
        if( Parser::holes[key].empty() ) {
            // Poskladani datove casti
            max = 0;
            for( auto& item: Parser::fragments[key]) if( max < item.last ) max = item.last;
            next = new u_char [max];
            
            for( auto& item: Parser::fragments[key] ) {
                for(int i = item.first; i < item.last; i++)
                    next[i] = item.data[i - item.first];
            }

            Parser::holes.erase(Parser::holes.find( key ) );
            Parser::fragments.erase(Parser::fragments.find( key ) );
        } else {
            return 4;
        }
    } 

    int ret = 0;
    switch( ip4->ip_p ) {
        case IPPROTO_TCP:
            ret = parseTCP( packet, next == NULL ? bytes + offset : next, next == NULL ? length - offset : max );
            break;
        case IPPROTO_UDP:
            ret = parseUDP( packet, next == NULL ? bytes + offset : next, next == NULL ? length - offset : max  );
            break;
        case IPPROTO_ICMP:
            ret = parseICMPv4( packet, (next == NULL ? bytes + offset : next), (next == NULL ? length - offset : max ) );
            break;
        default:
            std::cerr << "Unknown ip protocol: " << std::dec << unsigned( ip4->ip_p )<< std::endl;
            ret = 2;
            break;
    }
    if( next != NULL ) delete[] next;
    return ret;
}

/*
 * Parsovani IPv6
 *
 * \param packet Kam se ma ulozt vysledek zpracovani
 * \param bytes Paket pro zpracovani
 * \param length Delka paketu
 */
int Parser::parseIPv6( Packet *packet, const u_char* bytes, int length )
{
    struct ip6_hdr *ip6 = (struct ip6_hdr *)(bytes);
    struct ip6_ext *ip6ext = NULL;
    unsigned int offset = 40;

    uint8_t ip_type = ip6->ip6_nxt;
    
    // rozsirujici hlavicky
    while( IP6_EXT_HEADER( ip_type ) ) {
        ip6ext = (struct ip6_ext *)(bytes + offset);
        ip_type = ip6ext->ip6e_nxt;
        offset += (ip6ext->ip6e_len + 1)*8;
    }

    packet->setNet( new IPv6( ip6->ip6_src, ip6->ip6_dst, ip6->ip6_hops ) );

    switch( ip_type ) {
        case IPPROTO_TCP:
            return parseTCP( packet, bytes + offset, length - offset );
        case IPPROTO_UDP:
            return parseUDP( packet, bytes + offset, length - offset );
        case IPPROTO_ICMPV6:
            return parseICMPv6( packet, bytes + offset, length - offset );
        default:
            std::cerr << "Unknown ip protocol: " << std::dec << unsigned( ip_type )<< std::endl;
            return 2;
    }
}

/*
 * Parsovani ICMPv4
 *
 * \param packet Kam se ma ulozt vysledek zpracovani
 * \param bytes Paket pro zpracovani
 * \param length Delka paketu
 */
int Parser::parseICMPv4( Packet *packet, const u_char* bytes, int length )
{
    (void)length;
    struct icmphdr *icmp4 = (struct icmphdr *)(bytes);
    packet->setTrans( new ICMPv4( icmp4->type, icmp4->code ) );
    return 0;
}

/*
 * Parsovani ICMPv6
 *
 * \param packet Kam se ma ulozt vysledek zpracovani
 * \param bytes Paket pro zpracovani
 * \param length Delka paketu
 */
int Parser::parseICMPv6( Packet *packet, const u_char* bytes, int length )
{
    (void)length;
    struct icmp6_hdr *icmp6 = (struct icmp6_hdr *)(bytes);
    packet->setTrans( new ICMPv6( icmp6->icmp6_type, icmp6->icmp6_code ) );
    return 0;
}

/*
 * Parsovani TCP
 *
 * \param packet Kam se ma ulozt vysledek zpracovani
 * \param bytes Paket pro zpracovani
 * \param length Delka paketu
 */
int Parser::parseTCP( Packet *packet, const u_char* bytes, int length )
{
    (void)length;
    struct tcphdr *tcp = (struct tcphdr *)(bytes);
    packet->setTrans( new TCP( ntohs( tcp->th_sport ), ntohs( tcp->th_dport ), ntohl( tcp->th_seq ), ntohl( tcp->th_ack ), tcp->th_flags ) );
    return 0;
}

/*
 * Parsovani UDP
 *
 * \param packet Kam se ma ulozt vysledek zpracovani
 * \param bytes Paket pro zpracovani
 * \param length Delka paketu
 */
int Parser::parseUDP( Packet *packet, const u_char* bytes, int length )
{
    (void)length;
    struct udphdr *udp = (struct udphdr *)(bytes);
    packet->setTrans( new UDP( ntohs( udp->uh_sport ), ntohs( udp->uh_dport ) ) );
    return 0;
}

/*
 * Vraceni zpracovanych paketu
 *
 * \param packet Kam se ma ulozt vysledek zpracovani
 * \param bytes Paket pro zpracovani
 * \param length Delka paketu
 */
std::vector<Packet*> Parser::getPackets() 
{
    std::stable_sort( this->packets.begin(), this->packets.end(), this->sort );
    return this->packets;
}

/*
 * Vraceni vystupu agregace
 *
 * \param type Klic podle ktereho se bude agregovat
 */
std::vector<Parser::Group*> Parser::groupBy( int type )
{
    std::vector<Parser::Group*> aggr;
    std::string key("");
    for( auto packet : packets ) {

        switch( type ) {
            case SRC_MAC:  key = packet->srcMac();  break;
            case DST_MAC:  key = packet->dstMac();  break;
            case SRC_IP:   key = packet->srcIp();   break;
            case DST_IP:   key = packet->dstIp();   break;
            case SRC_PORT: key = packet->srcPort(); break;
            case DST_PORT: key = packet->dstPort(); break;
            default:
                key = "";
                break;
        }
        // zahoceni ICMP u zadi agregace podle portu
        if( key.compare( "" ) != 0) {
            bool flag = true;
            // prirazeni do skupiny
            for( auto item : aggr ) {  
                if( item->name.compare( key ) == 0 ) {
                    item->length += packet->getLength();
                    item->count  += 1;
                    flag = false;
                    break;
                }
            }
            // Nova skupina
            if( flag ) {
                Group *g = new Group();
                g->name = key;
                g->length = packet->getLength();
                g->count = 1;
                aggr.push_back( g );
            }
        }
    }
    // serazeni
    std::stable_sort( aggr.begin(), aggr.end(), this->sort );
    return aggr;
}

/*
 * Pravidlo pro razeni paketu
 */
bool Parser::Sort::operator() ( Packet *packet1, Packet *packet2 ) 
{ 
    switch( type ) {
        case NO_SORT:
        case PACKET_SORT:
            return false;
        case LEN_SORT:
            return ( packet1->getLength() > packet2->getLength() );
        default:
            return false;
    }
}

/*
 * Pravidlo pro razeni agregovanych skupin
 */
bool Parser::Sort::operator() ( Parser::Group *group1, Parser::Group *group2 ) 
{ 
    switch( type ) {
        case NO_SORT:
            return false;
        case PACKET_SORT:
            return group1->count > group2->count;
        case LEN_SORT:
            return group1->length > group2->length;
        default:
            return false;
    }
}

/*
 * Reprezentace agregovane skupiny jako retezec
 */
std::string Parser::Group::toString()
{
    std::stringstream out("");
    out << name << ": " << count << " " << length;
    return out.str(); 
}

/*
 * Potrebne pro pouziti jako klic v std::map
 */
bool Parser::FragmentKey::operator==(const FragmentKey &o) const 
{
    return identification == o.identification && src.s_addr == o.src.s_addr && dst.s_addr == o.dst.s_addr && protokol == o.protokol;
}

bool Parser::FragmentKey::operator<(const FragmentKey &o) const 
{
    return ( identification < o.identification ) || 
        ( identification == o.identification && src.s_addr < o.src.s_addr ) || 
        ( identification == o.identification && src.s_addr == o.src.s_addr && dst.s_addr < o.dst.s_addr ) ||
        ( identification == o.identification && src.s_addr == o.src.s_addr && dst.s_addr == o.dst.s_addr && protokol < o.protokol );
}

/******************
 * Fragment u IPv4
 ******************/
Parser::Fragment::Fragment() : 
    first(0), 
    last(0), 
    data(NULL) 
{

}

Parser::Fragment::Fragment( int first, int last, u_char* data ) : 
    first(first), 
    last(last), 
    data(data) 
{

}

Parser::Fragment::Fragment( const Fragment &f ) : 
    first( f.first ), 
    last( f.last ), 
    data( NULL ) 
{
    if( f.data != NULL ) {
        u_char *str = new u_char[f.last - f.first];
        memcpy( str, f.data, f.last - f.first );
        data = str;
    }
}

Parser::Fragment::~Fragment() 
{ 
    if( data != NULL ) {
        delete[] data;
        data = NULL;
    } 
}
