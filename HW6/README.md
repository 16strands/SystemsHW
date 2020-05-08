# Let's Network!
Sarah Strand and Ariel Flaster


For numbers similar to the etc in Eitan's paper, run with cache size 10,000,000. This results in a hit rate of about 81% on average.

`./server_cache -m 10,000,000`


### How to run:

make should compile things. Then, run ./cache_server in one terminal, and ./workload_test in another terminal. They should talk to one another! there are also some fancy command line options you can make use of:

for cache_server:

      -m    maxmem: sets your cache size 
      -s    server address: this is the ip address used in our endpoint for the listener
      -p    port: this is the port we listen on
      -t    threads: this is how many threads we use 👀👀
    
for baseline_latencies:

      -s    server address: this is the server we're going to listen to
      -p    port: this is the port we'll listen to   
      -t    threads: I wonder what this does 👀
      -tr   trials: how many trials of gets, updates, and dels there are

### Baseline Latencies

this is an updated version of HW5's baseline latencies that works with as many threads as you want it to. We added a new function, threaded_performance that creates a warm cache in the server, then makes threads each with its own cache that performs trials on the server. Each thread does trial number gets, updates, and dels. then, the times of each threads trials are printed to the terminal.

### The server already works in parallel because Sarah and I have foresight

:)
