#! /bin/bash

proto_src=./protobuf
proto_out=./protobuf/target

PROTOCC="protoc --cpp_out=$proto_out"

cd ..

rm -rf $proto_out
mkdir -p $proto_out

$PROTOCC $proto_src/goods.proto
$PROTOCC $proto_src/user.proto
$PROTOCC $proto_src/shop_item.proto
$PROTOCC $proto_src/shop.proto

cd $proto_src

proto_cpp_dir='./target/protobuf'
proto_cpp=" \
  $proto_cpp_dir/goods.pb.cc \
  $proto_cpp_dir/user.pb.cc \
  $proto_cpp_dir/shop_item.pb.cc \
  $proto_cpp_dir/shop.pb.cc "

rm -f a.out

if [ $# -lt 1 ]; then
  g++ -std=c++11 -I./target -g -O0 test.cc $proto_cpp -lpthread -lprotobuf
else
  g++ -std=c++11 -I./target -g -O0 $@ $proto_cpp -lpthread -lprotobuf
fi

if [ -f a.out ]; then
  ./a.out
fi
