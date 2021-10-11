# 句法分析
句法分析
  句法结构分析(syntactic structure parsing), 生成短语结构树
  依存关系分析(dependency parsing), 生成依存树
  目前都从句法结构分析转向依存分析
  依存树中没有短语，因此与短语结构树相比节点数更少、树更清晰，且两者是等价的
3个任务
  分词 -> 词性标注 -> 依存分析
  这3个任务是联合起来同时完成
开源的句法分析器
  SyntaxNet  依存分析、词性标注，准确率最高
  spaCy      依存分析、词性标注、命名实体识别，准确率比最好的差距不到1%，但性能最好
  @ref https://www.jiqizhixin.com/articles/2016-05-14

# 依存分析
what's
  命名实体识别关心相邻的词是否能接在一起构成成分(广义上的mention)
  依存分析关心两个词关系(entity relation)，即使不相邻
两类词及其关系
  修饰词称作从属词(dependent)，被修饰词称作支配词(head)
  有向关系：head -> dep
4个约束公理
  由语言学家Robinson提出
  1. 有且只有一个词不从属其他词，即root
  2. root外的词都从属其他词
  3. 每个词最多从属一个词
  4. 如果a从属b，那么a b之间的c从属 a b 或 ab之间的词
依存标签(attachment label)
  nsubj      nominal subject     名词主语
  nsubjpass  passive nsubj       被动语态动词的名词主语
  dobj       direct object       直接宾语
  iobj       indirect object     间接宾语
  csubj      clausal subj        主语从句，主语是一个从句
  csubjpass  passive csubj
  ccomp      clausal complement  补语从句
  xcomp      open ccomp
  @ref
    依存标签的详细解释  http://universaldependencies.org/docsv1/en/dep/index.html
    Stanford依存手册    https://downloads.cs.stanford.edu/nlp/software/dependencies_manual.pdf
    spacy label schema  https://spacy.io/models/en#en_core_web_lg-labels
评价指标
  POS  part-of-speech
  LAS  label attachment score  标签正确数 / 总数
  UAS  unlabeled attachment    关系正确数 / 总数
转化成分类问题
  把两个词和上下文丢给分类器，第一个分类器输出是否有依存关系，第二个分类器输出是哪种依存关系
3种方法
  1. Transition-based 基于转移，最好的是Stack-LSTM
  2. Graph-based 基于图，最流行的Biaffine模型
  3. deep learning
  @ref 邱锡鹏教授的综述 https://zhuanlan.zhihu.com/p/67106791
extend
  语义分割

# PCFG
what's
  Probabilistic(Stochastic) Context-Free Grammar 随机上下文无关文法
  PCFG文法定义成一个4元组定义：G=(w, N, N¹, R)
    w      叶节点，一个word，句子中的最小单元(词)
    N      非叶节点，一个POS-tag，N¹是根节点
    Σ      包含一个或多个节点的节点组
    R      生成规则的集合，一条规则表示成 Nⁱ->Σ，例如 S->(NP VP)
    w_kl   [wₖ wₗ]区间上的叶节点
    N_kl   支配w_kl的非叶节点
  PCFG 就是在 CFG 基础上给每个规则赋予概率值
assumption
  1. 上下文无关性    P(N_kl->Σ | words outside w_kl) = P(N_kl->Σ)  即N_kl的生成规则不受w_kl外的词影响
  2. 祖先节点无关性  P(N_kl->Σ | ancestors of N_kl)  = P(N_kl->Σ)  即不受N_kl的祖先节点影响
  3. ∀k P(N_k(k+c) -> Σ) 相等
solution
  1. P(N->Σ) = P(Σ|N) = count(N->Σ) / count(N)，从语料库(treebank)统计, tree指语法生成树(短语结构树)
  2. 目标值 T_best：给定句子找最可能的语法生成树
      T_best = argmax{P(S|T);T} = argmax{P(T|S)/P(S);T} = argmax{P(T|S);T}
               (max likelihood)                           (max posterior)
lexicalized & non-lexicalized parse tree
  例句：Last week IBM bought Lotus
  nonlexicalized ：S->(NP,NP,VP); NP->(JJ->Last,NN->week); NP->NNP->IBM; VP->(VBD->bought,NP->NNP->Lotus).
     lexicalized ：S(bought)->(NP,NP,VP); NP(week)->(JJ->Last,NN->week); NP(IBM)->NNP->IBM;
  区别：lexicalized 给出了中心词(`head word`), the head word of a `constituent` 某个结构块的中心词
  lexicalized 使 count(N(特定词)->Σ) 很小，影响P(N->Σ)的计算
模型-1
  参考 @ref_1 3.1节
  Model-1 给出了从 PCFG 扩展到 lexicalized-PCFG 的方法
  L-PCFG 的所有规则可以表示成同一个形式：A(h)->Lₙ(lₙ)···L₁(l₁)·H(h)·R₁(r₁)···Rₘ(rₘ)
    A(h)是H(h)的前驱规则，h是中心词，L(l)是左规则，R(r)是右规则
    例如在 S(bought)->(NP(week),NP(IBM),VP(bought)) 中，S(bought)是A(h) VP(bought)是H(h) NP(IBM)是L₁(l₁)
  规则的概率计算
    P(A->LⁱHRʲ) = P(LⁱHRʲ|A) = P(H|A,h)·P(LⁱRʲ|A,H,h) = P(H|A,h)·Pⁱ(Lᵢ|A,H,h)·Pʲ(Rⱼ|A,H,h)
    方程(3)(4)的假设即上下文无关假设 => 可以只考虑深度优先
  dependenies not to cross a verb
    依赖关系不会跨过动词
ref
  1. Head-Driven Statistical Models for Natural Language Parsing
  2. Standford nlp PCFG

# 乔姆斯基范式 CNF
Chomsky normal form
  只包含两种形式的规则：A->BC 或 A->a，ABC是非叶节点，a是叶节点
  任意CFG可以转换成CNF

# CYK algorithm
what's
  由 Cocke Younger Kasami 三人于60年代分别独立提出，基于动规思想
ref
  1. "CYK Parsing over Distributed Representations" 3.1节
  2. https://blog.csdn.net/Chase1998/article/details/84504191

# todo
https://nlp.stanford.edu/software/nndep.html