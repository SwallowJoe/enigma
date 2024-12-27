# 设置库文件搜索路径
set(ENIGMA_PATH "${ROOT_DIR}/../../output")
link_directories(${ENIGMA_PATH})
# 设置库头文件目录
set(ENIGMA_HEADS_PATH "${ROOT_DIR}/../../enigma")

message("lib path:" ${ENIGMA_PATH})
message("lib head path:" ${ENIGMA_HEADS_PATH})

# build target echo_server
add_executable(${PROJECT_NAME} musicplayer_main.cpp)

# 添加公开头文件搜索路径，包括子目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${ENIGMA_HEADS_PATH}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    enigma 
    -lglfw
    -lpthread
)