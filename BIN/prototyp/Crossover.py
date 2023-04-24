import random
import itertools

class Crossover(object):
    def __init__(self):
        self.gp = None
        self.operators = [self.uniformCrossover, self.sizeFairCrossover]

    def __call__(self, parentA, parentB):
        random.choice(self.operators)(parentA, parentB)

    def initialize(self, GP):
        self.gp = GP

    def uniformCrossover(self, parentA, parentB):
        nodes = [(parentA.tree, parentB.tree)]

        for nodeA, nodeB in nodes:
            if nodeA != None and nodeB != None:
                ran = random.random()
                if ran < 0.5:
                    nodeA.function, nodeB.function = nodeB.function, nodeA.function

                for t in itertools.zip_longest(nodeA.childs, nodeB.childs, fillvalue=None):
                    nodes.append(t)

    def sizeFairCrossover(self, parentA, parentB):
        n = random.randint(1, self.gp.treeDeep)

        nodesA = parentA.tree.nodesInDeep(n)
        nodesB = parentB.tree.nodesInDeep(n)

        choiceA = random.choice(nodesA)
        choiceB = random.choice(nodesB)

        choiceA.function, choiceB.function = choiceB.function, choiceA.function
        choiceA.childs, choiceB.childs = choiceB.childs, choiceA.childs
