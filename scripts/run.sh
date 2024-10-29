#!/bin/bash
# set -x
CODE_SOURCE_PATH='/home/luyimin/My_STL/'
BIN_FILE='MY_STL'


if [ "$1" == "gdb" ];then
    gdb "$CODE_SOURCE_PATH/build/$BIN_FILE"
else
    # ."$CODE_SOURCE_PATH/build/$BIN_FILE"
    ../build/MY_STL
fi

