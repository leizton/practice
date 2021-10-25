import nni
import math

if __name__ == '__main__':
  nni_exp_id = nni.get_experiment_id()
  nni_trial_id = nni.get_trial_id()
  nni_seq_id = nni.get_sequence_id() # from 0
  next_params = nni.get_next_parameter()
  x = next_params["x"]
  x = float(x) / 100.0
  y = math.sin(x)
  print('%s %s %d %.4f %.4f' % (nni_exp_id, nni_trial_id, nni_seq_id, x, y))
  # nni.report_intermediate_result(y)
  nni.report_final_result(y)
