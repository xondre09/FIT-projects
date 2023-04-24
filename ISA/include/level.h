/*!
 * \file    level.h
 * \authors Karel ONDŘEJ, FIT
 * \date    13. 11. 2017
 *
 * \brief   Rozhraní pro jednotlivé vrstvy TCP/IP.
 */

#ifndef _LEVEL_H_
#define _LEVEL_H_


#include <stdint.h>
#include <string>
#include <sstream>
#include <iomanip>

#include <arpa/inet.h>

/**
 * \brief Vrstva síťových rozhraní
 */
class L2 {
public:
	virtual ~L2() {}
    virtual std::string srcMac() = 0;
    virtual std::string dstMac() = 0;
    virtual std::string toString() = 0;
};


/**
 * \brief Síťová (IP) vrstva.
 */
class L3 {
public:
	virtual ~L3() {}
    virtual std::string srcIp() = 0;
    virtual std::string dstIp() = 0;
    virtual std::string toString() = 0;
};

/**
 * \brief Transportní vrstva
 */
class L4 {
public:
	virtual ~L4() {}
    virtual std::string srcPort() = 0;
    virtual std::string dstPort() = 0;
    virtual std::string toString() = 0;
};
#endif