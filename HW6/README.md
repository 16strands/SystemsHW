# Multithreading
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

Latencies for running multiple client threads with one server thread:
![sorry eitan](https://github.com/16strands/SystemsHW/blob/master/HW6/1%20server%20multiple%20client%20threads%20latencies.gif?raw=true)

Throughput for the same data:
![](https://github.com/16strands/SystemsHW/blob/master/HW6/1%20server%20multiple%20client%20threads%20throughput.gif?raw=true)

### The server already works in parallel because we have foresight

LAtencies for the server:
![check out these latencies, eitan](https://raw.githubusercontent.com/16strands/SystemsHW/master/HW6/2_client_threads.gif)

these are latency times for our server responding to 2 client threads. Super sick, if u ask me. 

throughput for the same data:
![Spicy](https://github.com/16strands/SystemsHW/blob/master/HW6/throughput%20for%202%20client%20threads.gif?raw=true)

These are the numbers we got when we took the times and divided them by the number of requests asked. These are how many requests happen per second.




eitan, I know you have sympathy for us, but, ariel here, Sarah and my dad tried to explain the difference between latency and throughput to me like 4 different times. I can't any more. If this is awful, we understand. We'll take the L. We spent soooo much time on this, please understand.
