#!/usr/bin/env python

with open('encoded', 'r') as f:
    nums = map(lambda x: x.strip(), f.readlines())
with open('encwords', 'w') as f:
    f.write(' '.join(nums))
    f.write('\n')
    
