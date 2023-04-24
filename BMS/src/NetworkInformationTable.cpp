/**
 * @file    NetworkInformationTable.cpp
 * @author  Karel Ondřej
 *
 * @brief
 */
#include <cassert>

#include "NetworkInformationTable.h"

NetworkInformationTable::NetworkInformationTable(bytearray_t payload)
    : ServiceInformationTable(ServiceInformationTable::NETWORK_INFORMATION_TABLE, payload),
      networkName("")
{
    this->parseBinary();
}

void NetworkInformationTable::parseBinary()
{
    uint16_t header;
    int offset = 1;
    int endOfLoop;

    header = this->accept16Bits(offset);

    assert((header & 0x8000) != 0);
    this->sectionLength = header & NetworkInformationTable::SECTION_LENGTH_MASK
                        >> NetworkInformationTable::SECTION_LENGTH_OFFSET;
    assert((this->sectionLength >> 10) == 0);
    this->networkID = this->accept16Bits(offset);
    this->skipBytes(offset, 3);
    header = this->accept16Bits(offset) & ServiceInformationTable::ONLY_12_BITS_MASK;
    endOfLoop = offset + header;

    while (offset < endOfLoop) {
        Descriptor* descriptor = this->descriptor(offset);
        this->descriptors[descriptor->tag].push_back(descriptor);
    }

    this->networkName = this->descriptors[Descriptor::NETWORK_NAME_TAG][0]->infos["name"]->sVal;
    uint16_t loopLenght = this->accept16Bits(offset) & NetworkInformationTable::LOOP_LENGTH_MASK
                        >> NetworkInformationTable::LOOP_LENGTH_OFFSET;

    endOfLoop = offset + loopLenght;
    while (offset < endOfLoop) {
        this->skipBytes(offset, 4);
        uint16_t transportDescriptorsLength = this->accept16Bits(offset) & ServiceInformationTable::ONLY_12_BITS_MASK;
        int endOfLoop2 = offset + transportDescriptorsLength;

        while (offset < endOfLoop2) {
            Descriptor* descriptor = this->descriptor(offset);
            this->descriptors[descriptor->tag].push_back(descriptor);
        }        
    }
}