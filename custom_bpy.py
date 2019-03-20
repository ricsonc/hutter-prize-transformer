#!/usr/bin/env python

# just do simple bpe -- on raw bytes!
from ipdb import set_trace as st
from collections import defaultdict
import pickle
import cProfile

current_code = 1000
codes = {}

def compute_mcp(sequence):
    d = defaultdict(int)
    for i in range(len(sequence)-1):
        d[sequence[i], sequence[i+1]] += 1
    d = list(d.items())
    d = sorted(d, key = lambda x:x[-1])
    mcp = d[-1]
    if mcp[-1] < 2:
        return None #mcp has freq of 1
    return mcp[0]

def replace_pair(sequence, mcp):
    global current_code, codes
    i = 0
    newseq = []
    while i < len(sequence)-1:
        if (sequence[i], sequence[i+1]) == mcp:
            newseq.append(current_code)
            i += 2
        else:
            newseq.append(sequence[i])
            i += 1
    if i == len(sequence)-1:
        newseq.append(sequence[-1])
    return newseq

def do_one_round(sequence):
    global current_code, codes
    mcp = compute_mcp(sequence)
    sequence = replace_pair(sequence, mcp)
    codes[current_code] = mcp
    current_code += 1
    return sequence

def make_vocab_size(n, sequence):
    base_size = len(set(sequence))
    for j in range(n-base_size):
        print('round', j+1)
        sequence = do_one_round(sequence)
    return sequence

def expand_pair(out, code):
    newout = []
    for x in out:
        if x == code[0]:
            newout.extend(list(code[1]))
        else:
            newout.append(x)
    return newout

def decode(encoded, codes):
    codes = sorted(list(codes.items()))
    out = encoded
    while codes:
        code = codes.pop()
        out = expand_pair(out, code)
    return out

def open_encode_and_save(infile, n):
    with open(infile, 'rb') as f0:
        sequence = list(f0.read())
    sequence = make_vocab_size(n, sequence)
    with open('encoded', 'wb') as f1:
        pickle.dump(sequence, f1)
    with open('codes', 'wb') as f2:
        pickle.dump(codes, f2)

# encoded = make_vocab_size(7, data)
# decoded = decode(encoded, codes)
# print(encoded)
# print(codes)
# print(bytes(decoded))
open_encode_and_save('enwik8', 10000)
