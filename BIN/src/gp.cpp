/** @file   gp.cpp
 *  @author Karel Ondřej <xondre09@stud.fit.vutbr.cz>
 */

#include "gp.h"

GeneticPrograming::GeneticPrograming(int minDeep, int maxDeep, int nVariables)
    : minDeep(minDeep)
    , maxDeep(maxDeep)
    , varsCount(nVariables)
    , functions(nVariables)
    , context(nVariables)
{
    this->terminateFitness = std::pow(2, nVariables-1) - std::pow(2, nVariables/2-1);
    std::srand(std::time(nullptr));
    int combination = 1 << varsCount;
    size_t size = 8*sizeof(int);

    for (int idx = 0; idx < varsCount; ++idx) {
        this->context[idx].resize(combination / size + 1);
    }

    for (int i = 0; i < combination; ++i) {
        for (int j = 0; j < varsCount; ++j) {
            this->context[j][i / size] |= ((i >> j) & 1) << (i % size);
        }
    }
}

void GeneticPrograming::fitness(Individual* individum) {
    // vypocteni tabulky
    truthTable_t fit = individum->tree->interpret(&this->context);
    std::vector<int> ttb(1 << this->varsCount);

    size_t size = 8*sizeof(int);
    unsigned value;
    // prevod log. 0 na -1
    for (size_t i = 0; i < ttb.size(); ++i) {
        value = (fit[i / size] >> (i % size)) & 1;
        ttb[i] = value == 0 ? -1 : 1;
    }

    // Fast Walsh–Hadamard transform
    for (size_t h = 1; h < ttb.size(); h *= 2) {
        for (size_t i = 0; i < ttb.size(); i += h*2) {
            for (size_t j = i; j < i + h; ++j) {
                int x = ttb[j];
                int y = ttb[j+h];
                ttb[j] = x + y;
                ttb[j+h] = x - y;
            }
        }
    }

    // nalezeni maxima absolutnich hodnot v transformaci
    int max_ = 0;
    for (auto item: ttb) {
       max_ = std::max(max_, item < 0 ? -item : item);
    }

    // vypocet nelinearity
    individum->fitness = ((1 << this->varsCount) - max_) >> 1;
}

Individual* GeneticPrograming::select(std::vector<Individual*> &population) {
    int index = std::rand() % population.size();
    int ran;

    for (size_t c = 0; c < 3-1; ++c) {
        ran = std::rand() % population.size();
        if (population[ran]->fitness > population[index]->fitness) {
            index = ran;
        }
    }

    return population[index];
}

void GeneticPrograming::cross(Individual* parentA, Individual* parentB) {
    int n = std::rand() % ((std::min(parentA->tree->deep(), parentB->tree->deep())));
    n = std::max(1, n);

    std::vector<Expression*> nodesA = parentA->tree->nodesInDeep(n);
    std::vector<Expression*> nodesB = parentB->tree->nodesInDeep(n);

    int choiceA = std::rand() % nodesA.size();
    int choiceB = std::rand() % nodesB.size();

    std::swap(nodesA[choiceA]->function, nodesB[choiceB]->function);
    std::swap(nodesA[choiceA]->childs, nodesB[choiceB]->childs);
}

void GeneticPrograming::pointMutate(Individual* individual) {
    std::vector<Expression*> nodes = individual->tree->allNodes();
    int choice = std::rand() % nodes.size();
    if (nodes[choice]->function->isVariable()) {
        nodes[choice]->function = this->functions.randomVariable();
    }
    if (nodes[choice]->function->isOperator()) {
        nodes[choice]->function = this->functions.randomOperation();
    }
}

void GeneticPrograming::branchMutate(Individual* individual) {
    int deep = std::rand() % (individual->tree->deep());
    std::vector<Expression*> nodes = individual->tree->nodesInDeep(deep);

    int idx = std::rand() % nodes.size();

    for (auto c: nodes[idx]->childs) {
        delete c;
    }
    nodes[idx]->childs.clear();

    int ran = std::rand() % 2;

    // dosažena max. hloubka, nebo se náhodně vygeneruje pouze proměnná
    if (ran == 0 || deep == this->maxDeep) {
        nodes[idx]->function = this->functions.randomVariable();
    }
    else {
        nodes[idx]->function = this->functions.randomOperation();
    }

    for (int i = 0; i < nodes[idx]->function->arity(); ++i) {
        nodes[idx]->childs.push_back(this->generateGrowExpression(maxDeep-deep, std::max(0, minDeep - deep)));
    }
}

void GeneticPrograming::mutate(Individual* individual) {
    double ran = std::rand() / (RAND_MAX+1.);
    int mode;
    if (ran < 0.1) {
        mode = std::rand() % 2;
        if (mode == 0) {
            this->pointMutate(individual);
        }
        else {
            this->branchMutate(individual);
        }
    }
}

Expression* GeneticPrograming::generateFullExpression(int deep) {
    Function* function = deep == 1 ? this->functions.randomVariable() : this->functions.randomOperation();
    Expression* expr = new Expression(function);

    for (int i = 0; i < function->arity(); ++i) {
        expr->childs.push_back(this->generateFullExpression(deep-1));
    }

    return expr;
}

Expression* GeneticPrograming::generateGrowExpression(int maxDeep, int minDeep) {
    int ran = std::rand() % 2;
    Function* function;

    if (maxDeep <= 1 || (minDeep == 0 && ran == 0)) {
        function = this->functions.randomVariable();
    }
    else {
        function = this->functions.randomOperation();
    }

    Expression* expr = new Expression(function);

    for (int i = 0; i < function->arity(); ++i) {
        expr->childs.push_back(this->generateGrowExpression(maxDeep-1, std::max(0, minDeep-1)));
    }
    return expr;
}

void GeneticPrograming::generatePopulation(std::vector<Individual*> &population, int popSize) {
    Individual* individum;
    Expression* expr;
    int ran;
    for (int idx = 0; idx < popSize; ++idx) {
        ran = std::rand() % 2;
        if (ran == 0) {
            expr = this->generateFullExpression(this->maxDeep);
        }
        else {
            expr = this->generateGrowExpression(this->maxDeep, this->minDeep);
        }

        individum = new Individual(expr);

        population[idx] = individum;
    }
}

/** Běh algoritmu genetického programování.
 * 
 *  @param popSize[in]           velikost populace
 *  @param terminateEvaluate[in] maximální počet evaluací
 */
Individual* GeneticPrograming::run(int popSize, int terminateEvaluate) {
    std::vector<Individual*> population(popSize);
    std::vector<Individual*> newGeneration(popSize);

    // generování populace
    this->generatePopulation(population, popSize);

    Individual *parentA, *parentB;
    Individual *childA, *childB;
    Individual *actualMax;

    Individual* top = population[0]->copy();
    top->generation = 0;
    top->evaluate = 0;

    int iteration = 0;

    while (iteration < (terminateEvaluate / popSize)) {
        // ohodnocení populace
        for(auto p: population) {
            this->fitness(p);
        }
        // nalezení nejlepšího jedince v generaci
        actualMax = population[0];
        for (auto p: population) {
            if (p->fitness > actualMax->fitness) {
                actualMax = p;
            }
        }

        // aktualizace nejlepšího doposud nalezeného jedince
        if (actualMax->fitness > top->fitness) {
            delete top;
            top = actualMax->copy();
            top->fitness = actualMax->fitness;
            top->generation = iteration;
            top->evaluate = (iteration+1) * popSize;

            if (top->fitness >= this->terminateFitness) {
                break;
            }
        }

        newGeneration.clear();

        // generování nové populace
        for (int i = 0; i < popSize / 2; ++i) {
            // výběr rodičů
            parentA = this->select(population);
            parentB = this->select(population);
            // vytvoření kopií rodičů
            childA = parentA->copy();
            childB = parentB->copy();
            // křížení
            this->cross(childA, childB);
            // mutace
            this->mutate(childA);
            this->mutate(childB);

            newGeneration.push_back(childA);
            newGeneration.push_back(childB);
        }

        for (auto p: population) {
            delete p;
        }

        population = newGeneration;
        iteration++;
    }

    for (auto p: population) {
        delete p;
    }

    return top;
}

/** Spuštění algoritmu GP. Parametry algoritmu lze zadat jako argumenty 
 *  programu. Výstup evoluce je ve formátu XML.
 */
int main(int argc, char** argv) {
    if (argc != 6) {
        std::cerr << "./gp deep.min deep.max population.size run terminate.evaluate" << std::endl;
        return 1;
    }
    
    int deep_min = std::atoi(argv[1])+1;
    int deep_max = std::atoi(argv[2])+1;
    int population_size = std::atoi(argv[3]);
    int run = std::atoi(argv[4]);
    int terminateEvaluate = std::atoi(argv[5]);
    
    GeneticPrograming app(deep_min, deep_max, 10);

    std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    std::cout << "<project>" << std::endl;
    std::cout << "  <batch population.size=\"" << population_size
              << "\" terminate.evaluate=\"" << terminateEvaluate
              << "\" terminate.fitness=\"" << app.terminateFitness
              << "\" deep.max=\"" << app.maxDeep-1
              << "\" deep.min=\"" << app.minDeep-1
              << "\">" << std::endl;
    
    for (int r = 0; r < run; ++r) {
        std::cout << "    <run number=\"" << r+1 << "\">" << std::endl;
        Individual* top = app.run(population_size, terminateEvaluate);

        std::cout << "      <individual "
                << "fitness=\"" << top->fitness << "\" "
                << "generation=\"" << top->generation << "\" "
                << "evaluate=\"" << top->evaluate << "\">" << std::endl;
        std::cout << "        <tree "
                << "size=\"" << top->tree->allNodes().size() << "\" "
                << "deep=\"" << top->tree->deep()-1 << "\" "
                << "notation=\"postfix\">"
                << top->tree->toString() 
                << "</tree>" << std::endl;
        std::cout << "      </individual>" << std::endl;
        std::cout << "    </run>" << std::endl;
    }

    std::cout << "  </batch>" << std::endl;
    std::cout << "</project>" << std::endl;

    return 0;
}
