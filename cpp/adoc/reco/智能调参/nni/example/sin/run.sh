#!/bin/bash

# 启动一次study
nnictl create --config sin/config.yaml

# 查看所有的exp, exp是study的一个实例
nnictl experiment list --all

# 查看某次实验的所有试验的结果
nnictl trial ls $exp_id

# 停止某个实验
nnictl stop $exp_id