/*!
 * \file    udp.h
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   UDP
 */

#ifndef _UDP_H_
#define _UDP_H_

#include "packet.h"

#define __FAVOR_BSD
#include <netinet/udp.h>

/*
 * Trida reprezentujici UDP
 */
class UDP : public L4 {
protected:    
    uint16_t sport;
    uint16_t dport;
public:
    UDP( uint16_t sport, uint16_t dport );

    virtual std::string srcPort();
    virtual std::string dstPort();
    virtual std::string toString();
};

#endif
