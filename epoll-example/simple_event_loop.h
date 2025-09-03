#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>
#include <queue>
#include <sys/epoll.h>
#include <sys/eventfd.h> // 添加eventfd头文件
#include <thread>
#include <unistd.h>
#include <vector>

// 任务回调类型
using TaskCallback = std::function<void()>;

// 事件类型
enum class EventType { READABLE = 1, WRITABLE = 2, ERROR = 4 };

// I/O 事件回调
using IoEventCallback = std::function<void(int fd, EventType events)>;

// 定时任务
struct ScheduledTask {
  TaskCallback callback;
  std::chrono::steady_clock::time_point execution_time;
  int id;

  bool operator>(const ScheduledTask &other) const {
    return execution_time > other.execution_time;
  }
};

// 简单事件循环类（使用eventfd）
class SimpleEventLoop {
public:
  SimpleEventLoop();
  ~SimpleEventLoop();

  // 启动事件循环
  bool start();

  // 停止事件循环
  void stop();

  // 提交即时任务
  void submitTask(TaskCallback task);

  // 提交定时任务
  void scheduleTask(TaskCallback task, int delay_ms);

  // 注册文件描述符监听
  bool registerFd(int fd, EventType events, IoEventCallback callback);

  // 取消注册文件描述符
  void unregisterFd(int fd);

private:
  void runLoop();
  void processIoEvents(epoll_event *events, int count);
  void processTasks();
  void processScheduledTasks();

  // 唤醒事件循环（使用eventfd）
  void wakeupEventLoop();

  // epoll 相关
  int epoll_fd_;
  std::unordered_map<int, IoEventCallback> fd_callbacks_;

  // 任务队列
  std::queue<TaskCallback> task_queue_;
  std::mutex task_mutex_;

  // 定时任务队列
  std::priority_queue<ScheduledTask, std::vector<ScheduledTask>,
                      std::greater<ScheduledTask>>
      scheduled_tasks_;
  std::mutex scheduled_task_mutex_;

  // 线程控制
  std::thread event_thread_;
  std::atomic<bool> running_{false};

  // 使用eventfd代替管道（AWS方式）
  int event_fd_{-1};
};