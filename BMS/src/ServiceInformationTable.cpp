/**
 * @file    ServiceInformationTable.cpp
 * @author  Karel Ondřej
 * 
 * @brief
 */
#include "NetworkInformationTable.h"
#include "ProgramAssociationTable.h"
#include "ProgramMapTable.h"
#include "ServiceDescriptionTable.h"
#include "ServiceInformationTable.h"


ServiceInformationTable::ServiceInformationTable(uint16_t pid, bytearray_t payload)
    : BytearrayParser(payload),
      pid(pid),
      tableID(payload[0])
{
}

ServiceInformationTable::~ServiceInformationTable()
{
    for (auto descriptorVec: this->descriptors) {
        for (auto descriptor: descriptorVec.second) {
            delete descriptor;
        }
    }
}

Descriptor* ServiceInformationTable::descriptor(int& offset)
{
    uint8_t tag = this->accept8Bits(offset);
    uint8_t length = this->accept8Bits(offset);
    bytearray_t descriptorBytearray = this->subarray(offset, offset+length);

    Descriptor* descriptor = Descriptor::createDescriptor(tag, descriptorBytearray);

    offset += length;
    return descriptor;
}

void ServiceInformationTable::parseBinary()
{
}

ServiceInformationTable* ServiceInformationTable::createTable(uint16_t pid, bytearray_t payload)
{
    ServiceInformationTable* table;
    switch (pid)
    {
    case ServiceInformationTable::PROGRAM_ASSOCIATION_TABLE:
        table = new ProgramAssociationTable(payload);
        break;
    case ServiceInformationTable::NETWORK_INFORMATION_TABLE:
        table = new NetworkInformationTable(payload);
        break;
    case ServiceInformationTable::SERVICE_DESCRIPTION_TABLE:
        table = new ServiceDescriptionTable(payload);
        break;
    default:
        if (payload[0] == 0x02) {
            table = new ProgramMapTable(pid, payload);
        }
        else {
            table = new ServiceInformationTable(pid, payload);
        }
        break;
    }
    return table;
}

std::vector<ServiceInformationTable*> ServiceInformationTable::tablesFromPackets(std::vector<MPEGPacket*> packets)
{
    std::map<uint16_t, std::vector<uint8_t>> pid2stream;
    std::vector<ServiceInformationTable*> SITables;

    ServiceInformationTable* table;

    for (auto packet: packets) {
        if (packet->pid == 8191) {
            table = ServiceInformationTable::createTable(packet->pid, packet->payload);
            SITables.push_back(table);
        }
        else if (packet->paylodUnitStartIndicator) {
            auto search = pid2stream.find(packet->pid);
            if (search != pid2stream.end()) {
                table = ServiceInformationTable::createTable(search->first, search->second);
                SITables.push_back(table);
            }
            pid2stream[packet->pid] = packet->payload;
        }
        else {
            auto search = pid2stream.find(packet->pid);
            if (search != pid2stream.end()) {
                search->second.insert(search->second.end(), 
                                      packet->payload.begin(),
                                      packet->payload.end());
            }
        }
    }

    for(auto stream: pid2stream) {
        table = ServiceInformationTable::createTable(stream.first, stream.second);
        SITables.push_back(table);
    }

    return SITables;
}