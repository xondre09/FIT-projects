/*!
 * \file    hint.cc
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    4. 5. 2017
 * \brief   Nápověda ke hře Solitaire Klondika.
 */

#include "hint.h"

Hint::Hint()
    : hints()
{

}

Hint::~Hint()
{
    this->clear();
}

/*
 * Vyčištěné tahů.
 */
void Hint::clear(void)
{

    for(auto &move : this->hints)
    {
        delete move;
    }
    hints.clear();
}

/*
 * Vygenerování tahů.
 */
int Hint::generateHints(Game *game) {
    Move *m;
    int ret = true;
    this->idx = -1;
    this->clear();

    for(int col_from = 0; col_from < 8; ++col_from) // z baličku nebo z herní hromádky
    {
        for(int col_to = 8; col_to < 12; ++col_to)  // do finální hromádky
        {
            for(int idx = 0; idx < (signed)(*game)[col_from].size(); ++idx) // kvuli balicku
            {
                if(game->isValidMove(col_from, col_to, idx) == true)
                {
                    m = new Move(col_from, col_to, idx);
                    hints.push_back(m);
                }
            }
        }
    }

    for(int col_from = 0; col_from < 8; ++col_from) // z balíčku nebo herní hromátky
    {
        for(int col_to = 1; col_to < 8; ++col_to)   // na herní hromádku
        {
            for(int idx = 0; idx < (signed)(*game)[col_from].size(); ++idx) // co nejvíc karet
            {
                if(game->isValidMove(col_from, col_to, idx) == true)
                {
                    // nadbytečné tahy
                    if(col_from == 0 || (idx == 0 && (*game)[col_from][idx]->getValue() != 12) || (idx != 0 && (*game)[col_from][idx-1]->isVisibility() == false))
                    {
                        m = new Move(col_from, col_to, idx);
                        hints.push_back(m);
                    }
                }
            }
        }
    }

    if(this->hints.empty())
    {
        ret = false;
    }

    if(!(*game)[0].empty())
    {
        m = new Move(0, 0, -1);
        hints.push_back(m);
    }
    return ret;
}

/*
 * Vrátí jeden tah. Cyklicky opakuje všechny tahy.
 */
Move* Hint::getHint(void)
{
    if(!this->hints.empty())
    {
        if(++idx >= (signed)this->hints.size())
        {
            idx = 0;
        }
        return this->hints[idx];
    }
    return NULL;
}
