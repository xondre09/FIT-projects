/**
 * @file    ProgramAssociationTable.cpp
 * @author  Karel Ondřej
 * 
 * @brief
 */
#include "ProgramAssociationTable.h"
#include "ProgramDescriptor.h"

ProgramAssociationTable::ProgramAssociationTable(bytearray_t bytearray)
    : ServiceInformationTable(ServiceInformationTable::PROGRAM_ASSOCIATION_TABLE, bytearray)
{
    this->parseBinary();
}

void ProgramAssociationTable::parseBinary()
{
    int offset = 0;
    this->skipBytes(offset, 1);
    uint16_t sectionLength = this->accept16Bits(offset) & ServiceInformationTable::ONLY_12_BITS_MASK;
    int endOfSection = offset + sectionLength;
    this->skipBytes(offset, 5);

    while (offset < endOfSection) {
        bytearray_t descriptorBytearray = this->subarray(offset, offset+4);
        Descriptor* descriptor = new ProgramDescriptor(descriptorBytearray);
        this->descriptors[0].push_back(descriptor);
        offset += 4;
    }
}