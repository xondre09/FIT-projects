/*!
 * \file    ethernet.cc
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   Ethernet
 */

#include "ethernet.h"

/*
 * Konstruktor tridy Ethernet
 *
 * \brief srcMac Zdrojova MAC adresa
 * \brief dstMac Cilova MAC adresa
 */
Ethernet::Ethernet(uint8_t srcMac[6], uint8_t dstMac[6]) : 
    Ethernet( srcMac, dstMac, 0, 0, 0 ) 
{

}

/*
 * Konstruktor tridy Ethernet
 *
 * \brief srcMac Zdrojova MAC adresa
 * \brief dstMac Cilova MAC adresa
 * \brief VID1   VID u IEEE 801.2Q
 */
Ethernet::Ethernet(uint8_t srcMac[6], uint8_t dstMac[6], uint16_t VID1) : 
    Ethernet( srcMac, dstMac, VID1, 0, 1 ) 
{

}

/*
 * Konstruktor tridy Ethernet
 *
 * \brief srcMac Zdrojova MAC adresa
 * \brief dstMac Cilova MAC adresa
 * \brief VID1   VID1 u IEEE 801.2ad
 * \brief VID2   VID2 u IEEE 801.2ad
 */ 
Ethernet::Ethernet(uint8_t srcMac[6], uint8_t dstMac[6], uint16_t VID1, uint16_t VID2) : 
    Ethernet( srcMac, dstMac, VID1, VID2, 2 ) 
{

}

/*
 * Konstruktor tridy Ethernet
 *
 * \brief srcMac Zdrojova MAC adresa
 * \brief dstMac Cilova MAC adresa
 * \brief VID1   VID1 u IEEE 801.2Q
 * \brief VID2   VID2 u IEEE 801.2ad
 * \brief VID_type zda se jedna o standard, IEEE 801.2Q nebo IEEE 801.2ad
 */ 
Ethernet::Ethernet(uint8_t srcMac[6], uint8_t dstMac[6], uint16_t VID1, uint16_t VID2, short VID_type) : 
    VID1( VID1 ), 
    VID2( VID2 ), 
    VID_type( VID_type ) 
{
    for( unsigned int i = 0; i < 6; i++ ) {
        this->src[i] = srcMac[i];
        this->dst[i] = dstMac[i];
    }
}

std::string Ethernet::srcMac() 
{
    std::ostringstream out("");
    for( unsigned int i = 0; i<5 ; i++ ) {
        out << std::setfill('0') << std::setw(2) << std::hex << unsigned(src[i]) << ":";
    }
    out << std::setfill('0') << std::setw(2) << std::hex << unsigned(src[5]);
    return out.str(); 
}

std::string Ethernet::dstMac() 
{ 
    std::stringstream out("");
    for( unsigned int i = 0; i<5; i++ ) {
        out << std::setfill('0') << std::setw(2) << std::hex << unsigned(dst[i]) << ":";
    }
    out << std::setfill('0') << std::setw(2) << std::hex << unsigned(dst[5]);
    return out.str(); 
}

std::string Ethernet::toString() 
{ 
    std::ostringstream out( "" );
    out << "Ethernet: " << this->srcMac() << " " << this->dstMac();
    if( this->VID_type != 0 ) {
        out << " " << unsigned( this->VID1 & VLAN_VID_MASK );
        if (this->VID_type == 2) {
            out << " " << unsigned( this->VID2  & VLAN_VID_MASK );
        }
    }
    return out.str();
}