# ref
最新进展  https://nlpprogress.com/

# 基本概念
词序             单词顺序
语法             控制词序的规则
词袋             BOW bag-of-word. 不考虑sen(句子或doc)的词序, 直接统计每个词的次数得到sen的特征向量
n-gram           n元语法. 考虑n步内的词序. 也可以看成(n-1)阶马尔可夫链. 或n步内的上下文
预测任务         给定上下文或历史, 给出某个位置出现某个单词的概率
POS-tag          part-of-speech 词性标签, 如名词NN, 动词VB
lemmatization    词性还原
stemming         词干提取

# n-gram的局限性
1. 随着n的增加, n元组的种类会迅速膨胀. 参数空间的爆炸式增长使之无法处理更长程的context
2. 不能对相似词建模. 如: ‘the cat is running’, 即使没出现过‘the dog is running’, 也能根据dog和cat的相似推断这句话的概率, 但n-gram不能

# 深度语言模型的发展
从统计模型转到深度模型
https://www.infoq.cn/article/66vicQt*GTIFy33B4mu9
2003 A neural probabilistic language model. by Bengio
2010 循环神经网络 rnn. by Mikolov
2013 word2vec. by Mikolov. 提升了词嵌入的性能, 从而可以做大规模词嵌入训练
  word2vec得到的词嵌入向量有一个神奇点: 学习到语义信息
2013 长短时记忆网络 LSTM. by 格雷夫斯(Graves)
2014 sequence-to-sequence 模型. by Sutskever
2015 注意力机制. by Bahdanau

# distributed & distributional
distributed
  A concept is represented as continuous activation levels in a number of elements. Like a dense word embedding, as opposed to 1-hot vectors.
  在向量空间上连续地表示词. 比如稠密的词嵌入向量表示, 与之相反的是稀疏离散的独热表示, 语义表示分散(distributed)在各bit上
distributional
  Meaning is represented by contexts of use. Word2vec is distributional, but so are count-based word vectors, as we use the contexts of the word to model the meaning.
  通过单词的上下文(在文本中前后几个单词)来建模语义信息, 比如w2vec和基于计数的词向量, 其中countBased wVec的上下文是doc
  而词袋是上下文无关(context-free)建模
完整版
  distributed representation 分布式表示
  distributional model 分布模型
localist representation: 与分布式表示相反
  @ref https://www.districtdatalabs.com/nlp-research-lab-part-1-distributed-representations
  这篇文章介绍了1-hot思想的来源. 1-hot每位表示一个概念(单词或名词短语), ‘0’表示对该概念取反或不存在于当前语境中
    assign one of NN's units to the representation of this concept.
    distributed representation is mainly driven by the fact that the representation of any single concept is distributed over many, if not all, processing units.
    distributed: 一个概念的表示是分配到多个unit. continuous values instead of discrete 1 and 0.

# A neural probabilistic language model
第一篇NN语言模型. by Bengio
考虑对有 10w个单词的10-gram 建立联合分布模型, 则有 10w ^ 10 = 1e50 个自由参数
本文将考虑: 比tri-gram更多的上下文词, 词之间的相似性
使用 分布式表示 解决维度灾难
词向量即词的特征向量. NN不仅能最优化隐藏函数, 还能自动学得特征
设词表V, 词表大小|V|, 词向量维度m
映射C: 从词表任一元素i 到 实向量C(i)∈R^m. 实践中, C表示成 |V|×m 的自由参数矩阵

# word2vec
word2vec学到的语义信息
  vecDiff(beijing,chain) vecDiff(tokyo,japan) vecDiff(Moscow,Russia) 相近
  vecDiff(given,give,gave) vecDiff(taken,take,took) 相近
  w2v特征向量相似反应的是语义上相似, 而不是统计上相似
两种w2v
  CBOW(连续词袋) skip-gram

# transformer

# BERT
https://zhuanlan.zhihu.com/p/51413773

# 斯坦福解析器
https://nlp.stanford.edu/software/lex-parser.shtml
> Fast Exact Inference with a Factored Model for Natural Language Parsing