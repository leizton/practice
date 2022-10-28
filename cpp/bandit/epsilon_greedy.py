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
    # 计算平均收益最大的coin
    max_reward_coin_i, max_reward = -1, -1
    for i in range(self.coin_num):
      reward_mean = self.coin_reward[i] / self.coin_select_num[i]
      if reward_mean > max_reward:
        max_reward_coin_i = i
        max_reward = reward_mean
    if utils.randHit(self.epsilon):
      self.explore_num += 1
      select = utils.randint(self.coin_num)
      while select == max_reward_coin_i:
        select = utils.randint(self.coin_num)
      return select
    else:
      return max_reward_coin_i

  def feedback(self, run_i, coin_i, reward):
    self.coin_select_num[coin_i] += 1
    self.coin_reward[coin_i] += reward
  