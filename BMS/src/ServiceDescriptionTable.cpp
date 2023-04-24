/**
 * @file    ServiceDescriptorTable.cpp
 * @author  Karel Ondřej
 * 
 * @brief
 */
#include <cassert>

#include "ServiceDescriptionTable.h"

ServiceDescriptionTable::ServiceDescriptionTable(bytearray_t bytearray)
    : ServiceInformationTable(ServiceInformationTable::SERVICE_DESCRIPTION_TABLE, bytearray)
{
    this->parseBinary();
}

void ServiceDescriptionTable::parseBinary()
{
    int offset = 0;
    uint16_t header;
    int endOfSection;

    this->skipBytes(offset, 1);    // skip table ID (extract within constuctor)

    header = this->accept16Bits(offset);
    assert((header >> 15) == 1);
    endOfSection = offset + (header & 0x0fff) - 4;
    this->skipBytes(offset, 8);
    while (offset < endOfSection) {
        uint16_t serviceID = this->accept16Bits(offset);
        this->skipBytes(offset, 1);
        header = this->accept16Bits(offset);
        uint16_t descriptionLoopLength = header & 0x0fff;
        int endOfLoop = std::min(offset + descriptionLoopLength, endOfSection);
        while(offset < endOfLoop) {
            Descriptor* descriptor = this->descriptor(offset);
            descriptor->infos["serviceID"] = new Infobox(serviceID, std::to_string(serviceID));
            this->descriptors[descriptor->tag].push_back(descriptor);
        }
    }
}

