/*!
 * \file    hint.h
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    4. 5. 2017
 * \brief   Nápověda ke hře Solitaire Klondika.
 */

#ifndef HINT_H
#define HINT_H

#include <vector>
#include "game.h"

class Game;

/*!
 * \brief Třída generující nápovědu pro aktuální tah.
 */
class Hint {
    std::vector<Move *> hints;  //! Vektor všech možných tahů
    int idx;                    //! Index naposledy ukázaného tahu

public:
    /*!
     * \brief Konstruktor třídy Hint.
     */
    Hint(void);
    ~Hint(void);
    /*!
     * \brief Vymaže možné tahy.
     */
    void clear(void);
    /*!
     * \brief Vygeneruje pro danou hru množinu tahů.
     *
     * Je vynechána nápověda např. pro tahy z finální hromádky, nebo mezi dvěma herními hromádkami.
     * \param game Hra pro kterou je generována nápověda
     * \return Pokud existuje nějaký tah (kromě otočení balíčku) vrací 1.
     */
    int generateHints(Game *game);
    /*!
     * \brief Cyklicky vrací jeden tah z množiny možných tahů.
     * \return Možný tah
     */
    Move* getHint(void);
};

#endif // HINT_H
