/*!
 * \file    icmpv4.h
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   ICMPv4
 */

#ifndef _ICMPV4_H_
#define _ICMPV4_H_

#include <netinet/ip_icmp.h>

#include "packet.h"

#ifndef ICMP_PARAMPROB_ERRATPTR
#define ICMP_PARAMPROB_ERRATPTR 0
#endif

/*
 * Trida reprezentujici ICMPv4
 */
class ICMPv4 : public L4 {
    uint8_t type;
    uint8_t code;
public:
    ICMPv4( uint8_t type, uint8_t code );

    virtual std::string srcPort() {return "";};
    virtual std::string dstPort() {return "";};
    virtual std::string toString();
};

#endif