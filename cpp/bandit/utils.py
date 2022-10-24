# -*- coding: UTF-8 -*-
import math
import random

def norm(vec, precision = 3):
  total = 0
  for n in vec:
    total += n
  for i in range(len(vec)):
    vec[i] = round(float(vec[i]) / float(total), precision)
  return vec

def randint(rand_end):
  return random.randint(0, rand_end-1)

def rand(prob, precision = 7):
  precision = max(1, min(15, int(precision)))
  max_rand = int(1)
  for i in range(precision):
    max_rand *= 10
  return 1 if randint(max_rand) < (max_rand * prob) else 0
