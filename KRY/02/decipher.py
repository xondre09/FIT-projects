#!/usr/bin/env python3
import argparse
import re

N_B = 64
bitlength = N_B * 8

class GF(object):
    """Trida reprezentujici cislo ve zbytkove tride Z_Fp."""
    def __init__(self, p):
        self.p = p

    def number(self, n):
        return GF.Number(n % self.p, self)

    class Number(object):
        def __init__(self, n, gf):
            assert(isinstance(n, int))
            self.value = n
            self.gf = gf

        def __add__(self, other):
            assert(isinstance(other, GF.Number))
            return self.gf.number(self.value + other.value)

        def __sub__(self, other):
            assert(isinstance(other, GF.Number))
            return self.gf.number(self.value - other.value)

        def __mul__(self, other):
            assert(isinstance(other, GF.Number))
            return self.gf.number(self.value * other.value)

        def __neg__(self):
            return self.gf.number(-self.value)

        def __div__(self, other):
            return self.__truediv__(other)

        def __truediv__(self, other):
            assert(isinstance(other, GF.Number))
            return self.gf.number(self.value * self.eea(Fp, other.value)[1])

        def __pow__(self, i):
            assert(isinstance(i, int))
            return self.gf.number(self.value ** i)

        def __eq__(self, other):
            assert(isinstance(other, GF.Number))
            return self.value == other.value

        def __str__(self):
            return "0x{:x}".format(self.value)

        def eea(self, i, j):
            """Rozsirena eulerova metoda"""
            assert(isinstance(i, int))
            assert(isinstance(j, int))
            (s, t, u, v) = (1, 0, 0, 1)
            while j != 0:
                (q, r) = (i // j, i % j)
                (unew, vnew) = (s, t)
                s = u - (q * s)
                t = v - (q * t)
                (i, j) = (j, r)
                (u, v) = (unew, vnew)
            (d, m, n) = (i, u, v)
            return (d, m, n)

class EllipticCurve(object):

    def __init__(self, gf, a, b):
        self.gf = gf
        self.a = self.gf.number(a)
        self.b = self.gf.number(b)

    def point(self, x, y):
        return EllipticCurve.Point(self.gf.number(x), self.gf.number(y), self)

    def O(self):
        O = self.point(0, 0)
        O.x.value = -1
        O.y.value = -1
        return O

    class Point(object):

        """Bod krivky."""
        def __init__(self, x, y, elipticCurve):
            self.x = x
            self.y = y
            self.elipticCurve = elipticCurve
            self.CONST_3 = self.elipticCurve.gf.number(3)

        def __add__(self, other):
            """Scitani dvou bodu."""
            assert isinstance(other, EllipticCurve.Point)
            O = self.elipticCurve.O()
            if self == -other:
                return O
            if self == O:
                return other
            if other == O:
                return self
            return self.__point_add(other) if self != other else self.__point_double()

        def __neg__(self):
            """
            P = (x, y) mod p
            -P = (x, -y) mod p
            """
            return EllipticCurve.Point(self.x, -self.y, self.elipticCurve)

        def point_scalar_mul(self, k):
            N = self
            R = self.elipticCurve.O()
            for bit in range (bitlength):
                if k & (1 << bit):
                    R = R + N
                N = N + N
            return R

        def __point_add(self, other):
            s = (self.y - other.y) / (self.x - other.x)
            Rx = s**2 - self.x  - other.x
            Ry = s * (self.x - Rx) - self.y
            return EllipticCurve.Point(Rx, Ry, self.elipticCurve)

        def __point_double(self):
            s = (self.CONST_3 * self.x**2 + self.elipticCurve.a) / (self.y + self.y)
            Rx = s**2 - self.x  - self.x
            Ry = s * (self.x - Rx) - self.y
            return EllipticCurve.Point(Rx, Ry, self.elipticCurve)

        def __eq__(self, other):
            assert isinstance(other, EllipticCurve.Point)
            return self.x == other.x and self.y == other.y

        def __str__(self):
            return "O" if self == self.elipticCurve.O() else "({}, {})".format(self.x, self.y)

Fp = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff
a = -0x3
b = 0x5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b

G = (0x6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296,
     0x4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5)

# Linearni reseni prolomeni verejneho klice.
def solution(p, a, b, G, Q):
    gf = GF(p)
    ec = EllipticCurve(gf, a, b)
    # Public key
    Q = ec.point(Q[0], Q[1])
    # Pocatecni bod
    G = ec.point(G[0], G[1])

    d = 1
    Q_new = G
    while Q_new != ec.O() and Q_new != Q:
        Q_new = Q_new + G
        d += 1

    return d

def publicKey(s):
    PATTERN = re.compile("\((0x[0-9a-f]+), (0x[0-9a-f]+)\)")
    match = PATTERN.match(s)
    if match:
        return (int(match.group(1), 16), int(match.group(2), 16))
    else:
        raise argparse.ArgumentTypeError("Public key must be '(0x[0-a-f]+, 0x[0-9a-f]+)'.'")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('publicKey', metavar='publicKey', help="", type=publicKey)
    args = parser.parse_args()

    print(solution(Fp, a, b, G, args.publicKey))
