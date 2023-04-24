/*!
 * \file    solitairegui.h
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    5. 5. 2017
 * \brief   Plátno na které se vykreslují karty.
 */

#ifndef SOLITAIREGUI_H
#define SOLITAIREGUI_H

#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QFileDialog>

#include "game.h"
#include "cardgui.h"
#include "solitairewindow.h"

#define SCORE_SIZE 15       //!< Velikost písma skóre v piselech
#define VICTORY_SIZE 50     //!< Velikost písma vítězství v pixelech

class SolitaireWindow;
class CardGui;

/*!
 * \brief Třída reprezentuje plátno hry.
 */
class SolitaireGui : public QGraphicsScene {

    Q_OBJECT

private:
    Game              *game;        //!< Logika hry
    QGraphicsView     *view;
    CardGui           *active;      //!< Aktivni karta
    QGraphicsTextItem *victory;     //!< Text konce hry
    QGraphicsTextItem *score;       //!< Text skóre

    QPointF offset;                 //!< Offset při posunu karty
    CardGui *flipDeck;              //!< Karta otáčení balíčku
    QList<CardGui *> cards;         //!< Hrací karty
    QList<CardGui *> backCards;     //!< Karty na pozadí
    QGraphicsRectItem * hints[2];   //!< Nápovědaa

public:
    /*!
     * \brief Konstruktor třídy SolitaireGui.
     * \param game      Logika hry
     * \param parent    Rodičovský objekt.
     */
    SolitaireGui(Game* game, SolitaireWindow *parent = 0);
    ~SolitaireGui();
    /*!
     * \brief Vytvoření nové hry.
     */
    void newGame(void);
    /*!
     * \brief Tah zpět.
     */
    void undo(void);
    /*!
     * \brief Nápověda tahu.
     */
    void hint(void);
    /*!
     * \brief Ukončení hry. Všechny hromádky jsou smazány.
     */
    void quitGame(void);
    /*!
     * \brief Uložení hry.
     *
     * Otevře se dialogové okno pro uložení hry.
     */
    void save(void);
    /*!
     * \brief Načtení hry.
     *
     * Otevře se dialogové okno pro načtení hry.
     */
    void load(void);

private:
    /*!
     * \brief Vykreslení nápovědy na plátno.
     * \param x Index hromádky.
     * \param y Pozice karty na hromádce
     * \param n Počet karet položených nad danou kartou
     * \return QGraphicsRectItem reprezentující nápovědu.
     */
    QGraphicsRectItem *addHint(int x, int y, int n);
    /*!
     * \brief Vykreslení karet na plátno podle logiky hry game.
     */
    void draw(void);
    /*!
     * \brief Vykreslení ukazatele skóre.
     */
    void drawScore(void);
    /*!
     * \brief Vykreslení ukončení hry.
     */
    void drawVictory(void);
    /*!
     * \brief Vyčištění plátna od karet.
     */
    void clear(void);

protected:
    /*!
     * \brief Po kliknutí se určí s kterou kartou se bude pohybovat.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    /*!
     * \brief Pohyb s kartou a zbytkem hromádky.
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    /*!
     * \brief Po uvolnění tlačítka se zkusí přesunout kartu na hromádku a překreslí se plátno.
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    /*!
     * \brief Zachytávání dvojkliku.
     *
     * V případě dvojkliku se aplikace pokusí přesunout kartu na finální balíček.
     */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
    /*!
     * \brief Zachytávání stisknutí klávesnice.
     *
     * N -> Nová hra
     * Q -> Konec hry
     * L -> Načtení hry
     * S -> Uložení hry
     * U -> Krok zpět
     * H -> Nápověda
     */
    void keyPressEvent(QKeyEvent* event);

};

#endif // SOLITAIREGUI_H
