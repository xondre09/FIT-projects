import itertools

class Fitness:

    def initialize(self, GP):
        self.gp = GP
        n = self.gp.nVars
        self.combination = 1 << n
        self.context = [0 for _ in range(n)]
        self.termFitness = ((1 << n) - (1 << n//2)) >> 1

        for comb in itertools.product([0,1], repeat=n):
            for idx, val in enumerate(comb):
                self.context[idx] = (self.context[idx] << 1) | (val & 1)

    def getTTL(self, x):
        ret = []
        for _ in range(1 << self.gp.nVars):
            val = -1 if x & 1 == 0 else 1
            x >>= 1
            ret.append(val)
        return ret

    def fwht(self, a):
       h = 1
       while h < len(a):
          for i in range(0, len(a), h * 2):
             for j in range(i, i + h):
                x = a[j]
                y = a[j+h]
                a[j] = x + y
                a[j+h] = x - y
          h *= 2

    def __call__(self, individuum):
        assert individuum.tree != None
        h = individuum.tree.interpret(self.context)
        ttf = self.getTTL(h)
        self.fwht(ttf)
        m = max(map(abs, ttf))
        individuum.fitness = (self.combination - m) >> 1
        return individuum
