#!/usr/bin/env bash

# t2t-datagen \
#   --t2t_usr_dir=. \
#   --data_dir=data \
#   --tmp_dir=tmp \
#   --problem=en_wik

t2t-trainer \
    --t2t_usr_dir=. \
    --data_dir=data \
    --model=transformer --hparams_set=transformer_tiny \
    --problem=en_wik \
    --tmp_dir=tmp \
    --output_dir=out 
