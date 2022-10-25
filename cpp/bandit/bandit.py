# -*- coding: UTF-8 -*-
import math
import random
import matplotlib.pyplot as plt

import utils
import random_strategy
import epsilon_greedy as curr_strategy

# 假设n个硬币 正面概率不同
# 正面奖励1 反面奖励0 最大化一段时间后的收益
class GlobalData:
  def __init__(self):
    self.coin_num = 0
    self.coin_prob = []
    self.coin_history = []
    self.run_num = 0
    self.reward_sum = 0
    self.debug = True

G = GlobalData()

def init():
  global G
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
  curr_strategy.onInit(G.coin_num)

def reset():
  global G
  G.coin_history = []
  for i in range(G.coin_num):
    G.coin_history += [[]]
  G.reward_sum = 0

# 选择第coin_i个硬币抛
def sample(coin_i):
  global G
  reward = utils.rand(G.coin_prob[coin_i])
  G.reward_sum += reward
  if G.debug:
    G.coin_history[coin_i] += [reward]
  return reward

def run():
  global G
  for run_i in range(G.run_num):
    coin_i = curr_strategy.action(run_i)
    reward = sample(coin_i)
    curr_strategy.feedback(run_i, coin_i, reward)

def statResult():
  global G
  if G.debug:
    select_prop = []
    for coin_i in range(G.coin_num):
      select_prop += [len(G.coin_history[coin_i])]
    select_prop = utils.norm(select_prop)
    print('--------------------')
    curr_strategy.onComplete(G.reward_sum, select_prop)
    print('--------------------')
    print('reward_sum: %s' % G.reward_sum)
    print('select_prop: %s' % select_prop)
    print('coin_prob: %s' % G.coin_prob)
  return G.reward_sum

def test():
  init()
  reset()
  run()
  statResult()

def testEpsilon():
  global G
  G.debug = False
  init()
  print('coin_prob: %s' % G.coin_prob)
  print('--------------------')
  curr_strategy.epsilon = 0
  reward_mean, reward_min, reward_max = [], [], []
  for i in range(80):
    curr_strategy.epsilon += 0.01
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
    print('%-7s %-7s %-7s %-7s' % (round(curr_strategy.epsilon, 2), r_mean, r_min, r_max))
  plt.plot(range(len(reward_mean)), reward_mean, '-', linewidth=1)
  plt.plot(range(len(reward_min)), reward_min, '-', linewidth=1)
  plt.plot(range(len(reward_max)), reward_max, '-', linewidth=1)
  plt.show()

if __name__ == '__main__':
  test()
