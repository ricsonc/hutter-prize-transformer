#!/usr/bin/env python


from tensor2tensor.data_generators import problem
from tensor2tensor.data_generators import text_problems
#from tensor2tensor.data_generators import text_encoder
from tensor2tensor.utils import registry
from ipdb import set_trace as st
import random

@registry.register_problem
class EnWik(text_problems.Text2SelfProblem):
# You can also choose to use a character-level encoder or a token encoder where you provide the vocab file yourself. See Text2TextProblem.vocab_type.

  @property
  def vocab_type(self):
    return text_problems.VocabType.TOKEN

  @property
  def is_generate_per_split(self):
    return True

  def generate_samples(self, data_dir, tmp_dir, dataset_split):
    """Generate samples of text.
    Args:
      data_dir: final data directory. Typically only used in this method to copy
        over user-supplied vocab files (for example, if vocab_type ==
        VocabType.TOKEN).
      tmp_dir: temporary directory that you can use for downloading and scratch.
      dataset_split: problem.DatasetSplit, which data split to generate samples
        for (for example, training and evaluation).
    Yields:
      Sample: dict<str feature_name, str text>: for language modeling problems
        (i.e. Text2SelfProblems), this generator should yield dicts with only
        the "targets" key.
    """
    #The vocab file should be stored in
    #    `data_dir/` with the name specified by `self.vocab_filename`.
    
    target = None
    length = 128

    with open('encwords', 'r') as f:
      toks = f.read().strip().split()
    N = len(toks)

    c = 0
    # while 1:
    #   c += random.randint(0, length)
    #   c %= (N-length)
    
    while c < N-length:
      #c += random.randint(1, 10)
      yield {"targets": ' '.join(toks[c:c+length])}
      c+=4
