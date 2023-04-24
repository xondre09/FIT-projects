class Expression(object):
    def __init__(self, function, childs):
        self.childs = childs
        self.function = function

    def interpret(self, context):
        return self.function(self, context)

    def allNodes(self):
        nodes = [self]

        for i in nodes:
            for child in i.childs:
                nodes.append(child)

        return nodes

    def nodesInDeep(self, n):
        tmp = [(n, self)]
        nodes = []

        for n, i in tmp:
            if n > 1:
                for child in i.childs:
                    tmp.append((n-1, child))
            else:
                nodes.append(i)

        return nodes

    def deep(self):
        return max((i.deep() for i in self.childs), default=0) + 1

    def __str__(self):
        return "{}({})".format(str(self.function), ", ".join(map(str, self.childs)))

    def __deepcopy__(self, memodict={}):
        children = tuple(c.__deepcopy__(memodict) for c in self.childs)
        return Expression(self.function, children)
