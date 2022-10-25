# -*- coding: UTF-8 -*-
import random
import utils

# ε-greedy
# 缺点: 选择多时极容易陷入某个非优解

epsilon = 0.01
coin_num = 0
coin_reward = []
explore_num = 0

def onInit(coin_n):
  global epsilon, coin_num, coin_reward, explore_num
  coin_num = coin_n
  for i in range(coin_num):
    coin_reward += [0]
  return

def onComplete(reward_sum, select_prop):
  global epsilon, coin_num, coin_reward, explore_num
  print('explore_num: %s' % explore_num)
  return

def action(run_i):
  global epsilon, coin_num, coin_reward, explore_num
  if utils.rand(epsilon):
    explore_num += 1
    return utils.randint(coin_num)
  else:
    max_reward = -1
    for i in range(coin_num):
      max_reward = max(max_reward, coin_reward[i])
    candidate = []
    for i in range(coin_num):
      if coin_reward[i] >= (max_reward - 1):
        candidate += [i]
    return candidate[utils.randint(len(candidate))]

def feedback(run_i, coin_i, reward):
  coin_reward[coin_i] += reward
  return
