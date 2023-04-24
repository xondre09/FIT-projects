/**
 * @file    ProgramDescriptor.cpp
 * @author  Karel Ondřej
 * 
 * @brief
 */
#include "ProgramDescriptor.h"

ProgramDescriptor::ProgramDescriptor(bytearray_t bytearray)
    : Descriptor(0x00, bytearray)
{
    this->parseBinary();
}

void ProgramDescriptor::parseBinary()
{
    int offset = 0;
    uint16_t programNumber = this->accept16Bits(offset);
    uint16_t programMapID = this->accept16Bits(offset) & 0x1fff;

    this->infos["program_number"] = new Infobox(programNumber, std::to_string(programNumber));
    this->infos["program_map_id"] = new Infobox(programMapID, std::to_string(programMapID));
}