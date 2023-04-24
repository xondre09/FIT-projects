/*!
 * \file    tcp.cc
 * \authors Karel ONDŘEJ, FIT
 * \date    2. 10. 2017
 *
 * \brief   trida uchovavajici informace o TCP protokolu
 */

#include "tcp.h"

/*
 * Konstruktor tridy reprezentujici TCP protokol
 */
TCP::TCP( uint16_t sport, uint16_t dport, uint32_t seq, uint32_t ack, uint8_t flags ) : 
    sport( sport ),
    dport( dport ), 
    seq( seq ), 
    ack( ack ), 
    flags( flags ) 
{

}

/*
 * vraci zdrojovi port jako retezec
 */
std::string TCP::srcPort()
{
    std::stringstream out("");
    out << sport;
    return out.str(); 
}

/*
 * vraci cilovy port jako retezec
 */
std::string TCP::dstPort()
{
    std::stringstream out("");
    out << dport;
    return out.str(); 
}

/*
 * Reprezentace TCP jako retezce
 */
std::string TCP::toString() 
{
    std::stringstream out("");
    out << "TCP: " << this->srcPort() << " " << this->dstPort() << " " << seq << " " << ack;
    out << " ";
    out << ( flags & TH_CWR  ? "C" : "." );
    out << ( flags & TH_ECE  ? "E" : "." );
    out << ( flags & TH_URG  ? "U" : "." ); 
    out << ( flags & TH_ACK  ? "A" : "." );
    out << ( flags & TH_PUSH ? "P" : "." );
    out << ( flags & TH_RST  ? "R" : "." );
    out << ( flags & TH_SYN  ? "S" : "." );
    out << ( flags & TH_FIN  ? "F" : "." );

    return out.str(); 
}