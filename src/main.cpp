#include <iostream>
#include "ThreadPool.hpp"
#include <chrono>
#include <cstdlib> 
#include <getopt.h>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <fstream>

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
  // print hardware capabilities
  unsigned int max_threads = std::thread::hardware_concurrency();
  printf("Max threads capable: %d\n", max_threads);

  int option;
  int n_threads = 0;
  bool personal_mode = 0;
  std::string input_path;

  while (true) {
    option = getopt(argc, argv, "n:m:i:"); 
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
        case 'i':
            input_path = optarg;
        default:
            // printf("Invalid input in command line\n");
            break;
    }
  }

  printf("Num threads: %d\n", n_threads);
  printf("Personal mode: %s\n", personal_mode ? "true" : "false");

  // read in data to file
  std::ifstream inputFile(input_path);
  if (inputFile.fail()) {
    std::cout << "FAIL TO OPEN: " << input_path << std::endl;
    return 1;
  }

  int num_lines;
  inputFile >> num_lines;

  std::vector<std::string> websites;

  for(int i = 0; i < num_lines; i++) {
    std::string website;
    inputFile >> website;
    websites.push_back("-c 1 " + website);
    // std::cout << website << std::endl;
  }
  printf("Num websites: %d\n", websites.size());
  std::vector<std::string> results(websites.size());

  if (n_threads == 0) {
    // do sequential
    do_sequential(std::ref(websites), std::ref(results));
  } else if (personal_mode) {
    // perform it with personal threadpool
    do_my_threadpool(n_threads, std::ref(websites), std::ref(results));
  } else {
    // perform the test with boost threadpool
    do_c_threadpool(n_threads, std::ref(websites), std::ref(results));
  }
  return 0;
}