/**
 * @file    Descriptor.h
 * @author  Karel Ondřej
 * 
 * @brief   Abstraktní třída pro zpracování popisovačů v tabulkách.
 */
#ifndef DESCRIPTOR_H__
#define DESCRIPTOR_H__

#include <cstdint>              // uint8_t 
#include <map>                  // std::map
#include <string>               // std::string

#include "BytearrayParser.h"    // BytearrayParser, bytearray_t
#include "Infobox.h"            // Infobox

/**
 * @brief Abstraktní třída pro zpracování popisovačů v tabulkách.
 */
class Descriptor : public BytearrayParser
{
public:
    /** @brief Identifikátor typu popisovače. */
    uint8_t tag;

    /**
     * @brief Konstruktor třídy.
     * 
     * @param[in] tag       identifikátor popisovače
     * @param[in] bytearray stream bytů k zpracování
     */
    Descriptor(uint8_t tag, bytearray_t bytearray);

    /**
     * @brief Virtuální destruktor třídy.
     */
    virtual ~Descriptor();

    /**
     * @brief Informace z popisovače. Klíčem je název z dokumentace pro 
     * konkrétní informaci zpracovánou ze streamu.
     */
    std::map<std::string, Infobox*> infos;
    
    /** @brief Identifikátor popisovače pro jméno sítě.*/
    static const uint8_t NETWORK_NAME_TAG = 0x40;

    /** 
     * @brief Indetifikátor pro Service Descriptor. Popisovač mimo jiné obsahuje 
     * jméno služby a providera a identifikátor služby. 
     */
    static const uint8_t SERVICE_TAG = 0x48;

    /** 
     * @brief Identifikátor pro Terrestrual Delivery System Descriptor. 
     * Popisovač  obsahuje obecné informace o multiplexu. 
     */
    static const uint8_t TERRESTRIAL_DELIVERY_SYSTEM_TAG = 0x5a;

    /** 
     * @brief Statická metoda, která vytvoří konkrétní instanci popisovače na 
     * základě identifikátoru.
     * 
     * @param[in] tag        identifikátor popisovače
     * @param[in] bytearray  stream k zpracování
     * @return konkrétní instance třídy Descriptor podle zadaného identifikátoru
     */
    static Descriptor* createDescriptor(uint8_t tag, bytearray_t bytearray);
protected:

    /** 
     * @brief Zpracování zadaného streamu (viz @see BinaryParser::parseBinary()).
     */
    virtual void parseBinary();
};

#endif // DESCRIPTOR_H__