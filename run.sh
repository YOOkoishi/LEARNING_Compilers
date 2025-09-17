#!/bin/bash

# 获取项目目录的绝对路径
PROJECT_DIR=$(pwd)

# 检查第一个参数
if [ "$1" = "test" ]; then
    # 测试模式：运行 autotest
    sudo docker run -it --rm -v $PROJECT_DIR:/root/compiler maxxing/compiler-dev \
        autotest -koopa -s "$2" /root/compiler
else
    # 默认模式：进入交互式 bash
    sudo docker run -it --rm -v $PROJECT_DIR:/root/compiler maxxing/compiler-dev bash -c "
        cd compiler &&
        cmake -DCMAKE_BUILD_TYPE=Debug -B build &&
        cmake --build build &&
        cd build &&
        bash
    "
fi