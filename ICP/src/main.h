/*!
 * \file    main.h
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    7. 5. 2017
 * \brief   Vykreslení dlaždic.
 */

#ifndef MAIN_H
#define MAIN_H

#include "solitairewindow.h"
#include <QApplication>

/*!
 * \brief Vykreslení dlaždic.
 */
class Main : public QGridLayout {
    int games;  //!< Počet vykreslených dlaždic

public:
    Main();
    /*!
     * \brief Vykreslení počtu slaždic.
     * \param numberOfGames Požadovaný počet dlaždic.
     */
    void grid(int numberOfGames);
};

#endif // MAIN_H
