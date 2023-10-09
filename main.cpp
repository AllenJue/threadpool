#include <iostream>
#include "ThreadPool.hpp"
#include <chrono>

using namespace std;

void test(int i, int arr[]) {
  arr[i] = 100;
}

int main() {
  ThreadPool tb(10); // Use the appropriate constructor arguments
  int arr[100];
  for(int i = 0; i < 100; i++) {
    tb.submit(test, i, arr);
  }
  // std::this_thread::sleep_for(std::chrono::seconds(2));

  tb.close();
  for (int i = 0; i < 100; i++) {
    std::printf("%d\n", i);
  }
  return 0;
}