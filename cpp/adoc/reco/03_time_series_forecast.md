# ref
https://neuralprophet.com/

# Prophet
原理
  分解成趋势项、周期项、事件(如假日 上线)、噪声
缺点
  不能利用未来不可知的特征。例如气温受风速影响，风速在学习时知道(过去可知)，预测时不知道
  不支持非高斯分布, https://github.com/facebook/prophet/issues/1500
  识别异常点、趋势随机性、多时序预测

# 自回归 AR auto regress
概念
  用自身的历史数据预测未来，具体公式如下
  yₜ = (∑wᵢ·yₜ₋ᵢ + b) + eₜ, 即线性回归+误差
应用
  解决未来不可知特征的未来数据
