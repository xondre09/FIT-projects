/*!
 * \file    packet.h
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   Reprezentace jednoho paketu.
 */

#ifndef _PACKET_H_
#define _PACKET_H_

#include "level.h"
#include "iostream"

/**
 * Třída reprezentující paket.
 */
class Packet : public L2, L3, L4 {
	unsigned number;
	uint32_t len;
	struct timeval ts;

	L2 *link;	///< L2 vrstva
	L3 *net;	///< L3 vrstva
	L4 *trans;  ///< L4 vrstva

public:
	Packet( unsigned number, struct timeval ts, uint32_t len );
	Packet( unsigned number, struct timeval ts, uint32_t len, L2* link, L3* net, L4* trans );
	~Packet();
	
	virtual std::string srcMac();
	virtual std::string dstMac();
	virtual std::string srcIp();
	virtual std::string dstIp();
	virtual std::string srcPort();
	virtual std::string dstPort();
	virtual std::string toString();

	void setLength(uint32_t len) {this->len=len;}
	uint32_t getLength() {return len;}

	void setLink( L2 *link );
	void setNet( L3 *net );
	void setTrans( L4 *trans );

	L2 *getLink();
	L3 *getNet();
	L4 *getTrans();
};

#endif