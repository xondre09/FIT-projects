/*!
 * \file    ipv6.cc
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   IPv6
 */

#include "ipv6.h"

/*
 * Konstruktor tridy reprezentujici IPv6
 */
IPv6::IPv6( struct in6_addr srcIp, struct in6_addr dstIp, uint8_t hops ) :
    src( srcIp ), 
    dst( dstIp ), 
    hops( hops ) 
{

}

/*
 * Vraci zdrojovou IP adresu jako retezec
 */
std::string IPv6::srcIp()
{
    char buffer[INET6_ADDRSTRLEN];  
    inet_ntop( AF_INET6, &this->src, buffer, INET6_ADDRSTRLEN );
    return std::string( buffer);
}

/*
 * Vraci cilovou IP adresu jako retezec
 */
std::string IPv6::dstIp()
{
    char buffer[INET6_ADDRSTRLEN];  
    inet_ntop( AF_INET6, &this->dst, buffer, INET6_ADDRSTRLEN );
    return std::string( buffer );
}

/*
 * Reprezentace IPv6 jako retezce
 */
std::string IPv6::toString() 
{
    std::stringstream out("");
    out << "IPv6: " << this->srcIp() << " " << this->dstIp() << " " << std::dec << unsigned( this->hops );
    return out.str(); 
}