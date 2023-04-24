/**
 * @file    BytearrayParser.h
 * @author  Karel Ondřej
 * 
 * @brief
 */
#ifndef BYTEARRAY_PARSER_H__
#define BYTEARRAY_PARSER_H__

#include <cstdint>  // uint8_t, uint16_t, uint32_t
#include <vector>   // std::vector

/** seznam bytů pro potřeby zpracování */
typedef std::vector<uint8_t> bytearray_t;

/** 
 * @brief Třída pro snadné zpracování stremu bytů. 
 * 
 * Třída nezpracovává samotný stream, ale poskytuje metody pro usnadnění zpracování.
 */
class BytearrayParser
{
public:
    /** @brief Konstruktor třídy.
     */
    BytearrayParser() = default;

    /** 
     * @brief Konstruktor třídy.
     * 
     * @param[in] bytearray stream bytů k zpracování
     */
    BytearrayParser(bytearray_t bytearray);

    /** @brief Destruktor třídy
     */
    virtual ~BytearrayParser() = default;

    /** 
     * @brief Metoda vrátí 8 bitů od ukazatele a následně ho posune za právě 
     * přečtené data.
     * 
     * @param[in,out] offset offset v bytech od začátku streamu
     * @return 8 bitů od ukazatele offset
     */
    uint8_t accept8Bits(int& offset);

    /** 
     * @brief Metoda vrátí 16 bitů od ukazatele a následně ho posune za právě 
     * přečtené data.
     * 
     * @param[in,out] offset offset v bytech od začátku streamu
     * @return 16 bitů od ukazatele offset
     */
    uint16_t accept16Bits(int& offset);

    /** 
     * @brief Metoda vrátí 32 bitů od ukazatele a následně ho posune za právě 
     * přečtené data.
     * 
     * @param[in,out] offset offset v bytech od začátku streamu
     * @return 32 bitů od ukazatele offset
     */
    uint32_t accept32Bits(int& offset);

    /** 
     * @brief Metoda posune ukazalel o zadaný počet bytů.
     * 
     * @param[in,out] offset ukazatel v bytech od začátku stremu
     * @param[in]     bytes  počet bytů, o které se má ukazatel posunout
     */
    void skipBytes(int& offset, int bytes);

    /** 
     * @brief Metoda vrátí část streamu od zadané začátku až po zadaný konec.
     * 
     * @param[in] start začátek
     * @param[in] end   konec
     * @return část stremu od paramentru strat, až po parametr end
     */
    bytearray_t subarray(int start, int end);
protected:
    /** @brief Stream bytů, které se zpracovávají třídou. */
    bytearray_t bytearray;

    /** @brief Metoda pro zpracování stremu bytů.
     */
    virtual void parseBinary() = 0;
};

#endif // BYTEArRAy_PARSER_H__