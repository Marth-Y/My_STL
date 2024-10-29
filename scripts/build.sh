#!/bin/bash
# 调试模式
# set -x
CODE_SOURCE_PATH='/home/luyimin/My_STL'
OUT_DIR='build'

clean() {
    echo "cleaning build directory..."
    rm -rf "$CODE_SOURCE_PATH/$OUT_DIR"
    echo "cleaning operation completed"
}

if [ "$1" == "clean" ];then
    clean
    exit 0
fi

if [ ! -d "$CODE_SOURCE_PATH/$OUT_DIR" ];then
    mkdir -p "$CODE_SOURCE_PATH/$OUT_DIR"
fi

cd "$CODE_SOURCE_PATH/$OUT_DIR"
cmake ..
make

