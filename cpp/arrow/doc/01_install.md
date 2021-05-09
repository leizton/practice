~~~shell

# 用 ubuntu 20.04 国内源更新

$ docker_mount add_arrow4 leizton/dev:0.8 $HOME/sky/arrow /arrow
$ docker exec -it $CONTAINER_ID bash

# install cmake
$ cd /tmp && wget https://github.com/Kitware/CMake/releases/download/v3.20.2/cmake-3.20.2.zip
$ cd cmake-3.20.2
    # sed -i '1i\set(CMAKE_USE_OPENSSL OFF)' CMakeLists.txt
    apt-get install libssl-dev
    ./boostrap
    make -j8
    make install
    cmake --version

# install arrow
$ cd /arrow/cpp
  cmake . \
      -DARROW_BUILD_SHARED=OFF \
      -DARROW_BUILD_STATIC=ON \
      -DARROW_COMPUTE=ON \
      -DARROW_CSV=ON \
      -DARROW_DATASET=ON \
      -DARROW_DEPENDENCY_USE_SHARED=OFF \
      -DARROW_FILESYSTEM=OFF \
      -DARROW_HDFS=OFF \
      -DARROW_JEMALLOC=OFF \
      -DARROW_JSON=OFF \
      -DARROW_ORC=OFF \
      -DARROW_PARQUET=OFF \
      -DARROW_PLASMA=OFF \
      -DARROW_WITH_BROTLI=OFF \
      -DARROW_WITH_BZ2=OFF \
      -DARROW_WITH_LZ4=OFF \
      -DARROW_WITH_SNAPPY=OFF \
      -DARROW_WITH_ZLIB=OFF \
      -DARROW_WITH_ZSTD=OFF
    make -j8
    make install
    exit

$ docker commit -a 'leizton@126.com' -m 'add arrow4' add_arrow4 leizton/dev:0.9.1
$ docker push leizton/dev:0.9.1

~~~