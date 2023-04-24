/*!
 * \file    ipv6.h
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   IPv6
 */

#ifndef _IPV6_H_
#define _IPV6_H_

#include "packet.h"
#include "udp.h"
#include "tcp.h"
#include "icmpv6.h"

#include <netinet/ip6.h>

/*
 * Kontrola zda se jedna o rozsirujici hlavicku
 */
#define IP6_EXT_HEADER( type ) ( (type) == IPPROTO_HOPOPTS  || (type) == IPPROTO_ROUTING || \
                                 (type) == IPPROTO_FRAGMENT || /* (type) == IPPROTO_ESP ||  \
                                 (type) == IPPROTO_AH       || */ (type) == IPPROTO_DSTOPTS || \
                                 (type) == IPPROTO_MH )

/*
 * trida reprezentujici IPv6
 */
class IPv6 : public L3 {
protected:
    struct in6_addr src;
    struct in6_addr dst;
    uint8_t hops;
public:
    IPv6( struct in6_addr srcIp, struct in6_addr dstIp, uint8_t hops );

    virtual std::string srcIp();
    virtual std::string dstIp();
    virtual std::string toString();
};

#endif