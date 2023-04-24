/*!
 * \file    icmpv6.h
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   ICMPv6
 */

#ifndef _ICMPV6_H_
#define _ICMPV6_H_

#include <netinet/icmp6.h>

#include "packet.h"

/*
 * Trida reprezentujici ICMPv6
 */
class ICMPv6 : public L4 {
    uint8_t type;	// typ zpravy
    uint8_t code;   // kod zpravy
public:
    ICMPv6(  uint8_t type, uint8_t code );

    virtual std::string srcPort() {return "";};
    virtual std::string dstPort() {return "";};
    virtual std::string toString();
};

#endif