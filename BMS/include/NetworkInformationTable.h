/**
 * @file    NetworkInformationTable.h
 * @author  Karel Ondřej
 * 
 * @brief
 */
#ifndef NETWORK_INFORMATION_TABLE_H__
#define NETWORK_INFORMATION_TABLE_H__

#include <cstdint>  // uint8_t, uint16_t
#include <string>   // std::string

#include "ServiceInformationTable.h"

class NetworkInformationTable : public ServiceInformationTable
{
public:
    uint16_t sectionLength;
    uint16_t networkID;
    uint8_t versionNumber;
    std::string networkName;
    
    NetworkInformationTable(bytearray_t payload);
    ~NetworkInformationTable() = default;
protected:
    virtual void parseBinary();
private:
    static const uint32_t SECTION_LENGTH_MASK = ServiceInformationTable::ONLY_12_BITS_MASK;
    static const uint32_t LOOP_LENGTH_MASK = ServiceInformationTable::ONLY_12_BITS_MASK;

    static const uint8_t SECTION_LENGTH_OFFSET = 0;
    static const uint8_t LOOP_LENGTH_OFFSET = 0;
};

#endif // NETWORK_INFORMATION_TABLE_H__