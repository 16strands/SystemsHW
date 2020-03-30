#include "cache.hh"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
//include a tester for a hasher
//this hasher just returns the hash given from the key value mod size of cache.
/*struct key_hasher
{


	~key_hasher() = default;

	key_hasher(const distance_map_t& distances)
	: distances_(distances)
	{}

	//this is wrong, it's just a functor I used elsewhere
	bool operator()(const vertex_descriptor &a, 
					const vertex_descriptor &b ) const 
	{
		return (distances_.at(a) > distances_.at(b));
	}
};*/



// I'm stealing this from Jim:
//
// parseCommand(command):
//
// Takes a string `command` and gives back a vector of strings,
// split into words according to spaces within `command`.
//
std::vector<std::string> parseCommand(std::string command) {
  std::istringstream iss(command);
  std::vector<std::string> words(std::istream_iterator<std::string>{iss},
				 std::istream_iterator<std::string>());  
  return words;
}
// "test_llist.cc", Jim Fix (2020)


int main(){

	int maxmemory;
	std::cout<< "how much memory should I allocate?"<<std::endl;
	std::cin>>maxmemory;

	/* uncomment when using hash table
	float maxLoadFactor;
	std::cout<< "what will maxLoadFactor be?"<<std::endl;
	std::cin>>maxLoadFactor;
	*/

    Cache cache_prototype(maxmemory);

    //the following is inspired by JIM:
    std::string command;

    do {
    	getline(std::cin,command);

    	std::vector<std::string> keywords = parseCommand(command);

    	if (keywords[0] == "help")
    	{
    		std::cout<<"set [key] [value] [size]:\tadds a key value pair to the cache"<<std::endl;
    		std::cout<<"get [key] [size]:\t\tgets a value from the cache"<<std::endl;
    		std::cout<<"del [key]:\t\t\tremoves the item from the cache"<<std::endl;
    		std::cout<<"space used:\t\ttellls u how much space used"<<std::endl;
    		std::cout<<"reset:\t\t\tremoves everything from the whatsitcalled"<<std::endl;
    		std::cout<<"quit:\tduh"<<std::endl;	
    	}

    	if (keywords[0] == "set") 
    	{
    		key_type key = static_cast<key_type>(std::atoi(keywords[1]));

    		Cache::val_type val = static_cast<val_type> ( &keywords[2] );

    		Cache::size_type size = static_cast<Cache::size_type>(keywords[3])

    		cache_prototype.set(key,val,size);
    	}


    	//std::cout<<cache_prototype->to_string();

    } while(command != "quit");


    if (test_construct() > 0){
        std::cout<<"Constructor failed"<<std::endl;
    }

    

}

