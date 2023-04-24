/**
 * @file    Descriptor.cpp
 * @author  Karel Ondřej
 * 
 * @brief
 */
#include "Descriptor.h"
#include "NetworkNameDescriptor.h"
#include "ServiceDescriptor.h"
#include "TerrestrialDeliverySystemDescriptor.h"

Descriptor::Descriptor(uint8_t tag, bytearray_t bytearray)
    : BytearrayParser(bytearray),
      tag(tag)
{
}

Descriptor::~Descriptor()
{
    for(auto item: this->infos) {
        delete item.second;
    }
}

void Descriptor::parseBinary()
{
}

Descriptor* Descriptor::createDescriptor(uint8_t tag, bytearray_t bytearray)
{
    Descriptor* descriptor;

    switch (tag) {
    case Descriptor::NETWORK_NAME_TAG:
        descriptor = new NetworkNameDescriptor(bytearray);
        break;
    case Descriptor::SERVICE_TAG:
        descriptor = new ServiceDescriptor(bytearray);
        break;
    case Descriptor::TERRESTRIAL_DELIVERY_SYSTEM_TAG:
        descriptor = new TerrestrialDeliverySystemDescriptor(bytearray);
        break;
    default:
        descriptor = new Descriptor(tag, bytearray);
        break;
    }
    return descriptor;
}