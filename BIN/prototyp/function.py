import random

class Variable(object):
    def __init__(self, index):
        self.index = index

    def __call__(self, expr, context):
        return context[self.index]

    def arity(self):
        return 0

    def __str__(self):
        return "x{}".format(self.index)

class Operation(object):
    pass

class And(Operation):
    def __call__(self, expr, context):
        left  = expr.childs[0].interpret(context)
        right = expr.childs[1].interpret(context)
        return left & right

    def arity(self):
        return 2

    def __str__(self):
        return "And"

class Nand(Operation):
    def __call__(self, expr, context):
        left  = expr.childs[0].interpret(context)
        right = expr.childs[1].interpret(context)
        return ~(left & right)

    def arity(self):
        return 2

    def __str__(self):
        return "Nand"

class Or(Operation):
    def __call__(self, expr, context):
        left  = expr.childs[0].interpret(context)
        right = expr.childs[1].interpret(context)
        return left | right

    def arity(self):
        return 2

    def __str__(self):
        return "Or"

class Nor(Operation):
    def __call__(self, expr, context):
        left  = expr.childs[0].interpret(context)
        right = expr.childs[1].interpret(context)
        return ~(left | right)

    def arity(self):
        return 2

    def __str__(self):
        return "Or"

class Xor(Operation):
    def __call__(self, expr, context):
        left  = expr.childs[0].interpret(context)
        right = expr.childs[1].interpret(context)
        return left ^ right

    def arity(self):
        return 2

    def __str__(self):
        return "Xor"

class Xnor(Operation):
    def __call__(self, expr, context):
        left  = expr.childs[0].interpret(context)
        right = expr.childs[1].interpret(context)
        return ~(left ^ right)

    def arity(self):
        return 2

    def __str__(self):
        return "Xnor"

class Functions(object):
    def __init__(self, variables=10):
        self.variables = tuple(Variable(i) for i in range(variables))
        self.function  = [And(), Nand(), Or(), Nor(), Xor(), Xnor()]

    def randomFunction(self):
        return random.choice(self.function)

    def randomVariable(self):
        return random.choice(self.variables)
