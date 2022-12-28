#ifndef QUEUE_H
#define QUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#define QUEUE_SIZE 10

template <typename T>
class Queue {
 public:
  void Pop() {
    if (!queue_.empty()) {
      queue_.pop();
    }
  }

  T Front() {
    return queue_.front();
  }

  bool Wait() {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty()) {
      if (is_release) return false;
      cond_.wait(mlock);
    }

    if (is_release) return false;
    
    return true;
  }

  void Push(T item) {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.size() > QUEUE_SIZE) queue_.pop();
    queue_.push(item);
    mlock.unlock();
    cond_.notify_one();
  }

  T Back() {
    return queue_.back();
  }

  int GetSize() {
    return queue_.size();
  }
  Queue() = default;
  Queue(const Queue&) = delete;            // disable copying
  Queue& operator=(const Queue&) = delete; // disable assignment

  void Release() {
    is_release = true;

    std::unique_lock<std::mutex> mlock(mutex_);
    while (!queue_.empty()) {
      queue_.pop();
    }

    // mlock.unlock();

    cond_.notify_all();
  }

  void Restart() {
    is_release = false;
  }

 private:
  volatile bool is_release{false};
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

#endif
