
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <iostream>
#include "gelf.h"

#include "include/core/EgPoint.h"
#include "include/core/EgRect.h"

int main(int argc, char const *argv[])
{
    EgPoint p{3, 4};
    EgRect rect = EgRect::MakeWH(800, 600);

    std::cout<<"Music Player running..."<<p.length()<<std::endl;
     // 初始化 GLFW 库
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // 设置创建窗口所需的配置
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 指定 OpenGL 版本为 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 指定使用核心模式

    // 创建窗口对象
    GLFWwindow* window = glfwCreateWindow(800, 600, "MusicPlayer Demo", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // 设置当前窗口的上下文为活动上下文
    glfwMakeContextCurrent(window);

    // 循环，直到用户关闭窗口
    while (!glfwWindowShouldClose(window)) {
        // 渲染代码将放在这里
        // TODO: renderFrame()

        // 交换前后缓冲区
        glfwSwapBuffers(window);

        // 处理事件
        glfwPollEvents();
    }

    // 销毁窗口并退出
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
