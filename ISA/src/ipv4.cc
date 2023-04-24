/*!
 * \file    ipv4.cc
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   IPv4
 */

#include "ipv4.h"

/**
 * \brief Konstruktor třídy reprezentující IPv4 protokol
 *
 * \param srcIp Zdrojová IP adresa
 * \param dstIp Cílová IP adresa
 * \param TTL   Time to Live
 */
IPv4::IPv4( in_addr srcIp, in_addr dstIp, uint8_t TTL ) : 
    src(srcIp), 
    dst(dstIp), 
    TTL(TTL) 
{

}

/**
 * \brief Vrátí zdrojovou IP adresu jako řetězec
 */
std::string IPv4::srcIp()
{
    return std::string( inet_ntoa( this->src ) );
}

/**
 * \brief Vrátí cílovo IP adresu jako řetězec
 */
std::string IPv4::dstIp()
{
    return std::string( inet_ntoa( this->dst ) );   
}

/**
 * \brief Reprezentace informací o IPv4 jako řetězec
 */
std::string IPv4::toString() 
{
    std::stringstream out("");
    out << "IPv4: " << this->srcIp() << " " << this->dstIp() << " " << std::dec << unsigned( this->TTL );
    return out.str(); 
}