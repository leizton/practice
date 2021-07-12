# CB召回
content-based、内容召回、标签召回
tag、category
倒排排序
  倒排排序的目标和rank是相同的, 比如rank是ctr/时长, 则倒排也是ctr/时长
  倒排主要用后验值, rank可以用先验值
    item的属性是先验值, 如item的category, 请求发生前就确定了
    item的ctr是后验值, 展现后才能确定
    比如user喜欢体育这个cate, rank可以把体育item排在前面, 体育就是排序的先验信息
    倒排因为没有user信息，主要用后验指标(如ctr)排序
  `指标被hack的问题`
    用后验指标就容易使倒排结果和rank有大的diff
    比如按ctr排后, 倒排头部是大量标题党, 倒排头部的rank分会偏低, 和rank结果不一致
  指标bias的问题
    比如用完播率排序, 头部是大量短视频, 可以用播放时长除视频时长归一化解决
触发key
  如: 最近点击多的分类; 最近几次点击的item
内容召回的优缺点
  优点
  1. 可以有效针对用户特定兴趣
  缺点
  1. 人工标记item属性和特征, 不全面/成本高, 模型依赖人工标记的好坏
  2. 只根据用户现有兴趣获得建议, 不能扩展用户兴趣

# CF
三类CF: user_based; item_based; model_based
model_based: 矩阵分解、关联算法、聚类算法、深度学习、图模型等