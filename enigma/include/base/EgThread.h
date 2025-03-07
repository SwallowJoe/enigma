#pragma once

#include <thread>
#include <string>
#include <memory>
#include <functional>

class EgThread {
public:
    explicit EgThread(const std::string& name);
    ~EgThread();

    void start();
    void stop();

private:
    void mainLoop();

    const std::string mName;
    std::unique_ptr<std::thread> mThread;
};