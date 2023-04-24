/**
 * @file    ProgramDescriptor.h
 * @author  Karel Ondřej
 * 
 * @brief   Třída implementuje zpracování mapování programu na službu. 
 */
#ifndef PROGRAM_DESCRIPTION_H__
#define PROGRAM_DESCRIPTION_H__

#include "Descriptor.h"

/**
 * @brief Třída implementuje zpracování mapování programu na službu. 
 * 
 * Popisovač se nachází v tabulce PAT a obsahuje informace o přiřazení PID k 
 * identifikátoru služby.
 */
class ProgramDescriptor : public Descriptor
{
public:
    /**
     * @brief Konstruktor třídy.
     * 
     * @param[in] bytearray pole bytů obsahující informace k zpracování
     */
    ProgramDescriptor(bytearray_t bytearray);

    /**
     * @brief Virtuální destruktor třídy.
     */
    ~ProgramDescriptor() = default;
protected:
    virtual void parseBinary();
};

#endif // PROGRAM_DESCRIPTION_H__