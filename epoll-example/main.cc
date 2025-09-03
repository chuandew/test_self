#include "simple_event_loop.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    SimpleEventLoop event_loop;
    
    // 启动事件循环
    event_loop.start();
    
    // 示例1：提交即时任务
    event_loop.submitTask([]() {
        std::cout << "即时任务执行，线程: " << std::this_thread::get_id() << std::endl;
    });
    
    // 示例2：提交定时任务
    event_loop.scheduleTask([]() {
        std::cout << "定时任务执行，线程: " << std::this_thread::get_id() << std::endl;
    }, 2000);  // 2秒后执行
    
    // 示例3：模拟网络服务器
    int mock_socket = 0; // 实际中应该是真实的socket
    
    // 注册读取事件回调
    event_loop.registerFd(mock_socket, EventType::READABLE, 
        [](int fd, EventType events) {
            (void) events; // 避免未使用警告
            std::cout << "收到读取事件 fd: " << fd 
                      << ", 线程: " << std::this_thread::get_id() << std::endl;
            // 这里处理数据读取...
        });
    
    // 模拟其他线程提交任务
    std::thread worker_thread([&event_loop]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        for (int i = 0; i < 3; ++i) {
            event_loop.submitTask([i]() {
                std::cout << "工作线程提交的任务 " << i 
                          << ", 线程: " << std::this_thread::get_id() << std::endl;
            });
        }
    });
    
    // 运行一段时间后停止
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    event_loop.stop();
    worker_thread.join();
    
    std::cout << "主线程结束: " << std::this_thread::get_id() << std::endl;
    return 0;
}
