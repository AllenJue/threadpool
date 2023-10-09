#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "ConcurrentBuffer.hpp"
#include <functional>
#include <vector>
#include <thread>

class ThreadPool {
private:
  ConcurrentBuffer<std::function<void()>> workQueue; /* Queue of functions */
  std::vector<std::thread> workers;
  bool complete;
  int activeTasks = 0;
public:
  /**
   * @brief Submits a type of function and binds the arguments to the workQueue
   * 
   * @tparam F 
   * @tparam Args 
   * @param f 
   * @param args 
   */
  template<typename F, typename... Args>
  void submit(F f, Args&&... args) {
    workQueue.push( std::bind(f, std::forward<Args>(args)...) );
  }

  ThreadPool(int threads); /* Constructs a thread pool with threads workers */
  void close(); /* Notifies all threads no new work. Joins threads */
  void worker();
  bool busy();
};

ThreadPool::ThreadPool(int threads) {
  complete = false;
  for (int i = 0; i < threads; ++i) {
    workers.emplace_back(&ThreadPool::worker, this);
  }
}

void ThreadPool::close() {
  workQueue.closeBuffer();
  for(auto& worker : workers) {
    if(worker.joinable()) {
      worker.join();
    }
  }
  workers.clear();
}

void ThreadPool::worker() {
  std::function<void()> work;
  // while work is not null
  while((work = workQueue.poll())) {
    // std::cout << std::this_thread::get_id() << std::endl;
    work();
  }
}

bool ThreadPool::busy() {
  return !workQueue.isEmpty();
}

#endif