# -*- coding: UTF-8 -*-
import random

import utils
import random_strategy
import epsilon_greedy as curr_strategy

# 假设5个硬币 正面概率不同
# 正面奖励1 反面奖励0 最大化一段时间后的收益
coin_num = 0
coin_prob = []
coin_history = []
run_num = 0

def init():
  global coin_num, coin_prob, coin_history, run_num
  coin_num = 5
  run_num = 100000
  coin_prob = [0.8, 0.3, 0.2, 0.5, 0.5]
  for i in range(coin_num):
    coin_history += [[]]
  #
  random_reward = 0
  for i in range(coin_num):
    random_reward += coin_prob[i] * run_num
  random_reward = int(random_reward/coin_num)
  print('random_reward: %s' % random_reward)
  print('max_reward: %s' % run_num)
  #
  curr_strategy.onInit(coin_num)

# 选择第coin_i个硬币抛
def sample(coin_i):
  global coin_num, coin_prob, coin_history, run_num
  reward = utils.rand(coin_prob[coin_i])
  coin_history[coin_i] += [reward]
  return reward

def run():
  global coin_num, coin_prob, coin_history, run_num
  for run_i in range(run_num):
    coin_i = curr_strategy.action(run_i)
    reward = sample(coin_i)
    curr_strategy.feedback(run_i, coin_i, reward)

def statResult():
  reward_sum = 0
  select_prop = []
  for coin_i in range(coin_num):
    select_prop += [len(coin_history[coin_i])]
    for reward in coin_history[coin_i]:
      reward_sum += reward
  select_prop = utils.norm(select_prop)
  print('--------------------')
  curr_strategy.onComplete(reward_sum, select_prop)
  print('--------------------')
  print('reward_sum: %s' % reward_sum)
  print('select_prop: %s' % select_prop)

if __name__ == '__main__':
  init()
  run()
  statResult()
