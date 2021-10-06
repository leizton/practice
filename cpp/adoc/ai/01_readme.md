# ref
paper search: https://paperswithcode.com/sota

# 概念
SOTA：state of the art 当前技术状态，SOTA模型指当前最好的模型
generative model：生成模型
discriminative model：判别模型
  监督学习方法分成生成方法和判别方法，所学到的模型分别是生成模型和判别模型
  GM是先学习联合概率P(XY)，然后用P(Y|X)=P(XY)/P(X)得到条件概率，例如贝叶斯
  DM是直接构建P(Y|X)来预测y，例如线性回归、SVM、随机森林、CRF(条件随机场)
MLE：maximum-likelihood estimates 最大似然估计