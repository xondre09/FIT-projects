/*!
 * \file    packet.cc
 * \authors Karel ONDŘEJ, FIT
 * \date    2. 10. 2017
 *
 * \brief   Paket
 */

#include "packet.h"

Packet::Packet( unsigned number, struct timeval ts, uint32_t len) :
	Packet( number, ts, len, NULL, NULL, NULL )
{

}

Packet::Packet( unsigned number, struct timeval ts, uint32_t len, L2 *link, L3 *net, L4 *trans ) :
	number( number ),
	len( len ),
	ts( ts ),
	link( link ),
	net( net ),
	trans( trans )
{

}

Packet::~Packet()
{
	delete link;
	delete net;
	delete trans;
}

void Packet::setLink( L2 *link ) 
{ 
	delete this->link;
	this->link = link; 
}

void Packet::setNet( L3 *net ) 
{
	delete this->net; 
	this->net = net; 
}
void Packet::setTrans( L4 *trans ) 
{ 
	delete this->trans;
	this->trans = trans; 
}

L2 *Packet::getLink() 
{ 
	return this->link; 
}

L3 *Packet::getNet() 
{
	return this->net; 
}

L4 *Packet::getTrans() 
{ 
	return this->trans;
}
std::string Packet::srcMac()
{
	return this->link->srcMac();
}
std::string Packet::dstMac()
{
	return this->link->dstMac();
}
std::string Packet::srcIp()
{
	return this->net->srcIp();
}
std::string Packet::dstIp()
{
	return this->net->dstIp();
}
std::string Packet::srcPort()
{
	return this->trans != NULL ? this->trans->srcPort() : "";
}
std::string Packet::dstPort()
{
	return this->trans != NULL ? this->trans->dstPort() : "";
}

std::string Packet::toString()
{
	std::stringstream out("");
    out << number << ": " << std::dec << 1000000 * this->ts.tv_sec + this->ts.tv_usec << " " << this->len << " | " << link->toString() << " | " << net->toString();
    if( trans != NULL ) {
    	out << " | " << trans->toString();
    }
    return out.str(); 
}