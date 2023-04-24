#include "individual.h"

Individual::Individual(Expression* expression)
    : tree(expression)
    , fitness(0)
    , generation(0)
    , evaluate(0)
{

}

Individual::Individual(Expression* expression, double fitness)
    : tree(expression)
    , fitness(fitness)
    , generation(0)
    , evaluate(0)
{

}

Individual* Individual::copy() {
    return new Individual(this->tree->copy(), this->fitness);
}

std::string Individual::toString() {
    return this->tree->toString();
}

Individual::~Individual() {
    delete this->tree;
}
