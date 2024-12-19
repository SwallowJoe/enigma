#!/bin/bash

# 定义颜色变量
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# 输出所有参数
#echo "All arguments:"
#for arg in "$@"; do
#    echo "$arg"
#done

# 检查 glxinfo 是否安装
if ! command -v glxinfo &> /dev/null; then
    echo -e "${YELLOW}glxinfo could not be found.${NC}"
    read -p "Do you want to install it? (Y/N): " choice
    case "$choice" in 
        [Yy]* ) 
            echo "Installing glxinfo..."
            sudo apt-get update && sudo apt-get install mesa-utils
            if [ $? -eq 0 ]; then
                echo "glxinfo installed successfully."
            else
                echo -e "${RED}Failed to install glxinfo.${NC}"
                exit 1
            fi
            ;;
        [Nn]* ) 
            echo -e "${RED}Exiting.${NC}"
            exit 1
            ;;
        * ) 
            echo "Invalid input. Please enter Y or N."
            exit 1
            ;;
    esac
fi

# 检查 vulkaninfo 是否安装
if ! command -v vulkaninfo &> /dev/null; then
    echo -e "${RED}vulkaninfo could not be found.${NC}"
    read -p "${YELLOW}Do you want to install it? (Y/N): ${NC}" choice
    case "$choice" in 
        [Yy]* ) 
            echo "Installing vulkaninfo..."
            sudo apt-get install vulkan-utils
            if [ $? -eq 0 ]; then
                echo -e "${YELLOW}vulkaninfo installed successfully.${NC}"
                vulkaninfo
            else
                echo -e "${RED}Failed to install vulkaninfo.${NC}"
                exit 1
            fi
            ;;
        [Nn]* ) 
            echo -e "${RED}Exiting.${NC}"
            exit 1
            ;;
        * ) 
            echo "Invalid input. Please enter Y or N."
            exit 1
            ;;
    esac
fi

# 检查 libglfw3 是否安装
pkg_list=$(dpkg -l | grep "libglfw3:")

if [ -z "$pkg_list" ]; then
    echo -e "${RED}libglfw3 is not installed.${NC}"
    read -p "Do you want to install it? (Y/N): " choice
    case "$choice" in 
        [Yy]* ) 
            echo "Installing libglfw3..."
            sudo apt-get update && sudo apt-get install libglfw3
            if [ $? -eq 0 ]; then
                echo -e "${GREEN}libglfw3 installed successfully.${NC}"
            else
                echo -e "${RED}Failed to install libglfw3.${NC}"
                exit 1
            fi
            ;;
        [Nn]* ) 
            echo -e "${RED}Exiting.${NC}"
            exit 1
            ;;
        * ) 
            echo "Invalid input. Please enter Y or N."
            exit 1
            ;;
    esac
fi

# 检查 libglfw3-dev 是否安装
pkg_list=$(dpkg -l | grep "libglfw3-dev")

if [ -z "$pkg_list" ]; then
    echo -e "${RED}libglfw3-dev is not installed.${NC}"
    read -p "Do you want to install it? (Y/N): " choice
    case "$choice" in 
        [Yy]* ) 
            echo "Installing libglfw3-dev..."
            sudo apt-get update && sudo apt-get install libglfw3-dev
            if [ $? -eq 0 ]; then
                echo -e "${GREEN}libglfw3-dev installed successfully.${NC}"
            else
                echo -e "${RED}Failed to install libglfw3-dev.${NC}"
                exit 1
            fi
            ;;
        [Nn]* ) 
            echo -e "${RED}Exiting.${NC}"
            exit 1
            ;;
        * ) 
            echo "Invalid input. Please enter Y or N."
            exit 1
            ;;
    esac
fi

# 提示用户选择
echo -e "${YELLOW}请选择操作：${NC}"
echo -e "0. 构建并执行 (*默认)"
echo -e "1. 仅构建"
echo -e "2. 清理项目"
echo -e "3. MusicPlayer测试"
echo -e "4. vulkaninfo"
echo -e "5. glxinfo"
echo -e "6. device info"

if [ $# -ge 1 ]; then
    choice=$1
    echo -e "${YELLOW}选择: $1 ${NC}"
else
    read -p "请输入数字选项: " choice
fi

# 如果用户没有输入，设置默认值为0
if [ -z "$choice" ]; then
    choice=0
fi

# 根据用户选择执行对应操作
case $choice in
    0)
        echo "构建并执行单元测试"
        ENIGMA_BUILD=true
        ENIGMA_RUN=true
        ;;
    1)
        echo "仅执行构建"
        ENIGMA_BUILD=true
        ;;
    2)
        echo "清理项目"
        ENIGMA_CLEAN=true
        ;;
    3)
        echo "MusicPlayer测试"
        TestMusicPlayer=true
        ;;
    4)
        echo -e "${YELLOW}vulkaninfo:${NC}"
        vulkaninfo
        ;;
    5)
        echo -e "${YELLOW}glxinfo:${NC}"
        glxinfo_output=$(glxinfo |grep -iE "OpenGL version")
        # 检查是否成功获取版本信息
        if [ -n "$glxinfo_output" ]; then
            OpenGLVersion="unknow"
            echo -e "OpenGL version: ${YELLOW}$glxinfo_output${NC}"
            if echo "$glxinfo_output" |grep -q "OpenGL version string: 4.5"; then
                OpenGLVersion="4.5"
            elif echo "$glxinfo_output" |grep -q "OpenGL version string: 4.0"; then
                OpenGLVersion="4.0"
            elif echo "$glxinfo_output" |grep -q "OpenGL version string: 3.2"; then
                OpenGLVersion="3.2"
            elif echo "$glxinfo_output" |grep -q "OpenGL version string: 3.1"; then
                OpenGLVersion="3.1"
            fi
        else
            echo -e "${RED}Failed to get OpenGL version!${NC}"
            exit 1
        fi
        ;;
    6)
        # 打印系统信息
        echo -e "${YELLOW}Printing system information:${NC}"
        echo -e "${YELLOW}CPU:${NC}"
        lscpu
        echo -e "${YELLOW}GPU:${NC}"
        gpuinfo=$(lshw -c video)
        echo -e "${gpuinfo}"
        echo -e "${YELLOW}RAM:${NC}"
        free -h
        ;;
    *)
        echo -e "${RED}无效选项，请输入合法选项.${NC}"
        exit 1
        ;;
esac

if [ "${ENIGMA_BUILD}" = true ];then
    # 获取当前脚本所在目录
    SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)

    # 检查并创建build目录
    if [ ! -d "build" ]; then
        mkdir build
    fi

    # 进入build目录
    cd build || exit
    # 执行cmake ..
    cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ .. || { echo -e "${RED}cmake failed${NC}"; exit 1; }

    # 执行make
    make || { echo -e "${RED}make failed${NC}"; exit 1; }
fi

if [ "${ENIGMA_CLEAN}" = true ];then
    # 获取当前脚本所在目录
    SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
    # 检查并创建build目录
    if [ ! -d "build" ]; then
        echo "clean finish."
    else
        # 进入build目录
        rm -rf build
        echo "clean finish."
    fi
fi

if [ "${TestMusicPlayer}" = true ];then
    # 获取当前脚本所在目录
    SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
    cd samples/MusicPlayer

    # 检查并创建build目录
    if [ ! -d "build" ]; then
        mkdir build
    fi

    # 进入build目录
    cd build || exit

    # 执行cmake ..
    cmake -DOpenGL_Version=$OpenGLVersion .. || { echo -e "${RED}cmake failed${NC}"; exit 1; }

    # 执行make
    make || { echo -e "${RED}make failed${NC}"; exit 1; }

    cd ..
    # 检查bin目录是否存在
    if [ ! -d "bin" ]; then
        echo -e "${RED}bin directory not found.${NC}"
        exit 1
    fi

    # 进入bin目录
    cd bin || exit
    # 检查可执行文件是否存在
    MISSING_FILES=""
    for file in "MusicPlayerDemo"; do
        if [ ! -f "$file" ]; then
            MISSING_FILES="$MISSING_FILES $file"
        fi
    done

    if [ -n "$MISSING_FILES" ]; then
        echo -e "${RED}Missing files: $MISSING_FILES${NC}"
    else
        echo -e "${YELLOW}*********************************************${NC}"
        echo -e "${YELLOW}run MusicPlayerDemo: ${NC}"
        ./MusicPlayerDemo
    fi
fi

