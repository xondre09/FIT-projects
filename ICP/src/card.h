/*!
 * \file    card.h
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    5. 5. 2017
 *
 * \brief   Implemetace třídy reprezentující údaje o kartě.
 */

#include <iomanip>
#include <fstream>

/*! Konstanty ke hře Solitaire Klondike. */
namespace Solitaire {
    /*! \brief Výčet hodnot karet.
     *
     */
    enum Value{Ace,     /*!< 0  */
               Two,     /*!< 1  */
               Three,   /*!< 2  */
               Four,    /*!< 4  */
               Five,    /*!< 5  */
               Six,     /*!< 6  */
               Seven,   /*!< 7  */
               Eight,   /*!< 8  */
               Nine,    /*!< 8  */
               Ten,     /*!< 9  */
               Jack,    /*!< 10 */
               Queen,   /*!< 11 */
               King     /*!< 12 */
              };
    /*! \brief Výčet barev karet.
     *
     */
    enum Color {Club, Spade, Diamond, Heart};
}

/*!
 * \brief Třída implementuje jednu kartu.
 */
class Card {
    Solitaire::Value value; //! Hodnota karty (eso, král, ...)
    Solitaire::Color color; //! Barva karty (listy, káry, ...)
    bool visibility;        //! Viditelnost karty (zda je otočena rubem nebo lícem)
    int x;                  //! Hromádka na které se nachází
    int y;                  //! Udává kolíkátou kartou je na hromádce
    bool moveable;          //! Pohyblivost karty

public:
    /*!
     * \brief Konstruktor třídy Card.
     *
     * Převezme data  předané instace třídy.
     * \param card Karta s daty.
     */
    Card(Card &card);
    /*!
     * \brief Konstruktor třídy Card.
     *
     * Nastaví prvnotní hodnoty atributů. Původní hodnota atributu x = 0, y = 0 a moveable false.
     * \param value Hodnota karty z výčtu Solitaire::Value
     * \param color Barva karty z výčtu Solitaire::Color
     * \param vis   Viditelnost karty. Líc karty true, rub false.
     */
    Card(Solitaire::Value value, Solitaire::Color color, bool vis);
    /*!
     * \brief Uloží atributy do streamu.
     * \param s    Stream, do kterého se má uložit.
     * \param card Karta k uložení
     */
    friend std::ostream &operator<<(std::ostream &s, Card &card);
    /*!
     * \brief Načte kartu ze streamu.
     * \param s    Stream, ze kterého se nmá načíst
     * \param card Karta, do kterého se uloží hodnoty
     */
    friend std::istream &operator>>(std::istream &s, Card &card);
    /*!
     * \brief Nastaví hodnotu karty.
     * \param value Hodnota karty z výčtu Solitaire::Value
     */
    void setValue(Solitaire::Value value);
    /*!
     * \brief Vrací honotu karty.
     * \return Hodnota karty z výčtu Solitaire::Value
     */
    Solitaire::Value getValue(void);
    /*!
     * \brief Nastavý barvu karty.
     * \param color barva karty z výčtu Solitaire::Color
     */
    void setColor(Solitaire::Color color);
    /*!
     * \brief Vrací Barvu karty.
     * \return Barva karty z výčtu Solitaire::Color
     */
    Solitaire::Color getColor(void);
    /*!
     * \brief Nastaví viditelnost karty.
     * \param f Pokud je karta otočená lícem tak true, pokud rubem tak false
     */
    void setVisibility(bool f);
    /*!
     * \brief Zda je karta otočená lícem nebo rubem.
     * \return Pokud je karta otočená lícem vrací true, pokud rubem false
     */
    bool isVisibility(void);
    /*!
     * \brief Pozice karty na stole.
     *
     * Argument x_ určuje hromádku, kde se karta nachází a y_ pořadí karty na hromádce.
     * \param x_ Index hromátky, kde se karta nachází
     * \param y_ Pořadí karty na hromádce (nejníž = 0)
     */
    void setPosition(int x_, int y_);
    /*!
     * \brief Nastavení pohyblivosti karty.
     *
     * Pokud se může s kartou hýbat, tak je nastavena na tru, jinak false.
     * \param f Pokud se dá s kartou hýbat true, jinak false
     */
    void setMoveable(bool f);
    /*!
     * \brief Vrací pohyblivost karty.
     * \return Pokud se dá s kartou hýbat true, jinak false
     */
    bool isMoveable(void);
    /*!
     * \brief Umístění na stole.
     * \return Vrací index hromátky, kde se karta nachází
     */
    int getCol(void);
    /*!
     * \brief Umístění na hromádce.
     * \return Vrací pořadí na hromádce.
     */
    int getRow(void);
};
