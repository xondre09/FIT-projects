import random
from function import Variable, Operation

class Mutation(object):
    def __init__(self, ration=0.1):
        self.gp = None
        self.ration = ration
        self.operators = (self.simpleMutation,
                          self.branchMutation)

    def __call__(self, individual):
        if random.random() < self.ration:
            random.choice(self.operators)(individual)

    def initialize(self, GP):
        self.gp = GP

    def simpleMutation(self, individual):
        nodes = individual.tree.allNodes()
        choice = random.choice(nodes)

        if isinstance(choice.function, Variable):
            choice.function = self.gp.functions.randomVariable()

        if isinstance(choice.function, Operation):
            choice.function = self.gp.functions.randomFunction()

    def branchMutation(self, individual):
        nodes = individual.tree.allNodes()
        choices = [random.choice(nodes)]

        for node in choices:
            if isinstance(node.function, Variable):
                node.function = self.gp.functions.randomVariable()

            if isinstance(node.function, Operation):
                node.function = self.gp.functions.randomFunction()
                for c in node.childs:
                    choices.append(c)
