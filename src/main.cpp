#include <iostream>
#include "ThreadPool.hpp"
#include <chrono>
#include <cstdlib> 


void test(int i, int arr[]) {
  arr[i] = 100; 
}

void callPingPopen(std::string request) {
    char buffer[128];
    FILE* pipe = popen(("ping -c 4 " + request).c_str(), "r"); // Execute 'ping' command
    if (!pipe) {
        perror("popen");
    }

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        printf("%s\n", buffer); // Print the output
    }

    pclose(pipe);

}

int main() {
  ThreadPool tb(10); // Use the appropriate constructor arguments
  // int arr[100];
  std::vector<std::string> pings;
  pings.push_back("-c1 -s1 8.8.8.8");
  pings.push_back("-c1 -s1 www.google.com");
  pings.push_back("-c1 -s1 www.github.com");

  for(int i = 0; i < pings.size(); i++) {
    std::cout << pings[i] << std::endl;
    tb.submit(callPingPopen, pings[i]);
  }
  // int x = std::system("ping -c1 -s1 8.8.8.8");
  // if (x==0){
  //     std::cout<<"success" << std::endl;
  // } else{
  //     std::cout<<"failed" << std::endl;
  // }  
// for(int i = 0; i < 100; i++) {
  //   tb.submit(test, i, arr);
  // }
  // std::this_thread::sleep_for(std::chrono::seconds(2));

  tb.close();
  // for (int i = 0; i < 100; i++) {
  //   std::printf("%d %d\n", i, arr[i]);
  // }
  return 0;
}