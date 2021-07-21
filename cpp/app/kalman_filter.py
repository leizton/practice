# -*- coding: UTF-8 -*-

import numpy as np
import pylab

def genNext():
  mu = 200
  sigma = 70
  res = int(np.random.normal(mu, sigma, 1)[0])
  return max(100, res)

Q = 1e-5  # 过程噪声
R = 4e-3  # 观测噪声

if __name__ == '__main__':
  x_predict = 150.  # x的先验估计
  x_estimate = 0.   # x的后验估计
  p_predict = 0.    # p的先验估计
  p_estimate = 0.   # p的后验估计
  k_gain = 0.       # 卡尔曼增益

  measure_arr = []
  estimate_arr = []

  for i in range(0, 200):
    # 观测
    measure_val = genNext()
    measure_arr += [measure_val]

    # 估计
    k_gain = p_predict / (p_predict + R)
    x_estimate = x_predict + k_gain * (measure_val - x_predict)
    p_estimate = (1 - k_gain) * p_predict
    estimate_arr += [x_estimate]
    # print('%s\t%s\t%s' % (i, measure_val, int(x_estimate)))

    # 更新
    x_predict = x_estimate
    p_predict = p_estimate + Q

  # plot
  pylab.figure()
  pylab.plot(measure_arr, '-', color='k')
  pylab.plot(estimate_arr, '-', color='b')
  pylab.show()
