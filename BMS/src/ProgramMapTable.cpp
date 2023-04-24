/**
 * @file    ProgramMapTable.cpp
 * @author  Karel Ondřej
 * brief
 */
#include "ProgramMapTable.h"

ProgramMapTable::ProgramMapTable(int pid, bytearray_t bytearray)
    : ServiceInformationTable(pid, bytearray)
{
    this->parseBinary();
}

void ProgramMapTable::parseBinary()
{
    int offset = 1;
    uint16_t sectionLength = (this->accept16Bits(offset) & 0x0fff);
    int sectionEnd = sectionLength + offset;
    uint16_t programNumber = this->accept16Bits(offset);
    Descriptor* descriptor = new Descriptor(0x0, this->bytearray);
    descriptor->infos["program_number"] = new Infobox(programNumber, "");
    this->descriptors[0].push_back(descriptor);
    this->skipBytes(offset, 5);
    uint16_t programInfoLenght = this->accept16Bits(offset) & 0x0fff;
    this->skipBytes(offset, programInfoLenght);

    while (offset < sectionEnd) {
        this->skipBytes(offset, 1);
        uint16_t elementaryPID = this->accept16Bits(offset) & 0x1fff;
        uint16_t descriptionLength = this->accept16Bits(offset) & 0x0fff;
        if (elementaryPID != 0x1FFF) {
            Descriptor* descriptor = new Descriptor(0x0, this->bytearray);
            descriptor->infos["elementary_pid"] = new Infobox(elementaryPID, "");
            this->descriptors[1].push_back(descriptor);
        }
        this->skipBytes(offset, descriptionLength);
    }
}