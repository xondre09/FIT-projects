/**
 * @file    BytearrayParser.cpp
 * @author  Karel Ondřej
 * 
 * @brief
 */
#include "BytearrayParser.h"

BytearrayParser::BytearrayParser(bytearray_t bytearray)
    : bytearray(bytearray)
{
}

uint8_t BytearrayParser::accept8Bits(int& offset)
{
    uint8_t result = this->bytearray[offset];
    offset++;
    return result;
}

uint16_t BytearrayParser::accept16Bits(int& offset)
{
    uint16_t result = (this->bytearray[offset] << 8) 
                    | (this->bytearray[offset+1]);
    offset += 2;
    return result;
}

uint32_t BytearrayParser::accept32Bits(int& offset)
{
    uint32_t result = (this->bytearray[offset] << 24) 
                    | (this->bytearray[offset+1] << 16) 
                    | (this->bytearray[offset+2] << 8) 
                    | (this->bytearray[offset+3]);
    offset += 4;
    return result;    
}

void BytearrayParser::skipBytes(int& offset, int bytes)
{
    offset += bytes;
}

bytearray_t BytearrayParser::subarray(int start, int end)
{
    return bytearray_t(this->bytearray.begin() + start, 
                       this->bytearray.begin() + end);
}