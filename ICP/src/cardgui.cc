/*!
 * \file    cardgui.cc
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    5. 5. 2017
 * \brief   Grafické reprezentace karty.
 */

#include "cardgui.h"
#include <iostream>
/*
 * Přidání vzhledu karty
 */
CardGui::CardGui(QIcon icon, SolitaireGui *parent)
    : QGraphicsPixmapItem(),
      Card(Solitaire::Ace, Solitaire::Club, false)
{
    parent->addItem(this);
    this->setPixmap(icon.pixmap(QSize(CARD_H, CARD_H)));
}

/*
 * Inicializace vzhledu karzy z třídy Card.
 */
CardGui::CardGui( Card *card, SolitaireGui *parent)
    : QGraphicsPixmapItem(),
      Card(*card)
{
    QString icon;\

    if(this->isVisibility() == false)       // zadní karta
    {
        icon = Solitaire::BACK_SIDE;
    }
    else    // ostatní karty
    {
        icon = Solitaire::CARDS[this->getValue()][this->getColor()];
    }

    parent->addItem(this);
    this->setPixmap(QPixmap(icon).scaledToHeight(CARD_H));
}

/*
 * Vykreslení karty.
 */
void CardGui::draw() {

    int x = this->getCol();
    int y = this->getRow();


    this->setZValue(y * 1);
    if(x == 0)
    {
        if(this->isVisibility())    //nabídka
        {
            this->setPos(DECK_OFFSET_X + COLUMN_PADDING, DECK_OFFSET_Y);
            this->setZValue((20 - y));
        }
        else    // balíček
        {
    		this->setPos(DECK_OFFSET_X, DECK_OFFSET_Y);
    	}
    }
    else if(1 <= x && x < 8)    // hrací hromádky
    {
        this->setPos(COLUMN_OFFSET_X + (x-1) * COLUMN_PADDING, COLUMN_OFFSET_Y + y * ROW_PADDING);
    }
    else if (8 <= x && x < 12)  // ukončovací hromádky
    {
        this->setPos(STORE_OFFSET_X  + (x-8) * COLUMN_PADDING, STORE_OFFSET_Y);
    }
}
