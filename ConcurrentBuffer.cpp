#include "ConcurrentBuffer.h"

/**
 * @brief Construct a new Concurrent Buffer< T>:: Concurrent Buffer object
 * 
 * @tparam T type of objects that the buffer will hold
 */
template <typename T>
ConcurrentBuffer<T>::ConcurrentBuffer() {}

/**
 * @brief Polls an object from the front, returning null if empty
 * 
 * @tparam T type of object to return
 * @return T object in the front of the buffer
 */
template <typename T>
T ConcurrentBuffer<T>::poll() {
  std::lock_guard<std::mutex> lock(mutex); // Lock the mutex
  if(dataDeque.empty()) {
    return NULL;
  }
  return dataDeque.pop_front();
}

/**
 * @brief Pushes an item to end the of the buffer
 * 
 * @tparam T Type of item to push
 * @param item item to insert
 */
template <typename T>
void ConcurrentBuffer<T>::push(T item) {
  std::lock_guard<std::mutex> lock(mutex); // Lock the mutex
  dataDeque.push_back(item);
}