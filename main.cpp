#include <iostream>
#include <fstream>
#include <getopt.h>
#include <pthread.h>
#include "ConcurrentBuffer.h"

using namespace std;

int main() {
  ConcurrentBuffer<int> cb;
  cb.push(5);
  cb.push(6);
  printf("Hello, world!\n");
  printf("%d\n", cb.poll());
  printf("%d\n", cb.poll());
  printf("%d\n", cb.poll());

  return 0;
}