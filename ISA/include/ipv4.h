/*!
 * \file    ipv4.h
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   IPv4
 */

#ifndef _IPV4_H_
#define _IPV4_H_

#include "packet.h"
#include "tcp.h"
#include "udp.h"
#include "icmpv4.h"

#include <stdio.h>
#include <string.h>

#include <vector>
#include <map>

#include <netinet/ip.h>

/*
 * Trida reprezentujici IPv4
 */
class IPv4 : public L3 {
protected:
    in_addr src;
    in_addr dst;
    uint8_t TTL;

public:
    IPv4( in_addr srcIp, in_addr dstIp, uint8_t TTL );

    virtual std::string srcIp();
    virtual std::string dstIp();
    virtual std::string toString();
};
#endif