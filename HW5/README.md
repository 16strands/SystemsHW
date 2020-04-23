# Let's Network!
Sarah Strand and Ariel Flaster


For numbers similar to the etc in Eitan's paper, run with cache size 1767000. This results in a hit rate of about 81% on average.

`./server_cache -m 17670000`


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


### TCP Server

Our server uses our implementation of the cache from HW2. We used code from boost.org (https://www.boost.org/doc/libs/1_72_0/libs/beast/example/http/server/async/http_server_async.cpp to be exact) as a starting point for our project. We rewrote the code in handle_request so that requests access our cache data structure.

### TCP Client 

Our client uses a different implementation of the cache from HW2. Our implementation turns each function into a http request to the server. We took inspiration from Vinnie Falco (https://www.boost.org/doc/libs/develop/libs/beast/example/http/client/async/http_client_async.cpp). After looking at his work with beast for so long, his name is engraved in my memory. Thank you, Vinnie. 

