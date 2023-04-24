import random

class TournamentSelection:
    def __init__(self, size=3):
        self.gp = None
        self.size = size

    def __call__(self, population):
        individuals = [random.choice(population) for _ in range(self.size)]
        return max(individuals, key=lambda i: i.fitness)

    def initialize(self, GP):
        self.gp = GP

class Selection:
    def __call__(self, population):
        max_ = self.max.fitness
        while True:
            index = int(random.random() * len(population))
            if random.random() < population[index].fitness / max_:
                return population[index]
