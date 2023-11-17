# Name of the text file to save the results
input_file="/Users/allenjue/Downloads/Cs378-Concurrency/threadpool/top_websites.txt"

time_file="/Users/allenjue/Downloads/Cs378-Concurrency/threadpool/timing/c_timing.csv"

fail_file="/Users/allenjue/Downloads/Cs378-Concurrency/threadpool/timing/c_fail.csv"
# Clear the file if it already exists
> $time_file
> $fail_file

echo "threads, execTime" > $time_file
echo "threads, totalFails" > $fail_file


mode=0
num_threads=(1 2 4 8 16 32 64 128 256 512 1024)

for threads in "${num_threads[@]}"; do
  total_elapsed=0
  
  # Run the command 10 times
  for ((i=1; i<=10; i++)); do
    start_time=$(gdate +%s.%N)
    
    echo "$threads, $(sudo time ./threadpool -n $threads -m $mode -i $input_file)" >> $fail_file
    
    end_time=$(gdate +%s.%N)
    
    # Calculate elapsed time
    elapsed_time=$(echo "$end_time - $start_time" | bc)
    
    # Accumulate total elapsed time
    total_elapsed=$(echo "$total_elapsed + $elapsed_time" | bc)
  done

  # Calculate average elapsed time
  average_elapsed=$(echo "scale=9; $total_elapsed / 10" | bc)
  
  echo "Threads: $threads, Average Elapsed Time: $average_elapsed s"
  echo "$threads, $average_elapsed" >> $time_file

done

