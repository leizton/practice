# -*- coding: UTF-8 -*-
import random
import utils

# ε-greedy

class EpsilonGreedyGlobalData:
  def __init__(self):
    self.epsilon = 0.01
    self.coin_num = 0
    self.coin_select_num = []
    self.coin_reward = []
    self.explore_num = 0

G = EpsilonGreedyGlobalData()

def onInit(coin_n):
  G.coin_num = coin_n
  for i in range(G.coin_num):
    G.coin_select_num += [0]
    G.coin_reward += [0]
  return

def onComplete(reward_sum, select_prop):
  print('G.explore_num: %s' % G.explore_num)
  return

def action(run_i):
  # 先把所有coin选一遍
  for i in range(G.coin_num):
    if G.coin_select_num[i] == 0:
      return i
  if utils.randHit(G.epsilon):
    G.explore_num += 1
    return utils.randint(G.coin_num)
  else:
    # 选择平均收益最大的coin
    select_coin, max_reward = -1, -1
    for i in range(G.coin_num):
      reward_mean = G.coin_reward[i] / G.coin_select_num[i]
      if reward_mean > max_reward:
        select_coin = i
        max_reward = reward_mean
    return select_coin

def feedback(run_i, coin_i, reward):
  G.coin_select_num[coin_i] += 1
  G.coin_reward[coin_i] += reward
  return
