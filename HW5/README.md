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
    
for workload_test:

      -s    server address: this is the server we're going to listen to
      -p    port: this is the port we'll listen to   

make sure to use cache_server with a maxmem arg of 10,000,000 for the hit rate to replicate the paper!

### Making the warm cache:

We did a cool mathematical model to simulate the lengths of values for items in the cache. The paper said that, for ETC, the average value size was 150 with value sizes ranging from 0 to 5k. We used a boost library for skewed distribution to accurately replicate this.

### Total requests:

The total ratio of different kinds of requests was edited to make the hit rate stay consistent. We started off with 66% GET, 5% DEL, and 29% SET.


### GET requests:

We went all out for get requests. We emulated the exact distribution mentioned in the paper. 90% of the get requests were randomly assigned to the most popular 10% of unique keys. 9% of requests are for the next 40% of unique keys. The last 1% of total get requests were asking for 50% of unique keys.

We kept this consistent through all clients by naming the keys in the same manner. The most popular key is key1, and the least popular key is keyN where N is the number of keys.


### SET requests:

Since posts are updated no matter how popular they are, updates are completely randomized.


### Hit rate:

When running one client with one server, the GET requests have a hit rate of about 81% when running with a cache size of 10 million. When more clients are added (as is the case when measuring throughput), the hit rate drops. This happens with a del rate of 15%, a update rate of 29 percent, and a get rate of 66%.



|Num clients| Maxmem | get rate  |  hit rate  |
|-----------|--------|-----------|------------|
|1|10,000,000|       66%|         81%|
|2|10,000,000|66%|%-|


### Throughput:

When 
