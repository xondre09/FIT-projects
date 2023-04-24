/*!
 * \file    game.h
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    5. 5. 2017
 * \brief   Logika karetní hry Solitaire Klondike.
 */

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <random>

#include "card.h"
#include "undo.h"
#include "hint.h"

class Hint;

/*!
 * \brief Logika hry.
 *
 * Třída implementující logiku karetní hry Solitaire Klondike.
 */
class Game {
    std::vector<Card *> desk[12];   //! Hrací stůl
    unsigned int topDeck;           //! Vrchol balíčku
    unsigned int count;             //! Počet otáčených karet při táhnutí z balíčku
    int score;                      //! Skóre hry
    Hint *hints;                    //! Nápověda
    std::vector<Undo *> undo_v;     //! Předchozí tahy

private:
    /*!
     * \brief Pomocná funkce pro tah z balíčku.
     */
    void rollDeckUtil(void);
    /*!
     * \brief Metoda generuje všechny aktuálně možné tahy.
     */
    void generateHints(void);

public:
    /*!
     * \brief Konstruktor třídy Game.
     */
    Game();
    /*!
     * \brief Destruktor třídy Game.
     */
    ~Game();
    /*!
     * \brief Uložení stavu třídy do streamu.
     * \param out  Stream k uložení hry.
     * \param game Rozehrná hra, která bude uložena.
     */
    friend std::ostream & operator<<(std::ostream & out, Game &game);
    /*!
     * \brief Načtení rozehrané hry ze streamu.
     * \param out  Stream s uloženou hrou.
     * \param game Hra kam se uloží rozehraná hra.
     */
    friend std::istream & operator>>(std::istream & out, Game &game);
    /*!
     * \brief  Vrací jednu hromátku ze hry.
     *
     * Hra obsahuje celkem 12 hromádek (0-11). Hromádka na pozici 0 reprezentuje balíček, 1-7 herní hromádky a 8-11 hromádky k konečnému uložení karet.
     *
     * \param  i Index hromádky ve hře.
     * \return Vector s kartami.
     */
    std::vector<Card *>& operator[](int i);

    /*!
     * \brief Vyčištění hry.
     */
    void clear(void);
    /*!
     * \brief Zamíchání karet.
     *
     * Karty z jednoho herního balíčku (52 karet) jsou (pseudo)náhodně zamíchány a rozdány do hromádek 1-7, kde pořadí hromádky odpovídá počtu rozdaných karet. Zbylé karty jsou otočené směrem dolů na hromádce 0.
     */
    void staffleCards(void);
    /*!
     * \brief  Otočení první vrchní karty na hromádce.
     *
     * Nastaví viditelnost karty podle argumentu f.
     *
     * \param  col Hromádka k otočení vrchní karty (1-7)
     * \param  f   true pokud má být karta vidět jinak false
     * \return Vrací 0 pokud se nepovedlo otočit kartu.
     */
    int flipCard(int col, bool f);
    /*!
     * \brief Otočení Hromádky.
     *
     * Otočí hromádku 0.
     */
    void rollDeck(void);
    /*!
     * \brief Dosažené skóre.
     * \return Dosažené skóre ve hře.
     */
    int getScore(void);
    /*!
     * \brief Metoda indikuje, zda byla hra dohrána.
     * \return true pokud byla hra dohrána, jina false
     */
    bool isEnd();
    /*!
     * \brief Povolený tah.
     *
     * Metoda vrací true pokud lze přetáhnout kartu z jedné hromádky do druhé.
     * \param from Hromádka, ze které karta pochází
     * \param to   Hromádka, kam by se měla karta přesunout
     * \param idx  Index karty na hromádce, kde 0 je nejspodnější karta
     * \return Pokud lze kartu přemístit tak true jinak false
     */
    int isValidMove(int from, int to, int idx);
    /*!
     * \brief Přesunutí vrchní karty z hromádky na finální hromádku.
     *
     * Pokud lze kartu přemístit, tak metoda přemístí nejvrchnější kartu z jedné hromádky na finální hromádku.
     * \param  from Hromádka, ze které se karta přesune
     * \return Metoda vrací 0 pokud se nepovede kartu přesunout.
     */
    int move(int from);
    /*!
     * \brief Přesunutí vrchní karty z hromádky.
     *
     * Pokud lze kartu přemístit, tak metoda přemístí nejvrchnější kartu z jedné hromádky na druhou.
     * \param  from Hromádka, ze které se karta přesune
     * \param  to
     * \return Metoda vrací 0 pokud se nepovede kartu přesunout.
     */
    int move(int from, int to);
    /*!
     * \brief Přesunutí karet z hromádky.
     *
     * Pokud lze kartu přemístit, tak metoda přemístí kartu z hromádky from na idexu idx (a všechny vyšší) na hromádku to.
     * \param  from Hromádka, ze které se karty přesunou
     * \param  to   Hromádka, kam se karty přesunou
     * \param  idx  Umístění karty na zdrojové hromádce
     * \return Metoda vrací 0 pokud se nepovede karty přesunout
     */
    int move(int from, int to, int idx);
    /*!
     * \brief Krok zpět
     * \return Pokud se nepovedlo 0
     */
    int undo();
    /*!
     * \brief Metoda slouží k určení, zda dojde k otočení balíčku.
     * \return Pokud se přetočí balíček true, jinak false
     */
    bool isDeckFlip(void);
    /*!
     * \brief Nápověda dalšího tahu.
     *
     * Po každém zavolání se cyklicky vrátí jeden možný tah.
     * \return
     */
    Move* getHint(void);
};

#endif // GAME_H
