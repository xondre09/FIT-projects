#include "function.h"

std::string Function::toString() {
    return this->name_;
}

int Function::arity() {
    return this->arity_;
}

bool Function::isOperator() {
    return this->isOperator_;
}

bool Function::isVariable() {
    return this->isVariable_;
}

Variable::Variable(int index)
    : index(index)
{
    name_ = "v" + std::to_string(index);
    arity_ = 0;
    isOperator_ = false;
    isVariable_ = true;
}

truthTable_t Variable::eval(Expression* expr, context_t* context) {
    return (*context)[this->index];
}
/**
 * AND
 */

And::And() {
    name_ = "and";
    arity_ = 2;
    isOperator_ = true;
    isVariable_ = false;
}

truthTable_t And::eval(Expression* expr, context_t* context) {
    truthTable_t left = expr->childs[0]->interpret(context);
    truthTable_t right = expr->childs[1]->interpret(context);
    truthTable_t ret(left.size());
    for (size_t i = 0; i < left.size(); ++i) {
        ret[i] = left[i] & right[i];
    }
    return ret;
}

/**
 * NAND
 */

Nand::Nand() {
    name_ = "nand";
    arity_ = 2;
    isOperator_ = true;
    isVariable_ = false;
}

truthTable_t Nand::eval(Expression* expr, context_t* context) {
    truthTable_t left = expr->childs[0]->interpret(context);
    truthTable_t right = expr->childs[1]->interpret(context);
    truthTable_t ret(left.size());
    for (size_t i = 0; i < left.size(); ++i) {
        ret[i] = ~(left[i] & right[i]);
    }
    return ret;
}

/**
 * OR
 */
Or::Or() {
    name_ = "or";
    arity_ = 2;
    isOperator_ = true;
    isVariable_ = false;
}

truthTable_t Or::eval(Expression* expr, context_t* context) {
    truthTable_t left = expr->childs[0]->interpret(context);
    truthTable_t right = expr->childs[1]->interpret(context);
    truthTable_t ret(left.size());
    for (unsigned int i = 0; i < left.size(); ++i) {
        ret[i] = left[i] | right[i];
    }
    return ret;
}

/**
 * NOR
 */
Nor::Nor() {
    name_ = "nor";
    arity_ = 2;
    isOperator_ = true;
    isVariable_ = false;
}

truthTable_t Nor::eval(Expression* expr, context_t* context) {
    truthTable_t left = expr->childs[0]->interpret(context);
    truthTable_t right = expr->childs[1]->interpret(context);
    truthTable_t ret(left.size());
    for (unsigned int i = 0; i < left.size(); ++i) {
        ret[i] = ~(left[i] | right[i]);
    }
    return ret;
}
/**
 * XOR
 */
Xor::Xor() {
    name_ = "xor";
    arity_ = 2;
    isOperator_ = true;
    isVariable_ = false;
}

truthTable_t Xor::eval(Expression* expr, context_t* context) {
    truthTable_t left = expr->childs[0]->interpret(context);
    truthTable_t right = expr->childs[1]->interpret(context);
    truthTable_t ret(left.size());
    for (unsigned int i = 0; i < left.size(); ++i) {
        ret[i] = left[i] ^ right[i];
    }
    return ret;
}

/**
 * Nxor
 */
Nxor::Nxor() {
    name_ = "nxor";
    arity_ = 2;
    isOperator_ = true;
    isVariable_ = false;
}

truthTable_t Nxor::eval(Expression* expr, context_t* context) {
    truthTable_t left = expr->childs[0]->interpret(context);
    truthTable_t right = expr->childs[1]->interpret(context);
    truthTable_t ret(left.size());
    for (unsigned int i = 0; i < left.size(); ++i) {
        ret[i] = ~(left[i] ^ right[i]);
    }
    return ret;
}

Functions::Functions(int varsCount) {
    this->operators.push_back(new And());
    this->operators.push_back(new Nand());
    this->operators.push_back(new Or());
    this->operators.push_back(new Nor());
    this->operators.push_back(new Xor());
    this->operators.push_back(new Nxor());

    for (int i = 0; i < varsCount; ++i) {
        this->variables.push_back(new Variable(i));
    }
}

Function* Functions::randomVariable() {
    int index = std::rand() % this->variables.size();
    return this->variables[index];
}

Function* Functions::randomOperation() {
    int index = std::rand() % this->operators.size();
    return this->operators[index];
}

Functions::~Functions() {
    for(auto a: this->operators) {
        delete a;
    }
    this->operators.clear();
    for(auto b: this->variables) {
        delete b;
    }
    this->variables.clear();
}
