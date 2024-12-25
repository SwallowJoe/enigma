
# 创建静态库
# add_library(${LIB_NAME} STATIC)

add_library(${LIB_NAME} SHARED)

# 定义源文件搜索路径

# 使用 GLOB_RECURSE 搜索所有 .cpp 文件
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")
target_sources(${LIB_NAME} PRIVATE ${SOURCES})

# 添加公开头文件搜索路径，包括子目录
target_include_directories(${LIB_NAME} PUBLIC
    ${SOURCE_DIR}
)