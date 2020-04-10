# Let's Network!
Sarah Strand and Ariel Flaster

This is our implementation of the TCP server and client. Please read the known bugs. We're sooo close. Please have mercy.

### How to run:

make should compile things. Then, run ./cache_server in one terminal, and ./test_cache_client in another terminal. They should talk to one another! there are also some fancy command line options you can make use of:

for cache_server:

      -m    maxmem: sets your cache size 
      -s    server address: this is the ip address used in our endpoint for the listener
      -p    port: this is the port we listen on
      -t    threads: this is how many threads we use 👀👀
    
    
    
for test_cache_client:

    -s    server address: this is the address of the server we will contact about our cache
    -p    port: this is the port that we will contact about our cache


all of the above arguments have default args, so feel free to just run our code as-is.


### TCP Server

Our server uses our implementation of the cache from HW2. We used code from boost.org (https://www.boost.org/doc/libs/1_72_0/libs/beast/example/http/server/async/http_server_async.cpp to be exact) as a starting point for our project. We rewrote the code in handle_request so that requests access our cache data structure.

### TCP Client 

Our client uses a different implementation of the cache from HW2. Our implementation turns each function into a http request to the server. We took inspiration from Vinnie Falco (https://www.boost.org/doc/libs/develop/libs/beast/example/http/client/async/http_client_async.cpp). After looking at his work with beast for so long, his name is engraved in my memory. Thank you, Vinnie. 


### KNOWN BUGS:

Put does some weird stuff. If you remove all calls to "my_cache->set" in the cache client tester, the tests will fail because the "my_cache->get"s all fail. This is good news though, because the code makes it to the assertions for test failures! 

We added print statements to show that the server does in fact receive requests from the client. I'm sure that what we're missing is a simple misusage of beast things, but we couldn't find enough documentation to fully excavate the problem.

If you remove all the calls to "my_cache->set" and still don't get the test failure assertions, you may need to try running on a different port.  

The only time the code works is when we don't PUT something to the server's cache. This causes tests to fail, but we can't figure out what the issue is on the server side with PUT. All we know is that the error is somewhere in PUT on the server side.
