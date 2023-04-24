/*!
 * \file    solitairewindow.h
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    5. 5. 2017
 * \brief   Vykreslení plátna a menu.
 */

#ifndef SOLITAIREWINDOW_H
#define SOLITAIREWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QAction>
#include <QGraphicsView>

#include "solitairegui.h"

class SolitaireGui;

/*!
 * \brief Třída reprezentuje menu a plátno.
 */
class SolitaireWindow : public QWidget
{
    Q_OBJECT

private:
    Game *game;                 //!< Logika hry
    SolitaireGui *solitaire;    //!< Plátno se hrou
public:
    /*!
     * \brief Konstruktor třídy SolitaireWindow.
     * \param parent Rodič tídy
     */
    explicit SolitaireWindow(QWidget *parent = 0);
    ~SolitaireWindow();
};

#endif // SOLITAIREWINDOW_H
