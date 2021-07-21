import numpy as np

def normRand(mu, sigma):
  return int(np.random.normal(mu, sigma, 1)[0])

def handle(lvl):
  if lvl == 1:
    avg_cost, max_cost = 220, 400
    mu, sigma = avg_cost, (max_cost - avg_cost) / 3
    res = max(normRand(mu, sigma), 100)
  else:
    mu, sigma = 28, 2.5
    res = max(normRand(mu, sigma), 20)
  return res

class SlideWindow:
  def __init__(self, wnd_size, init_val = 0):
    self.wnd_size = wnd_size
    self.wnd = [init_val] * wnd_size
    self.next = 0

  def push(self, val):
    self.wnd[self.next] = val
    self.next = (self.next + 1) % self.wnd_size
    return self.get()
  
  def get(self):
    sum = 0.
    for v in self.wnd:
      sum += v
    return int(sum / self.wnd_size)

class PID:
  def __init__(self):
    self.target = 200
    self.kp, self.ki, self.kd = 10, 1.0, 0.001
    self.pv, self.iv, self.dv = 0.0, 0.0, 0.0
    self.err, self.last_err = 0.0, 0.0

  def update(self, feedback):
    err = self.target - feedback
    self.pv = self.kp * err
    self.iv += err
    self.dv = err - self.last_err
    self.last_err = err
    out = self.pv + self.ki * self.iv + self.kd * self.dv
    return out

if __name__ == '__main__':
  feedback = 300
  feedback_wnd = SlideWindow(16, feedback)
  feedback_smooth = feedback_wnd.get()
  pid = PID()
  for i in range(10000):
    prev_feedback = feedback
    out = int(pid.update(feedback))
    lvl = 0 if out < 500 else 1
    feedback = handle(lvl)
    feedback_smooth = feedback_wnd.push(feedback)
    print('%s\t%s\t%s\t%s\t%s' % (i, out, lvl, feedback, feedback_smooth))
