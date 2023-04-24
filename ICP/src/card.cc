/*!
 * \file    card.cc
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    7. 5. 2017
 * \brief   Implemetace třídy reprezentující údaje o kartě.
 */

#include "card.h"
#include <iostream>

Card::Card(Card &card)
    : value(card.value),
      color(card.color),
      visibility(card.visibility),
      x(card.x),
      y(card.y),
      moveable(card.moveable)
{
}

Card::Card(Solitaire::Value value, Solitaire::Color color, bool vis)
    : x(0),
      y(0),
      moveable(0)
{
    this->color=color;
    this->value=value;
    this->visibility = vis;
}

/*
 * Nastavení honoty karty.
 */
void Card::setValue(Solitaire::Value value)
{
    this->value = value;
}

/*
 * Vrací hodnotu karty.
 */
Solitaire::Value Card::getValue(void)
{
    return value;
}

/*
 * Nastavení barvy karty.
 */
void Card::setColor(Solitaire::Color color)
{
    this->color = color;
}

/*
 * Vrací barvu karty.
 */
Solitaire::Color Card::getColor(void)
{
    return color;
}

/*
 * Nastavení viditelnosti karty.
 */
void Card::setVisibility(bool f)
{
    this->visibility = f;
}

/*
 * Pokud je karta viditelná vrací true, jinak false.
 */
bool Card::isVisibility(void)
{
    return visibility;
}

/*
 * Nastavení pohyblivosti karty.
 */
void Card::setMoveable(bool f)
{
    this->moveable = f;
}

/*
 * Vrycí true, pokud se dá s kartou hýbat, jinak false.
 */
bool Card::isMoveable()
{
    return this->moveable;
}

/*
 * Nastaení pozice. Index hromádky je x a pozice na hromádce y.
 */
void Card::setPosition(int x_, int y_)
{
    x = x_;
    y = y_;
}

/*
 * Vrací index Hromádky.
 */
int Card::getCol()
{
    return x;
}

/*
 * Vrací pozici na hromádce.
 */
int Card::getRow()
{
    return y;
}

/*
 * Uložení karty do streamu.
 */
std::ostream & operator<<(std::ostream &s, Card &card)
{
    s <<  std::setw(2) << std::setfill('0') << card.value      << " ";
    s <<  std::setw(2) << std::setfill('0') << card.color      << " ";
    s <<  std::setw(2) << std::setfill('0') << card.visibility << " ";
    s <<  std::setw(2) << std::setfill('0') << card.x          << " ";
    s <<  std::setw(2) << std::setfill('0') << card.y          << " ";
    s <<  std::setw(2) << std::setfill('0') << card.moveable;

    return s;
}

/*
 * Načtení karty ze streamu.
 */
std::istream & operator>>(std::istream &s, Card &card)
{
    int tmp;
    s >> tmp;
    if(s.eof()){
        return s;
    }
    if(tmp < Solitaire::Ace || Solitaire::King < tmp) {           // Kontrola Hodnoty karet
        s.setstate(std::ios::failbit);
        return s;
    }
    card.value = Solitaire::Value(tmp);
    s >> tmp;
    if(tmp != Solitaire::Club  && tmp != Solitaire::Diamond &&
       tmp != Solitaire::Spade && tmp != Solitaire::Heart ) {     // Kontrola hodnot karet
        s.setstate(std::ios::failbit);
        return s;
    }
    card.color = Solitaire::Color(tmp);
    s >> tmp;
    if(tmp != 0 && tmp != 1) {           // Kontrola viditelnosti
        s.setstate(std::ios::failbit);
        return s;
    }
    card.visibility =tmp;
    s >> tmp;
    if(tmp < 0 || 11 < tmp) {           // index hromadky
        s.setstate(std::ios::failbit);
        return s;
    }
    card.x = tmp;
    s >> tmp;
    if(card.x != 0 && (tmp < 0 || 20 < tmp)) {   // pozice na hromadce (max 7 + 13 karet)
        s.setstate(std::ios::failbit);
        return s;
    }
    card.y = tmp;
    s >> tmp;
    if(tmp != 0 && tmp != 1) {          // kontrola pohyblivosti
        s.setstate(std::ios::failbit);
        return s;
    }
    card.moveable = tmp;

    return s;
}
