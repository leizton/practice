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
