CXX=g++
CXXFLAGS= -std=c++17 -Werror -Wall -Wextra -O3
LDFLAGS=$(CXXFLAGS)
LIBS=-pthread
OBJ=$(SRC:.cc=.o)
BOOSTLIB=-L/usr/local/Cellar/boost/1.72.0_1/lib -lboost_program_options

all:  cache_server test_cache_client baseline_latencies

baseline_latencies: baseline_latencies.o cache_client.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS) $(BOOSTLIB)

cache_server: cache_server.o cache.o lru_evictor.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS) $(BOOSTLIB)

test_cache_client: test_cache_client.o cache_client.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS) $(BOOSTLIB)

%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	rm -rf *.o test_cache_client cache_server

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./cache_server
	valgrind --leak-check=full --show-leak-kinds=all ./test_cache_client
