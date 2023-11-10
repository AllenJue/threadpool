#include <iostream>
#include "ThreadPool.hpp"
#include <chrono>
#include <cstdlib> 
#include <getopt.h>


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



int main(int argc, char** argv) {
  // ThreadPool tb(10); // Use the appropriate constructor arguments
  // int arr[100];
  // std::vector<std::string> pings;
  // pings.push_back("-c 1 8.8.8.8");
  // pings.push_back("-c 1 www.google.com");
  // pings.push_back("-c 1 www.github.com");

  // std::vector<std::string> results(pings.size());
  // for(int i = 0; i < pings.size(); i++) {
  //   std::cout << pings[i] << std::endl;
  //   tb.submit(callPingPopen, pings[i], i, std::ref(results));
  // }
  // tb.close();
  // for(int i = 0; i < results.size(); i++) {
  //   std::cout << "ping: " << i << std::endl;
  //   std::cout << results[i] << std::endl;
  // }
  // return 0;

  int option;
  int n_threads = 0;
  bool personal_mode = 0;
  while (true) {
    option = getopt(argc, argv, "n:m:"); 
    if(option == -1) {
      break;
    }
    switch (option) {
        case 'n':
            n_threads = atoi(optarg);
            // cout << "Threads: " << num_threads << endl;
            break;
        case 'm':
            personal_mode = atoi(optarg);
            // cout << "Threads: " << num_threads << endl;
            break;
        default:
            // printf("Invalid input in command line\n");
            break;
    }
  }

  if (n_threads == 1) {
    // do sequential
  } else if (personal_mode) {

  } else {

  }

  printf("Num threads: %d\n", n_threads);
  printf("Personal mode: %s\n", personal_mode ? "true" : "false");
  return 0;
}