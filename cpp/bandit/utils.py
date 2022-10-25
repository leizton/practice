# -*- coding: UTF-8 -*-
import math
import random

def pow10(n):
  v = int(1)
  for i in range(n):
    v *= int(10)
  return v

def norm(vec, precision = 3):
  total = 0
  for n in vec:
    total += n
  for i in range(len(vec)):
    vec[i] = round(float(vec[i]) / float(total), precision)
  return vec

def randint(rand_end):
  return random.randint(0, rand_end-1)

def rand(prob, precision=8):
  precision = max(1, min(15, int(precision)))
  max_rand = pow10(precision)
  return 1 if randint(max_rand) < (max_rand * prob) else 0

# 随机分配
def randAlloc(prob_min, prob_max, N, precision=4):
  # alloc[i] ∈ [prob_min,prob_max]
  # len(alloc) = N
  # sum(alloc) = (prob_max+prob_min)/2 * N
  A = pow10(10)
  r_min, r_max = A * prob_min, A * prob_max
  alloc = []
  for i in range(N):
    r = random.randint(r_min, r_max)
    alloc += [r]
  # 平移
  mean = (r_max + r_min) / 2
  distance = int(round(mean - sum(alloc) / N, 0))
  alloc_min, alloc_max = alloc[0], alloc[0]
  for i in range(N):
    alloc[i] += distance
    alloc_min = min(alloc_min, alloc[i])
    alloc_max = max(alloc_max, alloc[i])
  # 缩放
  mean = sum(alloc) / N
  if alloc_min < r_min:
    scale = (mean - r_min) / (mean - alloc_min)
  elif alloc_max > r_max:
    scale = (r_max - mean) / (alloc_max - mean)
  else:
    scale = 1
  for i in range(N):
    alloc[i] = mean - scale * (mean - alloc[i])
  #
  prod_d = prob_max - prob_min
  r_size = r_max - r_min + 1
  for i in range(N):
    r = alloc[i]
    r = prob_min + (r - r_min) * prod_d / r_size
    alloc[i] = round(r, precision)
  alloc = sorted(alloc)
  return alloc
