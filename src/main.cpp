#include <iostream>
#include "ThreadPool.hpp"
#include <chrono>
#include <cstdlib> 


void test(int i, int arr[]) {
  arr[i] = 100; 
}

std::string bufferToString(char* buffer, int bufflen)
{
    std::string ret(buffer, bufflen);

    return ret;
}

void callPingPopen(std::string request, int i, std::vector<std::string>& ans) {
    char buffer[128];
    std::string result = ""; // Create a string to accumulate the results

    FILE* pipe = popen(("ping " + request).c_str(), "r"); // Execute 'ping' command
    if (!pipe) {
        perror("popen");
    }

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
      // printf("Buffer: %s\n", buffer); // Print the output
      result += buffer;
    }

    pclose(pipe);
    // printf("%s\n", result);
    // std::cout << result << std::endl;
    ans[i] = result;
    std::cout << ans[i] << std::endl;
}



int main() {
  ThreadPool tb(10); // Use the appropriate constructor arguments
  // int arr[100];
  std::vector<std::string> pings;
  pings.push_back("-c 1 8.8.8.8");
  pings.push_back("-c 1 www.google.com");
  pings.push_back("-c 1 www.github.com");

  std::vector<std::string> results(pings.size());
  for(int i = 0; i < pings.size(); i++) {
    std::cout << pings[i] << std::endl;
    tb.submit(callPingPopen, pings[i], i, std::ref(results));
  }
  tb.close();
  for(int i = 0; i < results.size(); i++) {
    std::cout << "ping: " << i << std::endl;
    std::cout << results[i] << std::endl;
  }
  return 0;
}