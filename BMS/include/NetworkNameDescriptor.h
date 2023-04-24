/**
 * @file    NetworkNameDescriptor.h
 * @author  Karel Ondřej
 * 
 * @brief   Zpracování jména sítě z pole bytů.
 */
#ifndef NETWORK_NAME_DESCRIPTOR_H__
#define NETWORK_NAME_DESCRIPTOR_H__

#include "Descriptor.h"     // Descriptor

/**
 * @brief Třída implementující zpracování jména sítě.
 * 
 * Popisovač se nachází v tabulce NIT.
 */
class NetworkNameDescriptor : public Descriptor 
{
public:
    /**
     * @brief Konstruktor třídy.
     * 
     * @param[in] binary pole bytů k zpracování
     */
    NetworkNameDescriptor(bytearray_t binary);
protected:
    virtual void parseBinary();
};

#endif // NETWORK_NAME_DESCRIPTOR_H__