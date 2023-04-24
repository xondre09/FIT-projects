from expression import *
from function import *
from Fitness import Fitness
from TournamentSelection import TournamentSelection
from Mutation import Mutation
from Crossover import Crossover

import copy
import random
import itertools
import sys

class Population(object):
    def __init__(self, tree=None):
        self.tree = tree
        self.fitness = 0

    def __lt__(self, other):
        return self.fitness < other.fitness

    def __str__(self):
        return "{}".format(str(self.tree))

    def __deepcopy__(self, memodict={}):
        p = Population()
        p.fitness = self.fitness
        p.tree = self.tree.__deepcopy__(memodict)
        return p


class GeneticPrograming(object):
    def __init__(self, nVars):
        self.nVars = nVars
        self.functions = Functions(nVars)

    def generate(self, n):
        if n == 0:
            return Expression(self.functions.randomVariable(), tuple())

        function = self.functions.randomFunction()
        childs = tuple(self.generate(n-1) for _ in range(function.arity()))

        return Expression(function, childs)

    def run(self, popSize = 100):
        population = [Population(self.generate(self.treeDeep)) for _ in range(popSize)]
        top = population[-1]
        itr = 0
        while itr < 5000/popSize:
            for p in population:
                self.fitness(p)

            self.max = max(population, key=lambda p: p.fitness)

            if top.fitness < self.max.fitness:
                top = self.max
                sys.stdout.write("\tGeneration {}, evaluate {}: best solution {}/{}\n".format(itr, (itr+1)*popSize, top.fitness, self.fitness.termFitness))

            if (itr+1)*popSize % 5000 == 0:
                sys.stdout.write("\tGeneration {}, evaluate {}: milestone\n".format(itr, (itr+1)*popSize))

            """
            if top.fitness >= self.fitness.termFitness:
                break
            """
            newGeneration = []
            for _  in range(0, popSize//2):
                parentA = self.select(population)
                parentB = self.select(population)

                childA = parentA.__deepcopy__()
                childB = parentB.__deepcopy__()

                self.cross(childA, childB)

                self.mutate(childA)
                self.mutate(childB)

                newGeneration.append(childA)
                newGeneration.append(childB)

            population = newGeneration
            itr+=1

        sys.stdout.write("\tResult: {}\n".format(top))
        return population

    def initialize(self):
        for op in (self.fitness, self.select, self.mutate, self.cross):
            op.initialize(self)

if __name__ == "__main__":
    g = GeneticPrograming(nVars = 10)
    g.fitness = Fitness()
    g.select = TournamentSelection(3)
    g.mutate = Mutation()
    g.cross = Crossover()

    g.initialize()

    for t in [4]: # 4, 6, 8, 10, 12
        g.treeDeep = t
        for pop in [50]:
            sys.stdout.write("Population: {}, Tree deep: {}\n".format(pop, t))
            for i in range(1):
                sys.stdout.write("Run {}/25:\n".format(i+1))
                r = g.run(pop)
