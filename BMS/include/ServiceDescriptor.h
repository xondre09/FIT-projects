/**
 * @file    ServiceDescriptor.h
 * @author  Karel Ondřej
 * 
 * @brief   Zpracování jména služby a providera z pole bytů.
 */
#ifndef SERVICE_DESCRIPTION_H__
#define SERVICE_DESCRIPTION_H__

#include "Descriptor.h"


/** 
 * @brief Třída implementuje zpracování informací o službě.
 *  
 * Popisovač obsahuje jména služby a providera.
 */
class ServiceDescriptor : public Descriptor
{
public:
    /**
     * @brief Konstruktor třídy
     * 
     * @param[in] bytearry pole bytů k zpracování
     */
    ServiceDescriptor(bytearray_t bytearray);

    /**
     * @brief Destruktor třídy
     */
    ~ServiceDescriptor() = default;
protected:
    virtual void parseBinary();
};

#endif // SERVICE_DESCRIPTION_H__