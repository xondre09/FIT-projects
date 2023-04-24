/**
 * @file    NetworkNameDescriptor.cpp
 * @author  Karel Ondřej
 * 
 * @brief
 */
#include "NetworkNameDescriptor.h"

NetworkNameDescriptor::NetworkNameDescriptor(bytearray_t binary)
    : Descriptor(Descriptor::NETWORK_NAME_TAG, binary)
{
    this->parseBinary();
}

void NetworkNameDescriptor::parseBinary()
{
    std::string name(this->bytearray.begin(), this->bytearray.end());
    this->infos["name"] = new Infobox(-1, name);
}