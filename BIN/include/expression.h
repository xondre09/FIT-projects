#ifndef EXPRESSION_H_
#define EXPRESSION_H_

/** @file   gp.h
 *  @author Karel Ondřej <xondre09@stud.fit.vutbr.cz>
 *  @brief  Výraz.
 */

#include <vector>
#include <queue>

#include "function.h"

class Function;

class Expression {
public:
    std::vector<Expression*> childs;
    Function* function;

    Expression(Function* fce);
    ~Expression();

    truthTable_t interpret(context_t* context);
    std::vector<Expression*> allNodes();
    std::vector<Expression*> nodesInDeep(int n);
    int deep();
    Expression* copy();
    std::string toString();

};

#endif // EXPRESSION_H_
