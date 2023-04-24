/*!
 * \file    solitairewindow.cc
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    6. 5. 2017
 * \brief   Vykreslení plátna a menu.
 */


#include "solitairewindow.h"
#include <iostream>

SolitaireWindow::SolitaireWindow(QWidget *parent) : QWidget(parent)
{
    game = new Game();

    solitaire = new SolitaireGui(game, this);

    QVBoxLayout  *myLayout = new QVBoxLayout(this);
    this->setLayout(myLayout);

    QGraphicsView * mcpGraphicsView1 = new QGraphicsView(solitaire);
    mcpGraphicsView1->setSceneRect(0, 0, 6 * COLUMN_PADDING + CARD_W + 2*COLUMN_OFFSET_X, 2*DECK_OFFSET_Y + COLUMN_OFFSET_Y + 19*ROW_PADDING + CARD_H);
    myLayout->addWidget(mcpGraphicsView1);

    QMenuBar *bar = new QMenuBar(this);
    myLayout->insertWidget( 0, bar );

    QAction* actionNewGame  = bar->addAction("New Game");
    QAction* actionQuitGame = bar->addAction("Quit Game");
    QAction* actionUndo     = bar->addAction("Undo");
    QAction* actionHelp     = bar->addAction("Help");
    QAction* actionSave     = bar->addAction("Save");
    QAction* actionLoad     = bar->addAction("Load");

    solitaire->connect(actionNewGame , &QAction::triggered, solitaire, &SolitaireGui::newGame);
    solitaire->connect(actionQuitGame, &QAction::triggered, solitaire, &SolitaireGui::quitGame);
    solitaire->connect(actionUndo    , &QAction::triggered, solitaire, &SolitaireGui::undo);
    solitaire->connect(actionSave    , &QAction::triggered, solitaire, &SolitaireGui::save);
    solitaire->connect(actionLoad    , &QAction::triggered, solitaire, &SolitaireGui::load);
    solitaire->connect(actionHelp    , &QAction::triggered, solitaire, &SolitaireGui::hint);
}

SolitaireWindow::~SolitaireWindow() {
    delete game;
}
