class tbb_hash_compare<Key> {
  hash(Key& a) size_t static { tbb_hasher(a) }
  equal(Key& a, Key& b) bool static { a == b }
}

class hash_map_node_base {
  static_const() {
    rehash_req = reinterpret_cast<hash_map_node_base*>(size_t(3));
  }

  next   hash_map_node_base*
  mutex  spin_rw_mutex
}

class bucket {
  // 拉链法
  node_list  hash_map_node_base* volatile
  mutex      spin_rw_mutex
}

class hash_map_base {
  static_const() {
    pointers_per_table = sizeof(void*) * 8  // 等于一个segment的bit数
    embedded_block = 1
    embedded_buckets_n = (1<<embedded_block)
  }
  typedef() {
    segment_ptr_t = bucket*
  }

  /*
     idx:    {0 1}  {2 3}  {4 5 6 7}  {8 9 10 11 12 13 14 15}  {16 ...}
     seg_i:   0      1      2          3                        4

     假想 二维数组my_table 展成 连续的一维数组是 my_raw

     segment_base(seg_i) 计算第seg_i个段在 my_raw 展开成的起始下标(基地址)
     segment_idx(idx)    把 my_raw 的下标转成段索引
  */
  segment_base(size_t seg_i) { (1 << seg_i) & (~1) }  // 0=0 1=2 2=4 3=8 ...
  segment_size(size_t seg_i) { (1 << seg_i) }         // 0=1 1=2 2=4 3=8 ...

  /*
    __builtin_clz(x)
      作用：count x 的前置 0 bit 的个数
      __builtin_clzll(x) 用于 sizeof(x) 是 8 bytes
    segment_idx(i)
      等于 floor(log2(i))
      0=0 1=0 2=1 3=1 4=2 5=2 6=2 7=2 8=3 ...
  */
  segment_idx(size_t idx) {
    sizeof(idx)*8 - 1 - __builtin_clzll(idx|1)
  }
  segment_idx(int idx) {
    sizeof(idx)*8 - 1 - __builtin_clz(idx|1)
  }

  // segment 和 bucket 是同一个东西

  fields() {
    my_table             segment_ptr_t[pointers_per_table=64]  // 共64个seg，每段长度呈2的幂次增长
    my_embedded_segment  bucket[embedded_buckets_n]
    my_mask              atomic<size_t>
    //
    for i = 0:embedded_block
      my_table[i] = my_embedded_segment + segment_base(i)
    my_mask.store(embedded_buckets_n-1)
  }
}

class MapNode<T> : hash_map_node_base {
  data_ T*  // data_ is kv_pair
  storage() T* { data_ }
  value() T& { *storage() }
}

class concurrent_hash_map<Key, Value, HashCompare, Allocator> : hash_map_base {
  typedef() {
    pair_type = std::pair<Key, Value>
    node = MapNode<pair_type>

    node_allocator_type = tbb::internal::allocator_rebind<Allocator, node>::type
    node_allocator_traits = tbb::internal::allocator_traits<node_allocator_type>  // allocator_utils
  }

  fields() {
    my_allocator  node_allocator_type
    my_hash_cmp   HashCompare  // 两个功能：计算key的hash值, 判断key是否相等
  }

  concurrent_hash_map(std::initializer_list<pair_type> lst) {
    hash_map_base()
    internal_copy(lst.begin(), lst.end(), lst.size())
  }

  create_node<...Args>(Args&&... args) node* {
    node* pn = node_allocator_traits::allocate(my_allocator, 1)
    node_allocator_traits::construct(my_allocator, pn)
    node_allocator_traits::construct(my_allocator, pn->storage(), std::forward<Args>(args)...)
    return pn
  }

  delete_node(node* n) {
    node_allocator_traits::destory(my_allocator, n->storage())
    node_allocator_traits::destory(my_allocator, n)
    node_allocator_traits::deallocate(my_allocator, n, 1)
  }

  get_bucket(size_t hash) bucket* {
    size_t seg_i = segment_idx(hash)
    hash -= segment_base(seg_i)
    auto seg = my_table[seg_i]
    return &seg[hash]
  }

  // 在bucket里查找key
  search_bucket(const Key& key, bucket* b) node* {
    node* pn = (node*)b->node_list
    while pn && !my_hash_cmp.equal(key, pn->value().first)
      pn = (node*)pn->next
    return pn
  }

  // @param result: 新插入或已存在的节点的迭代器
  insert(const pair_type& kv, accessor& result) bool {
    const size_t hash = my_hash_cmp.hash(key)
    size_t mask = my_mask.load()
    bool is_inserted = false
    Key& key, Value* val = kv.first, kv.second
    size_t grow_segment_idx = 0

    while true {
      is_inserted = false
      bucket_accessor pb(this, hash & mask, writer=false)
      node* pn = search_bucket(key, pb.my_b)
      if pn, break
      if !pb.upgrade_to_writer() {
        // 此时有另一个线程在写这个bucket, 所以加写锁失败
        if pn = search_bucket(key, pb.my_b); pn
          pb.downgrade_to_reader()
          break
      }
      else {
        if check_mask_race(hash, &mask), continue
        pn = create_node(key, val)
        grow_segment_idx = insert_new_node(pb.my_b, pn, mask)
        is_inserted = true
        result.my_node = pb
        result.my_hash = hash
        break
      }
    }

    if grow_segment_idx, enable_segment(grow_segment_idx)
    return is_inserted
  }
}

// scoped_t析构时会release锁
class bucket_accessor : scoped_t {
  fields() {
    my_b(nullptr)  bucket*
  }

  bucket_accessor(concurrent_hash_map* hm, size_t hash, bool writer) {
    acquire(hm, hash, writer)
  }

  acquire(concurrent_hash_map* hm, size_t hash, bool writer) {
    my_b = hm.get_bucket(hash)
    if my_b->node_list == hash_map_node_base::rehash_req
       && try_acquire(my_b->mutex, write=true)               // 加写锁
       && my_b->node_list == hash_map_node_base::rehash_req  // double-check
      hm->rehash_bucket(my_b, hash)
    else
      acquire(my_b->mutex, writer)
  }

  is_writer() bool { my_b->mutex.is_w_locked }
}

// concurrent_hash_map的常量迭代器
class const_accessor<Value> : scoped_t {
  fields() {
    my_node(nullptr)  node*   // node: MapNode<Value>
    my_hash           size_t
  }

  operator*() const Value& { my_node->value() }
  operator->() const Value* { &operator*() }

  empty() bool { my_node == nullptr }

  // scoped_t析构时会被调用
  release() override {
    my_node->mutex.release()
    my_node = nullptr
  }

  is_writer() bool { my_node->mutex.is_w_locked }
}

// concurrent_hash_map的迭代器
class accessor<Value> : const_accessor<Value> {
  operator*() Value& { my_node->value() }
  operator->() Value* { &operator*() }
}
