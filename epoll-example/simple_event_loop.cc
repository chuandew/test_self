#include "simple_event_loop.h"
#include <cstring>
#include <iostream>

// 构造函数 - 使用eventfd创建通知机制
SimpleEventLoop::SimpleEventLoop() : epoll_fd_(-1) {
  // 创建epoll实例
  epoll_fd_ = epoll_create1(0);
  if (epoll_fd_ == -1) {
    throw std::runtime_error("Failed to create epoll");
  }

  // 创建eventfd（AWS使用的方式）
  // EFD_CLOEXEC: exec时关闭，EFD_NONBLOCK: 非阻塞模式
  event_fd_ = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
  if (event_fd_ == -1) {
    close(epoll_fd_);
    throw std::runtime_error("Failed to create eventfd");
  }

  std::cout << "Created eventfd: " << event_fd_ << std::endl;

  // 将eventfd添加到epoll监控（边缘触发模式）
  struct epoll_event event;
  event.events = EPOLLIN | EPOLLET; // 读事件 + 边缘触发
  event.data.fd = event_fd_;        // 存储eventfd

  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, event_fd_, &event) == -1) {
    close(epoll_fd_);
    close(event_fd_);
    throw std::runtime_error("Failed to add eventfd to epoll");
  }
}

// 析构函数 - 清理资源
SimpleEventLoop::~SimpleEventLoop() {
  stop();
  if (epoll_fd_ != -1)
    close(epoll_fd_);
  if (event_fd_ != -1)
    close(event_fd_);
}

// 唤醒事件循环（通过eventfd）
void SimpleEventLoop::wakeupEventLoop() {
  uint64_t value = 1;
  ssize_t result = write(event_fd_, &value, sizeof(value));

  if (result == -1) {
    // 在非阻塞模式下，EAGAIN是正常的（eventfd已满）
    if (errno != EAGAIN) {
      std::cerr << "Failed to write to eventfd: " << strerror(errno)
                << std::endl;
    }
  }
}

bool SimpleEventLoop::start() {
  if (running_)
    return true;

  running_ = true;
  event_thread_ = std::thread(&SimpleEventLoop::runLoop, this);
  return true;
}

void SimpleEventLoop::stop() {
  if (!running_)
    return;

  running_ = false;

  // 写入eventfd唤醒事件循环线程
  wakeupEventLoop();

  if (event_thread_.joinable()) {
    event_thread_.join();
  }
}

void SimpleEventLoop::submitTask(TaskCallback task) {
  {
    std::lock_guard<std::mutex> lock(task_mutex_);
    task_queue_.push(std::move(task));
  }

  // 通过eventfd通知事件循环有新任务
  wakeupEventLoop();
}

void SimpleEventLoop::scheduleTask(TaskCallback task, int delay_ms) {
  auto now = std::chrono::steady_clock::now();
  auto execution_time = now + std::chrono::milliseconds(delay_ms);

  ScheduledTask scheduled_task{std::move(task), execution_time,
                               static_cast<int>(scheduled_tasks_.size() + 1)};

  {
    std::lock_guard<std::mutex> lock(scheduled_task_mutex_);
    scheduled_tasks_.push(std::move(scheduled_task));
  }

  // 通过eventfd通知事件循环有新的定时任务
  wakeupEventLoop();
}

bool SimpleEventLoop::registerFd(int fd, EventType events,
                                 IoEventCallback callback) {
  struct epoll_event event;
  event.events = EPOLLET; // 边缘触发

  if (events == EventType::READABLE) {
    event.events |= EPOLLIN;
  } else if (events == EventType::WRITABLE) {
    event.events |= EPOLLOUT;
  }

  event.data.fd = fd;

  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) == -1) {
    return false;
  }

  fd_callbacks_[fd] = std::move(callback);
  return true;
}

void SimpleEventLoop::unregisterFd(int fd) {
  epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
  fd_callbacks_.erase(fd);
}

void SimpleEventLoop::runLoop() {
  const int MAX_EVENTS = 10;
  struct epoll_event events[MAX_EVENTS];

  std::cout << "Event loop started in thread: " << std::this_thread::get_id()
            << ", using eventfd: " << event_fd_ << std::endl;

  while (running_) {
    int timeout_ms = 1000;

    {
      std::lock_guard<std::mutex> lock(scheduled_task_mutex_);
      if (!scheduled_tasks_.empty()) {
        auto now = std::chrono::steady_clock::now();
        auto next_task_time = scheduled_tasks_.top().execution_time;

        if (next_task_time <= now) {
          timeout_ms = 0;
        } else {
          auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(
              next_task_time - now);
          timeout_ms = static_cast<int>(delay.count());
        }
      }
    }

    // 等待事件（核心的epoll_wait调用）
    int num_events = epoll_wait(epoll_fd_, events, MAX_EVENTS, timeout_ms);

    if (num_events > 0) {
      processIoEvents(events, num_events);
    }

    processScheduledTasks();
    processTasks();
  }

  std::cout << "Event loop stopped" << std::endl;
}

void SimpleEventLoop::processIoEvents(epoll_event *events, int count) {
  for (int i = 0; i < count; ++i) {
    int fd = events[i].data.fd;
    uint32_t epoll_events = events[i].events;

    // 处理eventfd通知（AWS方式）
    if (fd == event_fd_) {
      // 读取eventfd的值（边缘触发模式必须读取所有数据）
      uint64_t value;
      ssize_t bytes_read;

      // 循环读取直到EAGAIN（非阻塞模式）
      do {
        bytes_read = read(event_fd_, &value, sizeof(value));
        if (bytes_read == sizeof(value)) {
          std::cout << "Received eventfd notification, value: " << value
                    << std::endl;
        } else if (bytes_read == -1) {
          if (errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cerr << "Failed to read from eventfd: " << strerror(errno)
                      << std::endl;
          }
          break;
        }
      } while (bytes_read > 0);

      continue;
    }

    // 处理普通文件描述符事件
    if (fd_callbacks_.find(fd) != fd_callbacks_.end()) {
      EventType events_type = EventType::ERROR;

      if (epoll_events & EPOLLIN) {
        events_type = EventType::READABLE;
      } else if (epoll_events & EPOLLOUT) {
        events_type = EventType::WRITABLE;
      }

      fd_callbacks_[fd](fd, events_type);
    }
  }
}

void SimpleEventLoop::processTasks() {
  std::queue<TaskCallback> local_queue;

  {
    std::lock_guard<std::mutex> lock(task_mutex_);
    local_queue.swap(task_queue_);
  }

  while (!local_queue.empty()) {
    auto task = std::move(local_queue.front());
    local_queue.pop();

    try {
      task();
    } catch (const std::exception &e) {
      std::cerr << "Task execution failed: " << e.what() << std::endl;
    }
  }
}

void SimpleEventLoop::processScheduledTasks() {
  auto now = std::chrono::steady_clock::now();
  std::vector<ScheduledTask> ready_tasks;

  {
    std::lock_guard<std::mutex> lock(scheduled_task_mutex_);

    while (!scheduled_tasks_.empty() &&
           scheduled_tasks_.top().execution_time <= now) {
      ready_tasks.push_back(std::move(scheduled_tasks_.top()));
      scheduled_tasks_.pop();
    }
  }

  for (auto &task : ready_tasks) {
    try {
      task.callback();
    } catch (const std::exception &e) {
      std::cerr << "Scheduled task execution failed: " << e.what() << std::endl;
    }
  }
}