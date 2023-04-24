#!/usr/bin/env python3

import itertools
import sys
import os
import argparse

N_B = 32
N = 8 * N_B

# Next keystream
def step(x, SUB):
    x = (x & 1) << N+1 | x << 1 | x >> N-1
    y = 0
    for i in range(N):
        y |= SUB[(x >> i) & 7] << i
    return y

# Previous keystream
def prevStep(z, SUB):
    n = [{idx for idx, val in enumerate(SUB) if val == 0},
         {idx for idx, val in enumerate(SUB) if val == 1}]
    k = {0x0, 0x1, 0x2, 0x3}

    for i in range(N):
        m = n[(z>>i) & 1]
        k = {(x<<i) | y for x, y in itertools.product(m, k) if (x&0x3) ^ (y>>i) == 0}

    return [b>>1 & ((1<<N) -1) for b in k if b & 3 == b >> N][0]

# Get key from keystrem
def getSecretKey(keystr):
    key = int.from_bytes(keystr[:N_B],'little')
    key2 = int.from_bytes(keystr[N_B:2*N_B],'little')

    for s in [n for n in itertools.product([0,1], repeat=8)]:
        if step(key, s) == key2:
            break

    for i in range(N//2):
        key = prevStep(key, s)

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
