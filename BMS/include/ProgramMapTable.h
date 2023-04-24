/**
 * @file    ProgramMapTable.h
 * @author  Karel Ondřej
 * 
 * @brief   Třída pro zpracování tabulky PMT.
 */
#ifndef PROGRAM_MAP_TABLE_H__
#define PROGRAM_MAP_TABLE_H__

#include "ServiceInformationTable.h"


/**
 * @brief Třída pro zpracování tabulky PMT.
 * 
 * Tabulky obsahuje mimo jiné seznam všech elementárních PID paketů, patřící 
 * k danému programu. Tabulka PMT je k programu přiřazena na základě svého
 * PID.
 */
class ProgramMapTable : public ServiceInformationTable
{
public:
    /**
     * @brief Konstruktor třídy.
     * 
     * @param[in] pid       PID tabulky
     * @param[in] bytearray pole bytů k zpracování
     */
    ProgramMapTable(int pid, bytearray_t bytearray);

    /**
     * @brief Destruktor třídy.
     */
    ~ProgramMapTable() = default;
protected:
    virtual void parseBinary();
};

#endif // PROGRAM_MAP_TABLE_H__