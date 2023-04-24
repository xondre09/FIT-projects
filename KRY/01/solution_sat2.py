#!/usr/bin/env python3

from z3 import Solver, Or, And, Not, Bool, unsat, BitVec
import os
import sys
import argparse
import itertools

parser = argparse.ArgumentParser()
parser.add_argument("dir", action='store')
args = parser.parse_args()

N_B = 32
N = 8 * N_B

# Next keystream
def step(x, sub):
    x = (x & 1) << N+1 | x << 1 | x >> N-1
    y = 0
    for i in range(N):
        y |= sub[(x >> i) & 7] << i
    return y

# Previous keystrem
def reverseStep(x, exprs):
    s = Solver()
    s.add(exprs[-1])
    for i in range(N):
        expr = exprs[i] == bool(x>>i & 1)
        s.add(expr)

    y = 0
    if s.check() != unsat:
        m = s.model()
        return (m[m[0]].as_long() >> 1 & (1<<N)-1)
    return y

# Get key from keystrem
def getSecretKey(keystr):
    key = int.from_bytes(keystr[:N_B],'little')
    key2 = int.from_bytes(keystr[N_B:2*N_B],'little')

    for s in [n for n in itertools.product([0,1], repeat=8)]:
        if step(key, s) == key2:
            break

    x = BitVec('x', N+2)
    exprs = [Or(*[((x >> i) & 0x07) == idx for idx, item in enumerate(s) if item == 1]) for i in range(N)]
    exprs.append(x & 3 == (x>>N) & 3)

    for i in range(N//2):
        key = reverseStep(key, exprs)

    return key.to_bytes(32, 'little')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('dir', action='store')
    args = parser.parse_args()

    fileA = os.path.join(args.dir, 'bis.txt')
    fileB = os.path.join(args.dir, 'bis.txt.enc')

    with open(fileA, 'rb') as fa, open(fileB, 'rb') as fb:
         keystr = bytearray(a^b for a, b in itertools.zip_longest(fa.read(), fb.read(), fillvalue=0x00))

    key = getSecretKey(keystr)

    if key[:4] == b'KRY{' and key[28] == ord('}'):
        sys.stdout.buffer.write(key[:29])
        sys.stdout.write('\n')
