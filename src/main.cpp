#include <iostream>
#include "ThreadPool.hpp"
#include <chrono>
#include <cstdlib> 
#include <getopt.h>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include "ping.hpp"
#include <boost/asio.hpp>
#include <fstream>
#include <atomic>

std::atomic_int atomic_fails;

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
    if (result.find("100.0% packet loss") != std::string::npos) {
      ++atomic_fails;
    }

    // std::cout << ans[i] << std::endl;
}

void handlePing(const boost::system::error_code& error, int i, const std::string& result, std::vector<std::string>& ans) {
    if (!error) {
        ans[i] = result;
        if (result.find("100.0% packet loss") != std::string::npos) {
            ++atomic_fails;
        }
        // You can use ans[i] here if needed
    } else {
        std::cerr << "Error in asynchronous ping: " << error.message() << std::endl;
    }
}

// void asyncPing(boost::asio::io_service& io_service, const std::string& request, int i, std::vector<std::string>& ans) {
    
// }

// void callPingPopen(std::string request, int i, std::vector<std::string>& ans) {
//     boost::asio::io_service io_service;
//     asyncPing(io_service, request, i, ans);
//     io_service.run();
// }

/**
 * @brief Performs a sequential implementation of calling ping Popen to each request. Stores the answers in results
 * 
 * @param requests a reference to the list of websites to visit
 * @param results  a reference to the list of results to record in
 */
void do_sequential(std::vector<std::string> &requests, 
  std::vector<std::string> &results) {
  int n = requests.size();
  for(int i = 0; i < n; i++) {
    callPingPopen(requests[i], i, std::ref(results));
  }
}

/**
 * @brief Performs my threadpool implementation with PingOpen. Inefficient due to PingOpen calling fgets, which blocks all running threads
 * 
 * @param n_threads 
 * @param requests a reference to the list of websites to visit
 * @param results  a reference to the list of results to record in
 */
void do_my_threadpool(int n_threads, std::vector<std::string> &requests, 
  std::vector<std::string> &results) {
  ThreadPool tb(n_threads); // Use the appropriate constructor arguments
  int n = requests.size();
  for(int i = 0; i < n; i++) {
    tb.submit(callPingPopen, requests[i], i, std::ref(results));
  }
  tb.close();
} 

/**
* @brief Performs boost threadpool implementation with PingOpen. Inefficient due to PingOpen calling fgets, which blocks all running threads
 * @param n_threads Number of threads to spawn
 * @param requests a reference to the list of websites to visit
 * @param results  a reference to the list of results to record in
 */
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
  // printf("Max threads capable: %d\n", max_threads);

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
            break;
        default:
            // printf("Invalid input in command line\n");
            break;
    }
  }

  // printf("Num threads: %d\n", n_threads);
  // printf("Personal mode: %s\n", personal_mode ? "true" : "false");

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
  // printf("Num websites: %lu\n", websites.size());
  std::vector<std::string> results(websites.size());
  printf("Here!\n");
  boost::asio::io_context io_context;
  boost::asio::io_context io_context2;

  // create a pinger for the website to only listen to 1 message
  pinger myPinger(io_context, "www.github.com", 1);
  pinger myPinger2(io_context2, "www.google.com", 1);

  printf("Running\n");
  io_context.run();
  io_context2.run();
  printf("Done\n");
  // if (n_threads == 0) {
  //   // do sequential
  //   do_sequential(std::ref(websites), std::ref(results));
  // } else if (personal_mode) {
  //   // perform it with personal threadpool
  //   do_my_threadpool(n_threads, std::ref(websites), std::ref(results));
  // } else {
  //   // perform the test with boost threadpool
  //   do_c_threadpool(n_threads, std::ref(websites), std::ref(results));
  // }

  // for(int i = 0; i < results.size(); i++) {
  //   std::cout << results[i] << std::endl;
  // }
  std::cout << atomic_fails << std::endl;
  return 0;
}