/**
 * @file    ServiceDescriptor.cpp
 * @author  Karel Ondřej
 *
 * @brief
 */
#include "ServiceDescriptor.h"

ServiceDescriptor::ServiceDescriptor(bytearray_t bytearray)
    : Descriptor(Descriptor::SERVICE_TAG, bytearray)
{
    this->parseBinary();
}

void ServiceDescriptor::parseBinary()
{
    int offset = 0;
    std::string providerName;
    std::string serviceName;

    this->skipBytes(offset, 1);
    uint8_t serviceProviderNameLength = this->accept8Bits(offset);
    int endOfProviderName = offset + serviceProviderNameLength;
    while (offset < endOfProviderName) {
        providerName += ((char)this->accept8Bits(offset));
    }

    uint8_t serviceNameLength = this->accept8Bits(offset);
    int endOfServiceName = offset + serviceNameLength;
    while (offset < endOfServiceName) {
        serviceName += ((char)this->accept8Bits(offset));
    }

    this->infos["service_provider_name"] = new Infobox(-1, providerName);
    this->infos["service_name"] = new Infobox(-1, serviceName);
}