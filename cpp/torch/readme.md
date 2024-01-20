# 生成code
~~~sh
#cmakefile里生成code的命令如下
set(GEN_COMMAND
    "${PYTHON_EXECUTABLE}" -m torchgen.gen  # 即torchgen目录下的gen.py文件
      --source-path ${CMAKE_CURRENT_LIST_DIR}/../aten/src/ATen
      --install_dir ${CMAKE_BINARY_DIR}/aten/src/ATen
      ${GEN_PER_OPERATOR_FLAG}
      ${GEN_ROCM_FLAG}
      ${GEN_MPS_FLAG}
      ${CUSTOM_BUILD_FLAGS}
)

# 也就是执行
python torchgen/gen.py --source-path aten/src/ATen --install_dir aten/src/ATen
~~~

# 各类所在的文件
GraphTask                torch/csrc/autograd/graph_task.h
GraphTask.Edge           torch/csrc/autograd/edge.h
GraphTask.Edge.Node      torch/csrc/autograd/function.h
