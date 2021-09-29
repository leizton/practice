# 安装
准备好python3环境
pip3 install -v nni==2.0

# example
search_space.json  定义搜参空间
config.yaml        实验配置
main.py            实验运行脚本

nnictl create --config example/sin/config.yaml
