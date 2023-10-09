#ifndef CONCURRENTBUFFER_H
#define CONCURRENTBUFFER_H

#include <iostream>
#include <deque>
#include <mutex>
#include <condition_variable>

template <typename T>
class ConcurrentBuffer {
private:
  std::deque<T> dataDeque;
  std::condition_variable empty;
  std::mutex mutex;
  bool close = false;
public:
  /**
   * @brief Construct a new Concurrent Buffer< T>:: Concurrent Buffer object
   *  
   * @tparam T type of objects that the buffer will hold
   */
  ConcurrentBuffer() {}

  /**
   * @brief Polls an object from the front, returning null if empty and closed
   * 
   * @tparam T type of object to return
   * @return T object in the front of the buffer
   */
  T poll() {
    std::unique_lock<std::mutex> lock(mutex);
    while(dataDeque.empty() && !close) {
      empty.wait(lock);
    }
    std::cout << "Polling\n";

    if(!dataDeque.empty()) {
      T value = dataDeque.front();
      dataDeque.pop_front();
      return value;
    }

    std::cout << "Closing\n";
    return NULL;
  }

  /**
   * @brief Pushes an item to end the of the buffer
   * 
   * @tparam T Type of item to push
   * @param item item to insert
   */
  void push(T item) {
    std::unique_lock<std::mutex> lock(mutex);
    dataDeque.push_back(item);
    empty.notify_one();
  }

  bool isEmpty() {
    std::unique_lock<std::mutex> lock(mutex);
    return dataDeque.empty();
  }

  void closeBuffer() {
    close = true;
    empty.notify_all();
  }
}; 



#endif