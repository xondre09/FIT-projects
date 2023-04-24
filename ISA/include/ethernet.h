/*!
 * \file    ethernet.h
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   Ethernet
 */

#ifndef _ETHERHET_H_
#define _ETHERHET_H_

#include "packet.h"
#include "ipv4.h"
#include "ipv6.h"

#include <pcap/vlan.h>
#include <net/ethernet.h>

#ifndef ETH_P_8021AD
#  define ETH_P_8021AD (0x88a8)
#endif

#define VLAN_VID_MASK (0x0FFF)

/*
 * Trida reprezentujici Ethernet
 */
class Ethernet : public L2 {
    uint8_t src[6];
    uint8_t dst[6];
    uint16_t VID1;
    uint16_t VID2;
    short VID_type;

public:
    Ethernet(uint8_t srcMac[6], uint8_t dstMac[6]);
    Ethernet(uint8_t srcMac[6], uint8_t dstMac[6], uint16_t VID1);
    Ethernet(uint8_t srcMac[6], uint8_t dstMac[6], uint16_t VID1, uint16_t VID2);

    virtual ~Ethernet() {}
    virtual std::string srcMac();
    virtual std::string dstMac();
    virtual std::string toString();

protected:
    Ethernet(uint8_t srcMac[6], uint8_t dstMac[6], uint16_t VID1, uint16_t VID2, short VID_type);
};

#endif