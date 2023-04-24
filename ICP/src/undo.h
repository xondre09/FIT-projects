/*!
 * \file    undo.h
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    5. 5. 2017
 * \brief   Třídy reprezentující jeden tah a tah zpět.
 */

#ifndef UNDO_H
#define UNDO_H

/*!
 * \brief Třída reprezentující jeden tah ve hře Soliteire.
 */
class Move {
    int from;   //!< Hromádka, ze které pochází karta
    int to;     //!< Hromádka, kam se přesune karta
    int idx;    //!< Pozice karty na původní hromádce
public:
    /*!
     * \brief Konstruktor třídy Move.
     * \param from_ Hromádka, ze které se má karta přesunout
     * \param to_   Hromádka, kam se má karta přesunout
     * \param idx_  Pozice karty v původní hromádce
     */
    Move(int from_, int to_, int idx_);
    /*!
     * \brief Vrací hromádku, ze které se má přetáhnout karta.
     * \return Index hromádky
     */
    int getFrom(void);
    /*!
     * \brief  Vrací hromádku, kam se má přetáhnout karta.
     * \return Index hromádky
     */
    int getTo(void);
    /*!
     * \brief Vrací pozici karty v původní hromádce.
     * \return Pozice karty v původní hromádce
     */
    int getIdx(void);
};
/*!
 * \brief Třída reprezentující jeden tah zpět ve hře Soliteire.
 */
class Undo : public Move {
    int op;     //!< Druh operace
    int score;  //!< Skóre hry
public:
    /*!
     * \brief Konstruktor třídy Undo.
     * \param op_    Druh operace
     * \param from_  Hromádka, ze které se má karta přesunout
     * \param to_    Hromádka, kam se má karta přesunout
     * \param idx_   Pozice karty v původní hromádce
     * \param score_ Skóre před provedením tahu
     */
    Undo(int op_, int from_, int to_, int idx_, int score_);
    /*!
     * \brief Vrací druh operace tahu zpět.
     * \return Druh operace
     */
    int getOperation(void);
    /*!
     * \brief Vrací nové skóre po tahu zpět.
     * \return Skóre
     */
    int getScore(void);
};
#endif // UNDO_H
