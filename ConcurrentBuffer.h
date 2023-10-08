#include <iostream>
#include <deque>
#include <mutex>


template <typename T>
class ConcurrentBuffer
{
private:
  std::deque<T> dataDeque;
  std::mutex mutex;
public:
  ConcurrentBuffer<T>(); /* Constructor */
  T poll(); /* Tries to poll from the front of deque */
  void push(T item); /* Tries to push to the end of deque */
}; 