# -*- coding: UTF-8 -*-
import os
import sys
import math
import numpy as np
from matplotlib import pyplot as plt

def show():
  ax = plt.gca()
  ax.xaxis.set_major_locator(plt.MultipleLocator(1))
  plt.grid(axis='x')
  plt.show()

def arr(y, x=None, ty='-'):
  if x is None:
    x = range(0, len(y))
  return plt.plot(x, y, ty, linewidth=1)

def gauss_op(x, delta_inv):
  t = -1 * x * x * 0.5 * delta_inv * delta_inv
  return math.exp(t) * delta_inv

def gauss(x, delta):
  delta_inv = 1 / delta
  y = []
  for e in x:
    y += [gauss_op(e, delta_inv)]
  return y

x = np.linspace(-10, 10, 200)
y1 = gauss(x, 1)
y2 = gauss(x, 2)
y3 = []
for i in range(len(x)):
  y3 += [y1[i] - y2[i]]
l1, = arr(y1, x)
l2, = arr(y2, x)
l3, = arr(y3, x)
plt.legend(handles=[l1,l2,l3], labels=['y1','y2','y3'])
show()
