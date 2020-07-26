class tbb_hash_compare<Key> {
  hash(Key& a):size_t static { tbb_hasher(a) }
  equal(Key& a, Key& b):bool static { a == b }
}

class node_base {
  mutex  spin_rw_mutex
  next   node_base*
}

class bucket {
  node_list  node_base*
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

  segment_base(size_t seg_i) { (1 << seg_i) & (~1) }  // 0->0 1->2 2->4 3->8
  segment_size(size_t seg_i) { (1 << seg_i) }         // 0->1 1->2 2->4 3->8

  fields() {
    my_table             segment_ptr_t[pointers_per_table]
    my_embedded_segment  bucket[embedded_buckets_n]
    //
    for i = 0:embedded_block
      my_table[i] = my_embedded_segment + segment_base(i)
  }
}

class node<T> : node_base {
  val_ T*
  storage():T* { val_ }
  value():T& { *storage() }
}

class concurrent_hash_map<Key, T, HashCompare, Allocator> : hash_map_base {
  typedef() {
    node_allocator_type = tbb::internal::allocator_rebind<Allocator, node>::type
    node_allocator_traits = tbb::internal::allocator_traits<node_allocator_type>
  }

  fields() {
    my_allocator  node_allocator_type
  }

  delete_node(node* n) {
    node_allocator_traits::destory(my_allocator, n->storage())
    node_allocator_traits::destory(my_allocator, n)
    node_allocator_traits::deallocate(my_allocator, n, 1)
  }
}
