/**
 * @file    Infobox.h
 * @author  Karel Ondřej
 * 
 * @brief   Třída pro uchování informací z tabulek.
 */
#ifndef INFOBOX_H__
#define INFOBOX_H__

#include <string>   // std::string

/** 
 * @brief Třída pro uchování informací z tabulek.
 * 
 * V infoboxech je uložena hodnota položky zpracovaná ze streamu (pokud se nejedná o string) a také její 
 * interpretace pomocí řetězce znaků.
 */
class Infobox
{
public:
    /** @brief Číselná hodnota zpracovaná ze streamu. */
    int iVal;

    /** @brief Interpretace číselné hodnoty jako řetězec. */
    std::string sVal;

    /**
     * @brief Konstruktor třídy.
     * 
     * @param[in]  iVal  číselná hodnota
     * @param[out] sVal  interpretace řetězce
     */
    Infobox(int iVal, std::string sVal) : iVal(iVal), sVal(sVal) {}

    /**
     * @brief Destruktor třídy.
     */
    ~Infobox() = default;
};

#endif // INFOBOX_H