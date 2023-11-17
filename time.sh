# Name of the text file to save the results
input_file="/Users/allenjue/Downloads/Cs378-Concurrency/threadpool/top_websites.txt"

time_file="/Users/allenjue/Downloads/Cs378-Concurrency/threadpool/seq_timing.csv"

fail_file="/Users/allenjue/Downloads/Cs378-Concurrency/threadpool/seq_fail.csv"
# Clear the file if it already exists
> $time_file
> $fail_file

echo "threads, execTime" > $time_file
echo "threads, totalFails" > $fail_file

mode=1
num_threads=(1 2 4 8 16 32 64 128 256 512 1024)

for threads in "${num_threads[@]}"; do
  start_time=$(date +%s.%N)  
  ./threadpool -n $threads -m $mode -i $input_file
  end_time=$(date +%s.%N) 

  cur_time=$(echo "$end_time - $start_time" | bc -l)
  echo "$threads, $cur_time" >> $time_file
done
