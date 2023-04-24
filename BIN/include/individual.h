#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

/** @file   individual.h
 *  @author Karel Ondřej <xondre09@stud.fit.vutbr.cz>
 *  @brief  Jedinec v populaci.
 */


#include "expression.h"
#include "function.h"

class Individual {
public:
    Expression* tree;
    double fitness;
    int generation;
    int evaluate;

    Individual(Expression* expression);
    Individual(Expression* expression, double fitness);
    Individual* copy();
    std::string toString();
    ~Individual();
};

#endif // INDIVIDUAL_H
