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

We did a cool mathematical model to simulate the lengths of values for items in the cache. The paper said that, for ETC, the average value size was 150 with value sizes ranging from 1 to 5k. We used a boost library for skewed distribution to accurately replicate this.

### Total requests:

The total ratio of different kinds of requests was edited to make the hit rate stay consistent. We started off with 66% GET, 5% DEL, and 29% SET. This ratio should be edited in order to maintain hit rates. This is detailed more thoroughly in the hit rate section.


### GET requests:

We went all out for get requests. We emulated the exact distribution mentioned in the paper. 90% of the get requests were randomly assigned to the most popular 10% of unique keys. 9% of requests are for the next 40% of unique keys. The last 1% of total get requests were asking for 50% of unique keys.

We kept this consistent through all clients by naming the keys in the same manner. The most popular key is key1, and the least popular key is keyN where N is the number of keys.


### SET requests:

Since posts are updated no matter how popular they are, updates are completely randomized.


### Hit rate:

When running one client with one server, the GET requests have a hit rate of about 81% when running with a cache size of 10 million. When more clients are added (as is the case when measuring throughput), the hit rate drops. This happens with a del rate of 15%, a update rate of 29 percent, and a get rate of 66%.

As more clients are added, their individual hit rate drops. the first client will have a hit rate of 40% for example, and the client that was started less than a second after will have a lower hit rate of 30%. We observed this trend across any number of clients, up to 7. This happens because as more clients are added, more sets and dels are done as well. The number of keys are the same, and all the requests refer to the same set of keys, so more deletes cause trouble.

We think that frequency of GET requests has the biggest effect on hit rate. In a trial of 3 clients with all variables the same, the same hit rate was noted even when we decreased the DEL rate from 5% of total requests to totally eliminating them entirely.

|Num clients| Maxmem | get rate  |  hit rate  |
|-----------|--------|-----------|------------|
|1|10,000,000|       66%|         81%|
|2|10,000,000|66%|44%-31%|
|3|10,000,000|66% (DEL 5%)|15%-6%|
|3|10,000,000|66% (DEL 0%)|12%-6%|


### Throughput:

To test throughput, we tried to find how many requests from as many clients as possible we could make before requests/second flattened out. Each client would test 100,000 requests. We tried this with only get requests, and it flattened out at 6300 requests per second with 4 clients. This is probably because we had the lowest optimmization settings. When we tried to find the throughput for all variety of requests, 

##### Just GETS:

This was done with -O0.

|num clients (total number of requests)|avg time per client| throughput (requests per second)|
|----|----|----|
|1 (100k)| 20 sec |5,000|
|2 (200k)| 32 sec| 6,000|
|3 (300k) | 48 sec | 6,250|
|4 (400k) | 63 sec| 6,300|

##### All kinds of requests:

This was done with -O3.

|Num clients (total number of requests)| avg time it took for each client| |
