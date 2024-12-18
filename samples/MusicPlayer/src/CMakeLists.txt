# build target echo_server
add_executable(MusicPlayerDemo musicplayer_main.cpp)


target_link_libraries(
    ${PROJECT_NAME}
    -lglfw
)