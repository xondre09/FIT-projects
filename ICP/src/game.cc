/*!
 * \file    game.cc
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    20. 4. 2017
 * \brief   Logika karetní hry Solitaire Klondike.
 */

#include "game.h"
#include <iostream>

Game::Game()
    : topDeck(0),
      count(1),
      score(0)
{
    hints = new Hint();
}

Game::~Game()
{
    this->clear();
    delete hints;
}

/*
 * Vyčištění hry.
 */
void Game::clear(void)
{
    this->score = 0;
    for(int i = 0; i < 12; i++)
    {
        while(!desk[i].empty())
        {
            delete desk[i][0];
            desk[i].erase(desk[i].begin());
        }
    }
    this->generateHints();
}

/*
 * Zamíchání karet.
 */
void Game::staffleCards(void)
{

    this->clear();

    std::vector<Card *> tmp;
    // Vygenerování balíčku
    for(int color = Solitaire::Club; color <= Solitaire::Heart; color++)
    {
        for(int num = Solitaire::Ace; num <= Solitaire::King; num++)
        {
            Card *c = new Card(Solitaire::Value(num), Solitaire::Color(color), false);
            tmp.push_back(c);
        }
    }
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    // Zamíchání a rozdání na hrací hromádky
    for(int i = 1; i < 8; i++)
    {
        for(int n = 0; n < i; n++)
        {
            int r = rand() % (int)(tmp.size());

            tmp[r]->setPosition(i, n);
            desk[i].push_back( tmp[r] );
            tmp.erase(tmp.begin() + r);
        }
        this->flipCard(i, true);
    }
    // Zamíchání zbytku karet
    while(!tmp.empty())
    {
        int r = rand() % (int)(tmp.size());
        tmp[r]->setPosition(0, desk[0].size());
        desk[0].push_back( tmp[r] );
        tmp.erase(tmp.begin() + r);
    }
    score = 0;
    topDeck = desk[0].size();
    this->generateHints();
}

/*
 * Otočení vrchní karty.
 */
int Game::flipCard(int col, bool f)
{
    // nejedná se o hrací hromádku nebo je prázdná
    if( !(1 <= col && col < 8) || desk[col].empty())
    {
        return 0;
    }
    // Karta je už otočená
    if(desk[col][desk[col].size()-1]->isVisibility() == f)
    {
        return 0;
    }
    // Přičtení
    if(f == true)
    {
        score += 5;
    }
    else
    {
        score -= 5;
    }

    desk[col][desk[col].size()-1]->setVisibility(f);
    desk[col][desk[col].size()-1]->setMoveable(f);

    return 1;
}

/*
 * Indexace karet, kde i je index krmádky.
 */
std::vector<Card *>& Game::operator[](int i)
{
    return desk[i];
}

/*
 * Líznutí z hromádky
 */
void Game::rollDeck()
{

    if(!desk[0].empty())
    {
        Undo *u = new Undo(3, topDeck + count, 0, 0, score);
        undo_v.push_back(u);
        if(topDeck == 0)
        {
            score = score - 100;
            score = score < 0 ? 0 : score;
            topDeck = desk[0].size();
            for(unsigned i = 0; i < desk[0].size(); ++i)
            {
                desk[0][i]->setVisibility(false);
                desk[0][i]->setMoveable(false);
            }
        }
        else {
            this->rollDeckUtil();
        }
        this->generateHints();
    }
}

/*
 * Pomocná funkce pro líznutí z balíčku.
 */
void Game::rollDeckUtil()
{
    unsigned int newTop = count > topDeck ? 0 : topDeck - count;

    for(unsigned int i = newTop; i < desk[0].size(); i++)
    {
        desk[0][i]->setVisibility(true);
        desk[0][i]->setMoveable(false);
    }
    if(newTop < desk[0].size())
    {
        desk[0][newTop]->setMoveable(true);
    }
    topDeck = newTop;
}

/*
 * Kontrola správnosti přesunu.
 */
int Game::isValidMove(int from, int to, int idx)
{

    if(from == to )    // pokud se jedna o presuz ze stejne hromadky
    {
        return 0;
    }

    if(!(0 <= from && from < 12) || !(0 < to && to < 12) || !( 0 <= idx && idx < (signed)desk[from].size()))
    {
        return 0;
    }
    // nelze s karou pohnout nebo není otočená
    if(desk[from][idx]->isMoveable() == false || desk[from][idx]->isVisibility() == false)
    {
        return 0;
    }
    // cílová hromádka je prázdná
    if(desk[to].empty())
    {
        if(to < 8 && desk[from][idx]->getValue() != 12)
        {
            return 0;
        }
        else if(8 <= to && desk[from][idx]->getValue() != 0)
        {
            return 0;
        }
    }
    else if(to < 8) // jedná se o balíček nebo hrací hromádku
    {
        if(desk[from][idx]->getValue() + 1 != desk[to][desk[to].size()-1]->getValue())  // na cílové hromádce musí být větší karta o 1
        {
            return 0;
        }
        if(desk[from][idx]->getColor() / 2 == desk[to][desk[to].size()-1]->getColor() / 2) // na cílové hromádce musí být jiná barva
        {
            return 0;
        }
    }
    else
    {
        if(from != 0 && idx != (signed)desk[from].size() -1 )   // kontrola indexu na balíčku
        {
            return 0;
        }
        if(desk[from][idx]->getValue() - 1 != desk[to][desk[to].size()-1]->getValue())
        {
            return 0;
        }
        if(desk[from][idx]->getColor() != desk[to][desk[to].size()-1]->getColor())
        {
            return 0;
        }
    }
    return 1;
}

/*
 * Vrací aktuální skóre.
 */
int Game::getScore(void)
{
    return score;
}

/*
 * Konec hry.
 */
bool Game::isEnd(void)
{
    if(desk[8].size() == 13 && desk[9].size() == 13 && desk[10].size() == 13 && desk[11].size() == 13) {
        return 1;
    }
    return 0;
}

/*
 * Generuje další tah.
 */
void Game::generateHints(void)
{
    hints->generateHints(this);
}

/*
 * Vrací jeden z možných tahů.
 */
Move* Game::getHint(void)
{
    return this->hints->getHint();
}

/*
 * Přesun z hromádky na jednu z finálních.
 */
int Game::move(int from)
{

    for(int to = 8; to < 12; ++to)
    {
        int err = this->move(from, to);
        if(err)
        {
            return err;
        }
    }

    return 0;
}

/*
 * Přesun z jedné hromádky na druhou vrchní kartu.
 */
int Game::move(int from, int to)
{
    if(from == 0)
    {
        return this->move(from, to, topDeck);
    }
    else
    {
        return this->move(from, to, desk[from].size()-1);
    }

    return 0;
}

/*
 * Přesun z jedné hromádky na druhou všechny karty od pozice idx.
 */
int Game::move(int from, int to, int idx)
{

    if(!this->isValidMove(from, to, idx)) // nejedná se o možný tah
    {
        return 0;
    }

    int end;
    int op = 0;
    int sc = this->score;

    if(from == 0 && to < 8)     // z balíčku na herní hromádku
    {
        score += 5;
    }
    else if(from == 0 && 8 <= to && to < 13)    // z balíčku do finální hromádky
    {
        score += 10;
    }
    else if( 0 < from && from < 8 && 8 <= to && to < 13) //z herní hromádky do finální hromádky
    {
        score += 10;
    }
    else if(8 <= from && from < 13 && to < 8) // z finální na herní
    {
        score -= 15;
    }

    if(from == 0)
    {
        desk[from][idx]->setPosition(to, desk[to].size());
        desk[to].push_back( desk[from][idx]);
        desk[from].erase(desk[from].begin() + idx);
        desk[0][topDeck]->setMoveable(true);
        for(unsigned int i = 0; i < desk[from].size(); ++i)
        {
            desk[from][i]->setPosition(from, i);
        }
        end = idx;
    }
    else
    {
        end = desk[to].size();
        while(idx < (signed)desk[from].size())
        {
            desk[from][idx]->setPosition(to, desk[to].size());
            desk[to].push_back( desk[from][idx]);
            desk[from].erase(desk[from].begin() + idx);
        }
        op = 1 + this->flipCard(from, true);
    }
    // tah z5
    Undo *u = new Undo(op, from, to, end, sc);
    undo_v.push_back(u);

    this->generateHints();
    return 1;
}

/*
 * Vrací, zda se překlopí balíček
 */
bool Game::isDeckFlip(void)
{
    return desk[0].size() > count && topDeck == 0 ? true : false;
}

/*
 * Tak zpět.
 */
int Game::undo()
{

    if(undo_v.empty())
    {
        return 0;
    }

    Undo *u = undo_v[undo_v.size()-1];
    undo_v.pop_back();

    int op   = u->getOperation();
    int from = u->getTo();
    int to   = u->getFrom();
    int idx  = u->getIdx();

    if(op == 0)
    {
        if((unsigned)idx < desk[0].size())
        {
            desk[0][idx]->setMoveable(false);
            desk[0].insert(desk[0].begin() + idx, desk[from][desk[from].size()-1]);
        }
        else
        {
            desk[0].push_back(desk[from][desk[from].size()-1]);
        }
        desk[from].pop_back();
        for(unsigned i = 0; i < desk[0].size(); i ++)
        {
            desk[0][i]->setPosition(0, i);
        }

    }
    else if(op == 1 || op == 2)
    {
        if(op == 2)
        {
            this->flipCard(to, false);
        }
        while(idx < (signed)desk[from].size())
        {
            desk[from][idx]->setPosition(to, desk[to].size());
            desk[to].push_back( desk[from][idx]);
            desk[from].erase(desk[from].begin() + idx);
        }
    }
    else if(op == 3)
    {
        for(unsigned int i = 0; i < count; i++)
        {
            if(topDeck + i < desk[0].size())
            {
                desk[0][topDeck + i]->setVisibility(false);
                desk[0][topDeck + i]->setPosition(0, topDeck + i);
                desk[0][topDeck + i]->setMoveable(false);
            }
        }
        topDeck = to;
        this->rollDeckUtil();
    }

    score = u->getScore();
    delete u;

    this->generateHints();
    return 1;
}

/*
 * Uložení stavu hry do streamu.
 */
std::ostream & operator<<(std::ostream & out, Game &game)
{
    out << std::setw(2) << std::setfill('0') << game.topDeck << " ";
    out << std::setw(2) << std::setfill('0') << game.count   << " ";
    out << std::setw(2) << std::setfill('0') << game.score   << std::endl;
    for(int i = 0; i < 12; i++)
    {
        for(auto &c : game.desk[i])
        {
            out << *c << std::endl;
        }
    }
    return out;
}

/*
 * Načtení stavu hry ze streamu.
 */
std::istream & operator>>(std::istream & out, Game& game)
{
    game.clear();
    out >> game.topDeck >> game.count >> game.score;
    while(out.good())
    {
        Card *card = new Card(Solitaire::Ace, Solitaire::Club, false);
        out >> *card;
        if(out.eof())   // konec streamu
        {
            delete card;
            break;
        }
        game.desk[card->getCol()].push_back(card);
    }
    if(!out.eof() && out.fail())
    {
        game.clear();
    }
    game.generateHints();
    return out;
}
