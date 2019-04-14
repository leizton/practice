#! /bin/bash

curr_dir=`pwd`
export GOPATH=$GOPATH:$curr_dir

go run main.go
