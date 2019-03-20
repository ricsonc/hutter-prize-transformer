#!/usr/bin/env python

with open('encwords', 'r') as f:
    nums = f.read().strip().split()
    unique = set(nums)

with open('data/vocab.en_wik.tokens', 'w') as f:
    for tok in map(str,sorted(list(map(int, unique)))):
        f.write(tok)
        f.write('\n')
