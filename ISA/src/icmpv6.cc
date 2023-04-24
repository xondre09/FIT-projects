/*!
 * \file    icmpv6.cc
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   ICMPv6
 */

#include "icmpv6.h"

/*
 * Konstruktor ICMPv6
 */
ICMPv6::ICMPv6( uint8_t type, uint8_t code ) : 
    type( type ), 
    code( code ) 
{

}

/*
 * Reprezentace ICMPv6 jako retezce
 */
std::string ICMPv6::toString() 
{
    std::stringstream out("");

    out << "ICMPv6: " << std::dec << unsigned( type ) << " " << std::dec << unsigned( code );
    
    switch( type ) {
        case ICMP6_DST_UNREACH:
            out << " destination unreachable";
            switch( code ) {
                case ICMP6_DST_UNREACH_NOROUTE :
                    out << " no route to destination";
                    break;
                case ICMP6_DST_UNREACH_ADMIN:
                    out << " communication with destination administratively prohibited";
                    break;
                case ICMP6_DST_UNREACH_BEYONDSCOPE:
                    out << " beyond scope of source address";
                    break;
                case ICMP6_DST_UNREACH_ADDR:
                    out << " address unreachable";
                    break;
                case ICMP6_DST_UNREACH_NOPORT:
                    out << " port unreachable";
                    break;
                case 5:
                    out << " source address failed ingress/egress policy";
                    break;
                case 6:
                    out << " reject route to destination";
                    break;
                default:
                    break;
            }
            break;
        case ICMP6_PACKET_TOO_BIG:
            out << " packet too big";
            break;
        case ICMP6_TIME_EXCEEDED:
            out << " time exceeded";
            switch( code ) {
                case ICMP6_TIME_EXCEED_TRANSIT:
                    out << " hop limit exceeded in transit";
                    break;
                case ICMP6_TIME_EXCEED_REASSEMBLY:
                    out << " fragment reassembly time exceeded";
                    break;
                default:
                    break;
            }
            break;
        case ICMP6_PARAM_PROB:
            out << " parameter problem";
            switch( code ) {
                case ICMP6_PARAMPROB_HEADER:
                    out << " erroneous header field encountered";
                    break;
                case ICMP6_PARAMPROB_NEXTHEADER :
                    out << " unrecognized next header type encountered";
                    break;
                case ICMP6_PARAMPROB_OPTION:
                    out << " unrecognized IPv6 option encountered";
                    break;
            }
            break;
        case ICMP6_ECHO_REQUEST:
            out << " echo request";
            break;
        case ICMP6_ECHO_REPLY:
            out << " echo reply";
            break;
        case 100:
        case 101:
        case 200:
        case 201:
            out << " private experimentation";
            break;
        case 127:
            out << " reserved for expansion of ICMPv6 error messages";
            break;
        case 255:
            out << " reserved for expansion of ICMPv6 informational messages";
            break;
        default:
            break;
    }

    return out.str(); 
}