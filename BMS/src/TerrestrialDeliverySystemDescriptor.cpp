/**
 * @file   TerrestrialDeliverySystemDescriptor.cpp
 * @author  Karel Ondřej
 * 
 * @brief
 */
#include "TerrestrialDeliverySystemDescriptor.h"

const std::string TerrestrialDeliverySystemDescriptor::STRING_BANDWIDTH_TABLE[] = {"8 MHz", "7 MHz", "6 MHz", "5 MHz", "NaN", "NaN", "NaN", "NaN"};
const std::string TerrestrialDeliverySystemDescriptor::STRING_CONSTELLATION_TABLE[] = {"QPSK", "16-QAM", "64-QAM", ""};
const std::string TerrestrialDeliverySystemDescriptor::STRING_CODE_RATE_TABLE[] = {"1/2", "2/3", "3/4", "5/6", "7/8", "", "", ""};
const std::string TerrestrialDeliverySystemDescriptor::STRING_GUARD_INTERVAL_TABLE[] = {"1/32", "1/16", "1/8", "1/4"};

TerrestrialDeliverySystemDescriptor::TerrestrialDeliverySystemDescriptor(std::vector<uint8_t> bytearray)
    : Descriptor(Descriptor::TERRESTRIAL_DELIVERY_SYSTEM_TAG, bytearray)
{
    this->parseBinary();
}

void TerrestrialDeliverySystemDescriptor::parseBinary()
{
    uint8_t header;
    int offset = 0;

    uint32_t centreFrequency = this->accept32Bits(offset);
    header = this->accept8Bits(offset);
    uint8_t bandwidth = (header & 0xe0) >> 5;
    header = this->accept8Bits(offset);
    uint8_t constelattion = (header & 0xc0) >> 6;
    uint8_t codeRateHP = (header & 0x5) >> 0;
    header = this->accept8Bits(offset);
    uint8_t codeRateLP = (header & 0xe0) >> 5;
    uint8_t guardInterval = (header & 0x18) >> 3;
    this->accept32Bits(offset);

    this->infos["centre_frequency"] = new Infobox(
        centreFrequency,
        std::to_string(centreFrequency));
    this->infos["bandwidth"] = new Infobox(
        bandwidth,
        TerrestrialDeliverySystemDescriptor::STRING_BANDWIDTH_TABLE[bandwidth]);
    this->infos["constellation"] = new Infobox(
        constelattion,
        TerrestrialDeliverySystemDescriptor::STRING_CONSTELLATION_TABLE[constelattion]);
    this->infos["code_rate-HP_stream"] = new Infobox(
        codeRateHP,
        TerrestrialDeliverySystemDescriptor::STRING_CODE_RATE_TABLE[codeRateHP]);
    this->infos["code_rate-LP_stream"] = new Infobox(
        codeRateLP,
        TerrestrialDeliverySystemDescriptor::STRING_CODE_RATE_TABLE[codeRateLP]);
    this->infos["guard_interval"] = new Infobox(
        guardInterval,
        TerrestrialDeliverySystemDescriptor::STRING_GUARD_INTERVAL_TABLE[guardInterval]);
}