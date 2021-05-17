#
http://web.stanford.edu/~jurafsky/slp3/3.pdf

NLP的发展经过了从统计模型到深度模型

对一个客观事物的理解能力, 可以看成对事物发展的预测能力
当能完全预测, 则得到的是定律, 即固定的规律

NLP的统计语言模型的一个预测任务: P(w|h), the probability of a word w given some history h

因为language is creative and any particular context might have never occurred before,
所以把history近似成最近的几个单词, 因此有了n-gram

n元语法(grammar): 一种基于(n-1)阶马尔可夫链的概率语言模型
任务变成 计算P(w|hn)

通过对语料库(corpus)统计, 可以得到相对频率 C(hn·w)/C(hn)
为了计算概率, 需要用到最大似然估计(max likelihood estimation, MLE)
对于train-corpus, 相对频率等于最大似然概率


# 词袋模型
BOW: bag of words。所有词放到同一个集合(袋子)里
把句子或doc变成词向量, 不考虑词法、词性、语序。
如以下两个句子
  Jane wants to go to Shenzhen
  Bob wants to go to Shanghai
词袋
  [Jane, wants, to, go, Shenzhen, Bob, Shanghai]
上面两个句子的词频向量: 
  [1,1,2,1,1,0,0]    -- 第一个数字1 表示 Jane出现1次
  [0,1,2,1,0,1,1]    -- 第一个数字0 表示 Jane出现0次


# n-gram的局限性
1. 随着n的增加, hn的种类数会迅速膨胀。参数空间的爆炸式增长使之无法处理更长程的context
2. 没有对相似词建模。如: ‘the cat is running’, 即使没出现过‘the dog is running’, 也能根据dog和cat的相似推断这句话的概率, 但n-gram不能


# 深度语言模型的发展
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
  在向量空间上连续地表示词。比如稠密的词嵌入向量表示, 与之相反的是稀疏离散的独热表示
distributional
  Meaning is represented by contexts of use. Word2vec is distributional, but so are count-based word vectors, as we use the contexts of the word to model the meaning.
  通过单词的上下文(在文本中前后几个单词)来建模语义信息。比如word2vec和基于计数的词向量

完整的是 distributed representation(分布式表示) & distributional model(分布模型)

localist representation: 与分布式表示相反
@ref https://www.districtdatalabs.com/nlp-research-lab-part-1-distributed-representations
这篇文章介绍了1-hot思想的来源。1-hot每位表示一个概念(单词或名词短语), ‘0’表示对该概念取反或不存在于当前语境中
  assign one of NN's units to the representation of this concept.
  distributed representation is mainly driven by the fact that the representation of any single concept is distributed over many, if not all, processing units.
  distributed: 一个概念的表示是分配到多个unit. continuous values instead of discrete 1 and 0.


# A neural probabilistic language model
第一篇NN语言模型. by Bengio
考虑对有 10w个单词的10-gram 建立联合分布模型, 则有 10w ^ 10 = 1e50 个自由参数
本文将考虑: 比tri-gram更多的上下文词, 词之间的相似性
使用 分布式表示 解决维度灾难
词向量即词的特征向量。NN不仅能最优化隐藏函数, 还能自动学得特征
设词表V, 词表大小|V|, 词向量维度m
映射C: 从词表任一元素i 到 实向量C(i)∈R^m。实践中, C表示成 |V|×m 的自由参数矩阵


# word2vec
word2vec学到的语义信息
  vecDiff(beijing,chain) vecDiff(tokyo,japan) vecDiff(Moscow,Russia) 相近
  vecDiff(given,give,gave) vecDiff(taken,take,took) 相近
这些语义信息用调优后的经典矩阵分解(SVD、LSA)也能学到

用于计算word vector的模型有 CBOW(连续词袋) skip-gram


# transformer


# BERT
https://zhuanlan.zhihu.com/p/51413773