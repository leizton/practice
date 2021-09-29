import nni
import math

if __name__ == '__main__':
  next_params = nni.get_next_parameter()
  x = float(next_params["x"])
  y = math.sin(x)
  # nni.report_intermediate_result(y)
  nni.report_final_result(y)
