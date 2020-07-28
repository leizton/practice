class tbb_hash_compare<Key> {
  hash(Key& a):size_t static { tbb_hasher(a) }
  equal(Key& a, Key& b):bool static { a == b }
}

class NodeBase {
  mutex  spin_rw_mutex
  next   node_base*
}

class Bucket {
  // 拉链法
  node_list  NodeBase*
  mutex      spin_rw_mutex
}

class hash_map_base {
  static_const() {
    pointers_per_table = sizeof(void*) * 8  // 等于一个segment的bit数
    embedded_block = 1
    embedded_buckets_n = (1<<embedded_block)
  }
  typedef() {
    segment_ptr_t = Bucket*
  }

  segment_base(size_t seg_i) { (1 << seg_i) & (~1) }  // 0->0 1->2 2->4 3->8
  segment_size(size_t seg_i) { (1 << seg_i) }         // 0->1 1->2 2->4 3->8

  fields() {
    my_table             segment_ptr_t[pointers_per_table]
    my_embedded_segment  Bucket[embedded_buckets_n]
    //
    for i = 0:embedded_block
      my_table[i] = my_embedded_segment + segment_base(i)
  }
}

class MapNode<T> : NodeBase {
  data_ T*
  storage():T* { data_ }
  value():T& { *storage() }
}

class concurrent_hash_map<Key, T, HashCompare, Allocator> : hash_map_base {
  typedef() {
    pair_type = std::pair<Key, T>
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

  create_node<...Args>(Args&&... args):node* {
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

  // 在bucket里查找key
  search_bucket(const Key& key, Bucket* b):node* {
    auto* p = (node*)b->node_list
    while p && !my_hash_cmp.equal(key, p->value().first)
      p = (node*)p->next
    return p
  }

  insert(const pair_type& kv):bool {
    return lookup(true, kv.first, &kv.second, nullptr, false, &allocate_node_copy_construct)
  }

  lookup(bool op_insert, const Key& key, const T* val, const_accessor* result, bool write, node* tmp):bool {
  }
}
