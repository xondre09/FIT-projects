/**
 * @file    ServiceInformationTable.h
 * @author  Karel Ondřej
 * 
 * @brief
 */
#ifndef SERVICE_INFORMATION_TABLE_H__
#define SERVICE_INFORMATION_TABLE_H__

#include <cstdint>  // uint16_t
#include <map>      // std::map
#include <vector>   // std::vector

#include "BytearrayParser.h"    // BytearrayParser, bytearray_t
#include "Descriptor.h"         // Descriptor
#include "MPEGPacket.h"         // MPEGPacket

/**
 * @brief Třída zpracující a reprezentující DVB-T tabulky. 
 * 
 * Třída je rodičem všem ostatním tabulkám.
 */
class ServiceInformationTable : public BytearrayParser
{
protected:
    /** 
     * @brief Metoda pro zpracování popisovačů.
     * 
     * @param[in,out] offset ukazatel na první byte v zpracovávaném streamu
     * @return zpracovaný popisovač
     */
    Descriptor* descriptor(int& offset);

    /** @see BytearrayParser::parseBinary 
     */
    virtual void parseBinary();
public:
    /** @brief PID (identifikátor) packetů obsahující danou tabulku */
    uint16_t pid;

    /** @brief Identifikátor tabulky. */
    uint8_t tableID;

    /** 
     * @brief Seznam popisovačů tabulky.  
     * 
     * Klíčem je číselný typ popisovače.
     */
    std::map<int, std::vector<Descriptor*>> descriptors;

    /** 
     * @brief Konstruktor třídy.
     * 
     * @param[in] pid     PID packetu obsahující tabulku
     * @param[in] payload stream bytů k zpracování
     */
    ServiceInformationTable(uint16_t pid, bytearray_t payload);

    /** 
     * @brief Destruktor třídy.
     */
    ~ServiceInformationTable();

    /** 
     * @brief Statická metoda pro vytvoření tabulky. 
     *
     * Metoda dostane PID packetu a stream bytů, podle PID určí konkrétní třídu, 
     * kterou následně zpracuje stream.
     * 
     * @param[in] pid     PID packetu
     * @param[in] payload stream bytů
     * @return instance tabulky zpracované ze streamu
     */
    static ServiceInformationTable* createTable(uint16_t pid, bytearray_t payload);

    /** 
     * @brief Statická matoda pro zpracování DVB-T tabulek ze seznamů MPEG 
     * packetů.
     * 
     * @param[in] packets seznam packetů k zpracování
     * @return seznam zpracovaných tabulek
     */
    static std::vector<ServiceInformationTable*> tablesFromPackets(std::vector<MPEGPacket*> packets);

    /** @brief PID tabulky PAT. */
    static const uint16_t PROGRAM_ASSOCIATION_TABLE = 0x0000;

    /** @brief PID tabulky NIT. */
    static const uint16_t NETWORK_INFORMATION_TABLE = 0x0010;

    /** @brief PID tabulky SDT. */
    static const uint16_t SERVICE_DESCRIPTION_TABLE = 0x0011;

    /** @brief Maska pro spodních 12 bitů. */
    static const uint16_t ONLY_12_BITS_MASK = 0x0fff;
    /** @brief Maska pro spodních 13 bitů. */
    static const uint16_t ONLY_13_BITS_MASK = 0x1fff;
};

#endif // SERVICE_INFORMATION_TABLE_H__