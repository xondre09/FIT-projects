/**
 * @file    TerrestrialDeliverySystemDescriptor.h
 * @author  Karel Ondřej
 * 
 * @brief   Třída implementující zpracování informací o síti.
 */
#ifndef TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_H__
#define TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_H__

#include "Descriptor.h"


/** 
 * @brief Třída implementuje zpracování informací o síti.
 */
class TerrestrialDeliverySystemDescriptor : public Descriptor
{
public:
    /** 
     * @brief Přiřazení řetězce popisující rychlost Bandwidth k získané hodnotě
     * z pole bytů.
     */
    static const std::string STRING_BANDWIDTH_TABLE[];
    /**
     * @brief Přiřazení řetězce popisující modulaci získanou z pole bytů
     */
    static const std::string STRING_CONSTELLATION_TABLE[];
    /**
     * @brief Přiřazení řetězce popisující Code Rate získaný z pole bytů. 
     */
    static const std::string STRING_CODE_RATE_TABLE[];
    /**
     * @brief Přiřazení řetězce popisující Guard Interval získaný z pole bytů.
     * */
    static const std::string STRING_GUARD_INTERVAL_TABLE[];

    /**
     * @brief Konstruktor třídy.
     * 
     * @param[in] binary ypole bytů k zpracování
     */
    TerrestrialDeliverySystemDescriptor(bytearray_t binary);
protected:
    virtual void parseBinary();
};

#endif // TERRESTRIAL_DELIVERY_SYSTEM_DESCRIPTOR_H__