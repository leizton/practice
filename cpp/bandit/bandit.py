# -*- coding: UTF-8 -*-
import math
import random
import matplotlib.pyplot as plt

import utils
import random_strategy
import epsilon_greedy

# 假设n个硬币 正面概率不同
# 正面奖励1 反面奖励0 最大化一段时间后的收益
class BanditGlobalData:
  def __init__(self):
    self.coin_num = 0
    self.coin_prob = []
    self.coin_history = []
    self.run_num = 0
    self.reward_sum = 0
    self.reward_sum_history = []
    self.debug = True
    self.strategy = None

G = BanditGlobalData()

def init():
  G.run_num = 100000
  G.coin_num = 10
  G.coin_prob = utils.randAlloc(0.1, 0.9, G.coin_num)
  #
  random_reward, max_coin_prob = 0, 0.
  for i in range(G.coin_num):
    random_reward += G.coin_prob[i] * G.run_num
    max_coin_prob = max(max_coin_prob, G.coin_prob[i])
  random_reward = int(random_reward/G.coin_num)
  print('random_reward: %s' % random_reward)
  print('max_reward: %s' % round(G.run_num * max_coin_prob, 0))
  #
  G.strategy.onInit(G.coin_num)

def reset():
  G.coin_history = []
  for i in range(G.coin_num):
    G.coin_history += [[]]
  G.reward_sum = 0

# 选择第coin_i个硬币抛
def sample(coin_i):
  reward = utils.randHit(G.coin_prob[coin_i])
  G.reward_sum += reward
  G.reward_sum_history += [G.reward_sum]
  if G.debug:
    G.coin_history[coin_i] += [reward]
  return reward

def run():
  for run_i in range(G.run_num):
    coin_i = G.strategy.action(run_i)
    reward = sample(coin_i)
    G.strategy.feedback(run_i, coin_i, reward)

def statResult():
  if G.debug:
    select_prop = []
    for coin_i in range(G.coin_num):
      select_prop += [len(G.coin_history[coin_i])]
    select_prop = utils.norm(select_prop)
    print('--------------------')
    G.strategy.onComplete(G.reward_sum, select_prop)
    print('--------------------')
    print('reward_sum: %s' % G.reward_sum)
    print('select_prop: %s' % select_prop)
    print('coin_prob: %s' % G.coin_prob)
  return G.reward_sum

def test(strategy):
  G.strategy = strategy
  init()
  reset()
  run()
  statResult()

def testEpsilon():
  G.strategy = epsilon_greedy.EpsilonGreedy()
  G.debug = False
  init()
  print('coin_prob: %s' % G.coin_prob)
  print('--------------------')
  G.strategy.epsilon = 0
  reward_mean, reward_min, reward_max = [], [], []
  for i in range(80):
    G.strategy.epsilon += 0.01
    r_mean, r_min, r_max = 0, int(math.pow(2, 60)), 0
    exp_n = 10
    for j in range(exp_n):
      reset()
      run()
      r = statResult()
      r_mean += r
      r_min, r_max = min(r_min, r), max(r_max, r)
      # print('run %s %s %s' % (i, j, r))
    r_mean = int(round(r_mean/exp_n, 0))
    reward_mean += [r_mean]
    reward_min += [r_min]
    reward_max += [r_max]
    print('%-7s %-7s %-7s %-7s' % (round(G.strategy.epsilon, 2), r_mean, r_min, r_max))
  plt.plot(range(len(reward_mean)), reward_mean, '-', linewidth=1)
  plt.plot(range(len(reward_min)), reward_min, '-', linewidth=1)
  plt.plot(range(len(reward_max)), reward_max, '-', linewidth=1)
  plt.show()

if __name__ == '__main__':
  test(epsilon_greedy.EpsilonGreedy())
