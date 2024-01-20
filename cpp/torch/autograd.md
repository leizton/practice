~~~py
# site-packages/torch/autograd/__init__.py
def torch.autograd.backward():
  tensors = (loss)
  grad_tensors_= tuple(None,)   # 没有额外tensor需要算梯度
  grad_tensors_ = (tensor(1),)  # _make_grads(outputs=tensors, grads=grad_tensors_, is_grads_batched=False)
  Variable._execution_engine.run_backward(
    tensors, grad_tensors_, keep_graph=False, create_graph=False, inputs=(), allow_unreachable=True, accumulate_grad=True)

# site-packages/torch/autograd/variable.py
class Variable(torch._C._LegacyVariableBase, metaclass=VariableMeta):  # type: ignore[misc]
    _execution_engine = ImperativeEngine()

# site-packages/torch/_C/__init__.pyi
class _ImperativeEngine: # Defined in torch/csrc/autograd/python_engine.cpp
  pass
~~~

~~~c++
// torch/csrc/autograd/python_engine.cpp
THPEngine_run_backward(tensors, grad_tensors, keep_graph, create_graph,
                       inputs, allow_unreachable, accumulate_grad) {  // THP torch python
  // inputs: 指定计算哪些tensor的grad, 如果empty则计算图上所有的
  vector<Edge> roots;
  vector<Tensor> grads;
  for i in range(len(tensors)) {
    auto& t = tensors[i];
    Edge grad_edge(t.grad_fn(), t.output_nr());  // output_nr:output_number
    roots += [grad_edge];
    grads += [grad_tensors[i]];
  }
  //
  static PythonEngine engine;
  vector<Edge> output_edges;
  vector<Tensor> outputs = engine.execute(roots, inputs=grads, keep_graph, create_graph, accumulate_grad, output_edges);
}

// torch/csrc/autograd/engine.cpp
Engine::execute(root_edges, inputs, keep_graph=false, create_graph=false, accumulate_grad=true, outputs) {
  // 计算图的根节点 起始节点
  c10::SmallVector<Node*, 4> temp_roots;
  for Edge& e : root_edges {
    temp_roots += [e.function.get()];  // Edge.function是shared_ptr<Node>
  }
  //
  auto graph_task = make_shared<GraphTask>(reentrant_depth_=0, graph_roots_=temp_roots);
  auto graph_root = make_shared<GraphRoot>(root_edges, inputs);
  auto min_topo_nr = compute_min_topological_nr(outputs);
  compute_dependencies() {
    // 初始化每个节点依赖的前驱节点数
    // dependencies_:unordered_map<Node*, int>
    auto& dep = graph_task->dependencies_;
    // 深度优先遍历
    vector<Node*> sk{graph_root.get()};
    while !sk.empty() {
      Node* fn = sk.back();
      sk.pop_back();
      for edge : fn->next_edges()
        auto next = edge.function.get()
        dep[next] += 1;
        if graph_task->nodes_in_graph_.insert(next).second
          sk.push_back(next);
    }
  }
  //
  this->execute_with_graph_task(graph_task, graph_root, InputBuffer(variable_list()));
  //
  auto& fut = graph_task->future_result_;
  fut->wait();
  return fut->value().toTensorVector();
}

Engine::execute_with_graph_task(graph_task, graph_root, input_buffer) {
  initialize_device_threads_pool() {
    c10::call_once(start_device_threads_flag_, &Engine::start_device_threads, this);
  }
  // 选择空闲的device. 默认是cpu
  auto queue = this->ready_queue(graph_task->cpu_ready_queue_, input_buffer.device());
  queue->push(NodeTask(graph_task, graph_root, input_buffer));
  this->thread_main(graph_task) {
    // 等graph_task结束. 同时可能会执行其他的graph_task 例如子图?
    while !graph_task->future_result_->completed() {
      NodeTask task = local_ready_queue->pop();
      shared_ptr<GraphTask> local_graph_task = task.base_.lock();
      this->evaluate_function(local_graph_task, *task.fn_, task.inputs_, local_graph_task->cpu_ready_queue_);
      --local_graph_task->outstanding_tasks_;
    }
  }
}

Engine::evaluate_function(graph_task, Node& fn, InputBuffer& inputs, shared_ptr<ReadyQueue>& cpu_ready_queue) {
  // run node
  auto outputs = call_function(graph_task, &fn, inputs) {
    for hook : fn.tensor_pre_hooks()
      inputs = (*hook)(inputs);
    for pair : fn.retains_grad_hooks()
      inputs = (*pair.second)(inputs);
    for pair : fn.pre_hooks()
      inputs = (*pair.second)(inputs);
    //
    outputs = fn(inputs);
    //
    for hook : fn.post_hooks()
      outputs = (*hook)(outputs, inputs);
    return outputs
  }
  if (!graph_task->keep_graph_) fn.release_variables();
  // 驱动 next
  auto num_outputs = outputs.size();
  for i : range(num_outputs) {
    auto& next = fn.next_edge(i);
    bool is_ready = false;
    auto& dep = graph_task->dependencies_;
    auto iter = dep.find(next.function.get())
    if iter != dep.end() && --iter->second == 0 {
      dep.erase(iter);
      is_ready = true;
    }
    if is_ready {
      auto queue = this->ready_queue(cpu_ready_queue, input_buffer.device());
      queue->push(NodeTask(graph_task, next.function, input_buffer));
    }
  }
}
~~~

# 以 ExpBackward0 举例
tensor的grad_fn 就是 GraphTask的Node
~~~c++
struct Node {
  variable_list operator()(variable_list&& inputs) {
    return apply(inputs);
  }
  virtual variable_list apply(variable_list&& inputs) = 0;
};

struct ExpBackward0 : public Node {
  variable_list apply(variable_list&& grad_outputs) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto result = result_.unpack(this);
    variable_list grad_inputs(1);
    // result.conj(): 如果result是复数(is_complex)则返回复共轭, 否则返回自身
    // grad_outputs: 输出张量关于loss的梯度. exp()的梯度就是自身的输出, 所以直接乘result
    // grad_inputs: 输入张量关于loss的梯度
    grad_inputs[0] = grad_outputs[0] * result.conj();
    return grad_inputs;
  }
};
~~~