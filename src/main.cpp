#include <iostream>
#include "ThreadPool.hpp"
#include <chrono>
#include <cstdlib> 
#include <getopt.h>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

/**
 * changes a char array buffer to a string 
*/
std::string bufferToString(char* buffer, int bufflen)
{
    std::string ret(buffer, bufflen);

    return ret;
}

/**
 * Has a request to a URL, index to ans array, and makes a ping request
 * The result is captured as a string and placed into ans
*/
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

/**
 * Performs a sequential implementation of calling ping Popen to each request
 * Stores the answers in results
*/
void do_sequential(std::vector<std::string> &requests, 
  std::vector<std::string> &results) {
  int n = requests.size();
  for(int i = 0; i < n; i++) {
    callPingPopen(requests[i], i, std::ref(results));
  }
}

void do_my_threadpool(int n_threads, std::vector<std::string> &requests, 
  std::vector<std::string> &results) {
  ThreadPool tb(n_threads); // Use the appropriate constructor arguments
  int n = requests.size();
  for(int i = 0; i < n; i++) {
    tb.submit(callPingPopen, requests[i], i, std::ref(results));
  }
  tb.close();
}

void do_c_threadpool(int n_threads, std::vector<std::string> &requests, 
  std::vector<std::string> &results) {
  // create the number of threads for thread pool
  boost::asio::thread_pool pool(n_threads); 
  int n = requests.size();
  // create a task that takes in relevant parameters
  for(int i = 0; i < n; i++) {
    boost::asio::post(pool, [&requests, &results, i]() {
      callPingPopen(requests[i], i, results);
    });  
  }
  // wait for job to finish
  pool.join();
}

int main(int argc, char** argv) {
  // ThreadPool tb(10); // Use the appropriate constructor arguments
  // int arr[100];
  std::vector<std::string> pings;
  pings.push_back("-c 1 8.8.8.8");
  pings.push_back("-c 1 www.google.com");
  pings.push_back("-c 1 www.github.com");
  unsigned int max_threads = std::thread::hardware_concurrency();

  printf("Max threads capable: %d\n", max_threads);
  std::vector<std::string> results(pings.size());

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
    do_sequential(std::ref(pings), std::ref(results));
  } else if (personal_mode) {
    do_my_threadpool(n_threads, std::ref(pings), std::ref(results));
  } else {
    do_c_threadpool(n_threads, std::ref(pings), std::ref(results));
  }

  printf("Num threads: %d\n", n_threads);
  printf("Personal mode: %s\n", personal_mode ? "true" : "false");
  return 0;
}