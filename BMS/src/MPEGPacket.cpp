/**
 * @file    MPEGPacket.cpp
 * @author  Karel Ondřej
 * 
 * @brief
 */
#include <cassert>
#include <fstream>

#include "MPEGPacket.h"


std::vector<MPEGPacket*> MPEGPacket::read(std::string filename)
{
    std::ifstream myfile;
    std::vector<MPEGPacket*> packets;

    char memblock[MPEGPacket::PACKET_LENGTH];

    myfile.open(filename);

    if (! myfile.is_open()) {
        std::cerr << "File '" << filename << "' doesn't exists." << std::endl;
        exit(1);
    }

    myfile.read(memblock, MPEGPacket::PACKET_LENGTH);

    while (! myfile.eof()) {
        MPEGPacket* packet = new MPEGPacket();
        myfile >> (*packet);
        packets.push_back(packet);
    }
    myfile.close();

    return packets;
}

void MPEGPacket::parseBinary()
{
    int offset = 0;
    unsigned int length;
    uint32_t header;
    uint8_t syncByte;

    header = this->accept32Bits(offset);
    syncByte = (header & MPEGPacket::SYNC_BYTE_MASK) 
             >> MPEGPacket::SYNC_BYTE_OFFSET;

    assert(syncByte == MPEGPacket::SYNC_BYTE);

    this->transportErrorIndicator = (header & MPEGPacket::TEI_MASK) 
                                  >> MPEGPacket::TEI_OFFSET;
    this->paylodUnitStartIndicator = (header & MPEGPacket::PUSI_MASK) 
                                   >> MPEGPacket::PUSI_OFFSET;
    this->transportPriority = (header & MPEGPacket::TRANSPORT_PRIORITY_MASK) 
                            >> MPEGPacket::TRANSPORT_PRIORITY_OFFSET;
    this->pid = (header & MPEGPacket::PID_MASK) >> MPEGPacket::PID_OFFSET;

    if (this->transportErrorIndicator) {
        std::cerr << "Corrupted packet." << std::endl;
    }

    this->transportScramblingControl = (header & MPEGPacket::TSC_MASK) 
                                     >> MPEGPacket::TSC_OFFSET;
    this->adaptionFieldControl = (header & MPEGPacket::ADAPTION_FIELD_CONSTROL_MASK) 
                               >> MPEGPacket::ADAPTION_FIELD_CONSTROL_OFFSET;
    this->continuityCounter = (header & MPEGPacket::CONTINUITY_COUNTER_MASK) 
                            >> MPEGPacket::CONTINUITY_COUNTER_OFFSET;

    switch (this->adaptionFieldControl) {
    case MPEGPacket::ADFC_PAYLOAD_ONLY:
        length = offset + paylodUnitStartIndicator;
        this->payload.assign(this->bytearray.begin()+length, 
                             this->bytearray.end()); 
        break;
    case MPEGPacket::ADFC_ADAPTION_FIELD_AND_PAYLOAD:
        length = this->accept8Bits(offset);
        length += offset + paylodUnitStartIndicator;
        this->payload.assign(this->bytearray.begin()+length, 
                             this->bytearray.end()); 
        this->extractPRC();
        break;
    case MPEGPacket::ADFC_ADAPTION_FIELD_ONLY:
        this->extractPRC();
    default:
        break;
    }
}

void MPEGPacket::extractPRC()
{
    int offset = 0;
    this->skipBytes(offset, 5);

    int flags = this->accept8Bits(offset);

    this->prcPresent = (flags & MPEGPacket::PROGRAM_CLOCK_REFERENCE_FLAG_MASK)
                     >> MPEGPacket::PROGRAM_CLOCK_REFERENCE_FLAG_OFFSET;

    if (this->prcPresent)
    {
        uint64_t prcField = ((uint64_t)this->accept16Bits(offset) << 32) 
                          | this->accept32Bits(offset);
        int base = (prcField & MPEGPacket::PROGRAM_CLOCK_REFERENCE_BASE_MASK)
                 >> MPEGPacket::PROGRAM_CLOCK_REFERENCE_BASE_OFFSET;
        int extencion = (prcField & MPEGPacket::PROGRAM_CLOCK_REFERENCE_EXTENCION_MASK)
                      >> MPEGPacket::PROGRAM_CLOCK_REFERENCE_EXTENCION_OFFSET;

        this->prc = base * 300 + extencion;
    }

}

std::istream& MPEGPacket::parseStream(std::istream& is)
{
    char buffer[MPEGPacket::PACKET_LENGTH];

    is.read(buffer, MPEGPacket::PACKET_LENGTH);
    this->bytearray.assign((uint8_t*)buffer, (uint8_t*)(buffer + MPEGPacket::PACKET_LENGTH));
    
    this->parseBinary();

    return is;
}

std::istream& operator>> (std::istream& is, MPEGPacket& packet) 
{
    return packet.parseStream(is);
}