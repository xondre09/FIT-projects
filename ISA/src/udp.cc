/*!
 * \file    udp.cc
 * \authors Karel ONDŘEJ, FIT
 * \date    2. 10. 2017
 *
 * \brief   Trida uchovavajici informace o UDP protokolu
 */

#include "udp.h"

/*
 * Konstruktor tridi reprezentuji UDP
 */
UDP::UDP( uint16_t sport, uint16_t dport ) : 
    sport( sport ), 
    dport( dport ) 
{

}

/*
 * Vraci zdrojovy port jako retezec
 */
std::string UDP::srcPort()
{
    std::stringstream out("");
    out << sport;
    return out.str(); 
}

/*
 * Vraci cilovy port jako retezec
 */
std::string UDP::dstPort()
{
    std::stringstream out("");
    out << dport;
    return out.str(); 
}

/*
 * reprezentace UDP protokolu jako string
 */
std::string UDP::toString() 
{
    std::stringstream out("");
    out << "UDP: " << this->srcPort() << " " << this->dstPort();
    return out.str(); 
}