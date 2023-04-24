/**
 * @file     ProgramAssociationTable.h
 * @author   Karel Ondřej
 * 
 * @brief    Třída pro zpracování tabulky PAT.
 */
#ifndef PROGRAM_ASSOCIATION_TABLE_H__
#define PROGRAM_ASSOCIATION_TABLE_H__

#include "ServiceInformationTable.h"


/** 
 * @brief Třída pro zpracování tabulky PAT.
 * 
 * Tabulka PAT mimo jiné obsahuje informace o přiřazení PID programu k
 * identifikátoru služby.
 */
class ProgramAssociationTable : public ServiceInformationTable
{
public:
    /**
     * @brief Konstruktor třídy.
     * 
     * @param[in] bytearray pole bytů k zpracování
     */
    ProgramAssociationTable(bytearray_t bytearray);

    /**
     * @brief Destruktor třídy.
     */
    ~ProgramAssociationTable() = default;
protected:
    virtual void parseBinary();
private:
    /**
     * @brief Metoda pro zpravání popisovače.
     * 
     * První byte určuje druh popisovače, který nýsledně zpracuje konkrétní 
     * třída. Jedná se o přetíženou metodu.
     * 
     * @param[in,out] offset Offset, kde začíná popisovač. Offset je po skončení 
     *                       metody posunut za poslední byte popisovače.
     * @return zpracovaný popisovač
     */
    Descriptor* descriptor(int* offset);
};

#endif // PROGRAM_ASSOCIATION_TABLE_H__