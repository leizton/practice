# -*- coding: UTF-8 -*-
import random

coin_num = 0

def onInit(coin_n):
  global coin_num
  coin_num = coin_n
  return

def onComplete(reward_sum, select_prop):
  return

def action(run_i):
  return random.randint(0, coin_num-1)

def feedback(run_i, coin_i, reward):
  return
