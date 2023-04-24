/*!
 * \file    icmpv4.cc
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   ICMPv4
 */

#include "icmpv4.h"

/*
 * Konstruktor ICMPv4
 */
ICMPv4::ICMPv4( uint8_t type, uint8_t code ) : 
    type( type ), 
    code( code ) 
{

}

/*
 * Reprezentace ICMPv4 jako retezce
 */
std::string ICMPv4::toString() 
{
    std::stringstream out("");
   
    out << "ICMPv4: " << std::dec << unsigned( type ) << " " << std::dec << unsigned( code );

    // textovy popis typu zpravy a kodu zpravy
    switch( type ) {
        case ICMP_ECHOREPLY:
            out << " echo reply";
            break;
        case ICMP_UNREACH:
            out << " destination unreachable";
            switch( code ) {
                case ICMP_UNREACH_NET:
                    out << " net unreachable";
                    break;
                case ICMP_UNREACH_HOST:
                    out << " host unreachable";
                    break;
                case ICMP_UNREACH_PROTOCOL:
                    out << " protocol unreachable";
                    break;
                case ICMP_UNREACH_PORT:
                    out << " port unreachable";
                    break;
                case ICMP_UNREACH_NEEDFRAG:
                    out << " fragmentation needed and DF set";
                    break;
                case ICMP_UNREACH_SRCFAIL:
                    out << " source route failed";
                    break;
                default:
                    break;
            }
            break;
        case ICMP_SOURCEQUENCH:
            out << " source quench";
            break;
        case ICMP_REDIRECT:
            out << " redirect";
            switch( code ) {
                case ICMP_REDIRECT_NET:
                    out << " redirect datagrams for the network";
                    break;
                case ICMP_REDIRECT_HOST:
                    out << " redirect datagrams for the host";
                    break;
                case ICMP_REDIRECT_TOSNET:
                    out << " redirect datagrams for the type of service and network";
                    break;
                case ICMP_REDIRECT_TOSHOST:
                    out << " redirect datagrams for the type of service and host";
                    break;
                default:
                    break;
            }
            break;
        case ICMP_ECHO:
            out << " echo";
            break;
        case ICMP_TIMXCEED:
            out << " time exceeded";
            switch( code ) {
                case ICMP_TIMXCEED_INTRANS:
                    out << " time to live exceeded in transit";
                    break;
                case ICMP_TIMXCEED_REASS:
                    out << " fragment reassembly time exceeded";
                    break;
                default:
                    break;
            }
            break;
        case ICMP_PARAMPROB:
            out << " parameter problem";
            if( code == ICMP_PARAMPROB_ERRATPTR ) out << " pointer indicates the error";
            break;
        case ICMP_TSTAMP:
            out << " timestamp";
            break;
        case ICMP_TSTAMPREPLY:
            out << " timestamp reply";
            break;
        case ICMP_IREQ:
            out << " information request";
            break;
        case ICMP_IREQREPLY:
            out << " information reply";
            break;
    }
    return out.str(); 
}