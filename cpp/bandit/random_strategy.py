# -*- coding: UTF-8 -*-
import random

class RandomBandit:
  def __init__(self):
    self.coin_num = 0

  def onInit(self, coin_n):
    self.coin_num = coin_n

  def onComplete(self, reward_sum, select_prop):
    return

  def action(self, run_i):
    return random.randint(0, self.coin_num-1)

  def feedback(self, run_i, coin_i, reward):
    return
