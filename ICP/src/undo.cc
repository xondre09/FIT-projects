/*!
 * \file    undo.cc
 * \authors Karel ONDŘEJ, FIT
 * \authors David SMÉKAL, FIT
 * \date    5. 5. 2017
 * \brief   Třídy reprezentující jeden tah a tah zpět.
 */

#include "undo.h"

Move::Move(int from_, int to_, int idx_)
    : from(from_),
      to(to_),
      idx(idx_)
{
}

int Move::getFrom(void)
{
    return from;
}

int Move::getTo(void)
{
    return to;
}

int Move::getIdx(void)
{
    return idx;
}

Undo::Undo(int op_, int from_, int to_, int idx_, int score_)
    : Move(from_, to_, idx_),
      op(op_),
      score(score_)
{
}

int Undo::getOperation(void)
{
    return op;
}

int Undo::getScore(void)
{
    return score;
}
