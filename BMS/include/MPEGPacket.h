/**
 * @file    MPEGPacket.h
 * @author  Karel Ondřej
 * 
 * @brief   Třída pro zpracování transportního paketu. 
 */
#ifndef MPEG_PACKET_H__
#define MPEG_PACKET_H__

#include <cstdint>  // uint8_t, uint16_t, uint32_t
#include <iostream> // std::istream
#include <string>   // std::string
#include <vector>   // std::vector

#include "BytearrayParser.h"    // Bytearray


/** 
 * @brief Třída pro zpracování transportního paketu. 
 */
class MPEGPacket : public BytearrayParser {
public:
    /** @brief Délka paketu v bytech. */
    static const uint8_t PACKET_LENGTH = 188;

    /** 
     * @brief Statická metoda pro zpracování transportních paketů ze souboru.
     * 
     * @param[in] filename název souboru
     * @return posloupnost zpracovaných paketů ze zadaného souboru
     */
    static std::vector<MPEGPacket*> read(std::string filename);

    /** 
     * @brief Přetížený operátor '>>' pro zpracování jednoho paketu ze streamu.
     * 
     * @param[in]  is      stream ze kterého se má paket zpracovat
     * @param[out] packet  paket do kterého budou uloženy informace ze streamu
     * @return stream bez zpracovaného paketu
     */
    friend std::istream& operator>> (std::istream& is, MPEGPacket& packet);

    /** @brief Příznak, zda je paket poškozen. */
    bool transportErrorIndicator;
    /** @brief Příznak zda se jedná o první paket tabulky. */
    bool paylodUnitStartIndicator;
    /** @brief Příznak pro prioritní paket. */
    bool transportPriority;
    /** @brief Identifikátor paketu. */
    uint16_t pid;
    /** @brief Příznak TSC (viz dokumentace DVB_T). */
    uint8_t transportScramblingControl;
    /** @brief Příznak, zda hlavička pokračuje Adaption hlavičkou, nebo 
     * užitečným nákladem. */
    uint8_t adaptionFieldControl;
    /** @brief Počítadlo paketů. */
    uint8_t continuityCounter;
    /** */
    bool prcPresent;
    /** */
    int prc;
    /** @brief Náklad uvnitř paketu. */
    bytearray_t payload;

protected:
    /** @see BytearrayParser::parseBinary()
     */
    virtual void parseBinary();

private:
    /** @brief Zpracování streamu bytů. Použito v přetíženém operátoru >>. 
     * 
     * @param[in] stream  stream bytů
     * @return stream, ze kterého byl zpracován jeden packet
     */
    std::istream& parseStream(std::istream& stream);

    /**
     * 
     */
    void extractPRC();

    /** @brief Hodnota synchornizačního byte. */
    static const uint8_t SYNC_BYTE = 'G';
    
    /** maska pro zpracování synchornizačního bytu */
    static const uint32_t SYNC_BYTE_MASK = 0xff000000;
    /** maska pro příznak TEI (@see transportErrorIndicator) */
    static const uint32_t TEI_MASK = 0x00800000;
    /** maska pro příznak PUSI (@see paylodUnitStartIndicator) */
    static const uint32_t PUSI_MASK = 0x00400000;
    /** maska pro prioritu trasportu  (@see transportPriority) */
    static const uint32_t TRANSPORT_PRIORITY_MASK = 0x00200000;
    /** maska pro PID packetu */
    static const uint32_t PID_MASK = 0x001fff00;
    /** maska pro TSC */
    static const uint32_t TSC_MASK = 0x000000c0;
    /** maska pro položku určující, zda paket obsahuje náklad a Adaption 
     * hlavičku */
    static const uint32_t ADAPTION_FIELD_CONSTROL_MASK = 0x00000030;
    /** maska pro počítadlo paketů */
    static const uint32_t CONTINUITY_COUNTER_MASK = 0x0000000f;
    /** */
    static const uint32_t PROGRAM_CLOCK_REFERENCE_FLAG_MASK = 0x010;
    /** */
    static const uint64_t PROGRAM_CLOCK_REFERENCE_BASE_MASK = 0x0000ffffffff8000;
    /** */
    static const uint64_t PROGRAM_CLOCK_REFERENCE_EXTENCION_MASK = 0x000000000000001ff;

    /** offset synchronizačního bytu */
    static const uint8_t SYNC_BYTE_OFFSET = 24;
    /** offset TEI příznaku */
    static const uint8_t TEI_OFFSET = 23;
    /** offset PUSI příznaku */
    static const uint8_t PUSI_OFFSET = 22;
    /** offset pro položku priority transportu */
    static const uint8_t TRANSPORT_PRIORITY_OFFSET = 21;
    /** offset pro identifikátor packetu*/
    static const uint8_t PID_OFFSET = 8;
    /** offset pro položku TSC */
    static const uint8_t TSC_OFFSET = 6;
    /** offset pro položku určující, zda paket obsahuje náklad a Adaption
     * hlavičku */
    static const uint8_t ADAPTION_FIELD_CONSTROL_OFFSET = 4;
    /** offset pro počítadlo packetů */
    static const uint8_t CONTINUITY_COUNTER_OFFSET = 0;
    /** */
    static const uint32_t PROGRAM_CLOCK_REFERENCE_FLAG_OFFSET = 4;
    /** */
    static const uint64_t PROGRAM_CLOCK_REFERENCE_BASE_OFFSET = 15;
    /** */
    static const uint64_t PROGRAM_CLOCK_REFERENCE_EXTENCION_OFFSET = 0;

    /** rezervovaná hodnota (@see ADAPTION_FIELD_CONTROL) */
    static const uint8_t ADFC_RESERVED = 0x0;
    /** po hlavičce pokračuje náklad (@see ADAPTION_FIELD_CONTROL) */
    static const uint8_t ADFC_PAYLOAD_ONLY = 0x1;
    /** po hlavičce pokračuje Adaption hlavička, která není následována 
     * nákladem */
    static const uint8_t ADFC_ADAPTION_FIELD_ONLY = 0x2;
    /** po hlavičce následuje Adaption položky, které následuje náklad */
    static const uint8_t ADFC_ADAPTION_FIELD_AND_PAYLOAD = 0x3;
};

#endif // MPEG_PACKET_H__