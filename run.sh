#!/bin/bash

# 获取项目目录的绝对路径
PROJECT_DIR=$(pwd)

# 检查第一个参数
if [ "$1" = "test" ]; then
    # 测试模式：运行 autotest
    sudo docker run -it --rm -v "$PROJECT_DIR":/root/compiler maxxing/compiler-dev \
        autotest "$2" -s "$3" /root/compiler

elif [ "$1" = "koopa" ] || [ "$1" = "riscv" ]; then
    # 编译运行模式
    MODE=$1
    INPUT_FILE=${2:-"../hello.c"}
    
    if [ "$MODE" = "koopa" ]; then
        OUTPUT_FILE=${3:-"hello.koopa"}
        FLAG="-koopa"
    else
        OUTPUT_FILE=${3:-"hello.riscv"}
        FLAG="-riscv"
    fi

    echo "Running in $MODE mode..."
    echo "Input: $INPUT_FILE"
    echo "Output: $OUTPUT_FILE"

    sudo docker run -it --rm -v "$PROJECT_DIR":/root/compiler maxxing/compiler-dev bash -c "
        cd compiler &&
        cmake -DCMAKE_BUILD_TYPE=Debug -B build > /dev/null &&
        cmake --build build > /dev/null &&
        cd build &&
        ./compiler $FLAG \"$INPUT_FILE\" -o \"$OUTPUT_FILE\" &&
        echo 'Compilation finished. Output:' &&
        cat \"$OUTPUT_FILE\"
    "

else  
    # 默认模式：编译并进入 shell
    sudo docker run -it --rm -v "$PROJECT_DIR":/root/compiler maxxing/compiler-dev bash -c "
        cd compiler &&
        cmake -DCMAKE_BUILD_TYPE=Debug -B build &&
        cmake --build build &&
        cd build &&
        bash
    "
fi