/**
 * @file    ServiceDescriptionTable.h
 * @author  Karel Ondřej
 * 
 * @brief   Třída implementující zpracování tabulky SDT.
 */
#ifndef SERVICE_DESCRIPTION_TABLE_H__
#define SERVICE_DESCRIPTION_TABLE_H__

#include "ServiceInformationTable.h"


/**
 * @brief Třída implementuje zpracování tabulky SDT.
 * 
 * Tabulka SDT mimo jiné obsahuje přiřazení identifikátoru služby k jménu
 * providera a služby.
 */
class ServiceDescriptionTable : public ServiceInformationTable
{
public:
    /**
     * @brief Konstruktor třídy.
     * 
     * @param[in] bytearray pole bytů k zpracování
     */
    ServiceDescriptionTable(bytearray_t bytearray);

    /**
     * @brief Destruktor třídy.
     */
    virtual ~ServiceDescriptionTable() = default;
protected:
    virtual void parseBinary();
};

#endif // SERVICE_DESCRIPTION_TABLE_H__