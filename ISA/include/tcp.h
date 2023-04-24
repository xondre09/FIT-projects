/*!
 * \file    tcp.h
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   TCP
 */

#ifndef _TCP_H_
#define _TCP_H_

#include "packet.h"

#define __FAVOR_BSD
#include <netinet/tcp.h>

// Priznaky TCP
#ifndef __FAVOR_BSD
#  define TH_FIN  0x01
#  define TH_SYN  0x02
#  define TH_RST  0x04
#  define TH_PUSH 0x08
#  define TH_ACK  0x10
#  define TH_URG  0x20
#endif

#define TH_ECE  0x40
#define TH_CWR  0x80

/*
 * trida reprezentujici TCP
 */
class TCP : public L4 {
	uint16_t sport;
	uint16_t dport;
    uint32_t seq;
    uint32_t ack;
    uint8_t flags;

public:
    TCP( uint16_t sport, uint16_t dport, uint32_t seq, uint32_t ack, uint8_t flags );

	std::string srcPort();
	std::string dstPort();
    virtual std::string toString();
};

#endif
