  # -*- coding: UTF-8 -*-
import random
import utils

# ε-greedy

class EpsilonGreedy:
  def __init__(self):
    self.epsilon = 0.01
    self.coin_num = 0
    self.coin_select_num = []
    self.coin_reward = []
    self.explore_num = 0

  def onInit(self, coin_n):
    self.coin_num = coin_n
    for i in range(self.coin_num):
      self.coin_select_num += [0]
      self.coin_reward += [0]

  def onComplete(self, reward_sum, select_prop):
    print('self.explore_num: %s' % self.explore_num)

  def action(self, run_i):
    # 先把所有coin选一遍
    for i in range(self.coin_num):
      if self.coin_select_num[i] == 0:
        return i
    if utils.randHit(self.epsilon):
      self.explore_num += 1
      return utils.randint(self.coin_num)
    else:
      # 选择平均收益最大的coin
      select_coin, max_reward = -1, -1
      for i in range(self.coin_num):
        reward_mean = self.coin_reward[i] / self.coin_select_num[i]
        if reward_mean > max_reward:
          select_coin = i
          max_reward = reward_mean
      return select_coin

  def feedback(self, run_i, coin_i, reward):
    self.coin_select_num[coin_i] += 1
    self.coin_reward[coin_i] += reward
  