#include "cache.hh"
#include "fifo_evictor.hh"

#include <iostream>
#include <string>
#include <cassert>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

//creates a socket to the server
auto makeCache(std::string host = "127.0.0.1", std::string port = "2020"){
    std::cout<<"Making a new cache"<<std::endl;
    Cache* my_cache = new Cache(host, port);
    std::cout<<"Cache has been initialized"<<std::endl;
    char value[]{ "six" };
    my_cache->set("apple", value, 4);
    std::cout<<"Apple has been set"<<std::endl;
    return my_cache;
}

bool testGet(bool DEBUG_PRINT_MESSAGES, std::string host, std::string port){
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing putting and getting something from the cache"<<std::endl;
    auto my_cache = makeCache(host, port);
    Cache::size_type size;
    auto ret = (my_cache->get("apple", size));
    std::string p(ret);
    if (DEBUG_PRINT_MESSAGES) {
        std::cout << ret << std::endl;
    }
    try{
        delete my_cache;
    }
    catch (int e){
    }
    return (p == "four");
}

bool testGetNull(bool DEBUG_PRINT_MESSAGES, std::string host, std::string port){
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing getting something that isn't in the cache"<<std::endl;
    auto my_cache = makeCache(host, port);
    Cache::size_type size;
    auto ret = (my_cache->get("pear", size));
    delete my_cache;
    if (ret == nullptr){
        return true;
    }
    return false;
}

bool testDel(bool DEBUG_PRINT_MESSAGES, std::string host, std::string port){
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing that a deleted item is no longer in the cache"<<std::endl;
    auto my_cache = makeCache(host, port);
    Cache::size_type size;
    delete my_cache;
    return ((my_cache->del("apple")) && (my_cache->get("apple", size)== nullptr));
}

bool testDelNull(bool DEBUG_PRINT_MESSAGES, std::string host, std::string port){
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing deleting something that isn't in the cache"<<std::endl; 
    auto my_cache = makeCache(host, port);
    delete my_cache;
    return !my_cache->del("pear");
}

bool testSpaceUsed(bool DEBUG_PRINT_MESSAGES, std::string host, std::string port){
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing to make sure that space used is equal to the space of everything we put in"<<std::endl;
    auto my_cache = makeCache(host, port);
    Cache::size_type size = 5;
    auto ret = my_cache->space_used();
    if (DEBUG_PRINT_MESSAGES) std::cout<<"ret is: "<< ret <<", size is: "<<size<<std::endl;
    delete my_cache;
    return (ret == size);
}

bool testReset(bool DEBUG_PRINT_MESSAGES, std::string host, std::string port){
    if (DEBUG_PRINT_MESSAGES) std::cout<<"Testing resetting the cache and making sure it's empty afterwards"<<std::endl;
    auto my_cache = makeCache(host, port);
    Cache::size_type size;
    if (DEBUG_PRINT_MESSAGES) std::cout<<"Cache created"<<std::endl;
    my_cache->reset();
    if (DEBUG_PRINT_MESSAGES) std::cout<<"Cache reset"<<std::endl;
    auto ret = (my_cache->get("apple", size));
    if (DEBUG_PRINT_MESSAGES) std::cout<<"Ret gotten"<<std::endl;
    delete my_cache;
    if ((ret == nullptr) && (my_cache->space_used() == 0)){
        return true;
    }
    return false;
}

//should try putting in something with the same key to see what happens
bool testSameKey(bool DEBUG_PRINT_MESSAGES, std::string host, std::string port)
{
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing putting something into the array that a key already exists for"<<std::endl;
    auto my_cache = makeCache(host, port);
    char value[]{ "six" };
    Cache::size_type size;
    my_cache->set("apple", value, 4);
    auto ret = (my_cache->get("apple", size));
    std::string p(ret);
    if (DEBUG_PRINT_MESSAGES) {
        std::cout << ret << std::endl;
    }
    delete my_cache;
    return (p == "six");
}


// this test should fill up the array, then use the evictor to remove something to put something new in
bool testEvictorWithFullCache(bool DEBUG_PRINT_MESSAGES, std::string host, std::string port)
{
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing evicting from a full array and putting something new in"<<std::endl;
    auto my_cache = makeCache(host, port);
    // i like bridge
    char spades[]{ "AK10xxx" };
    my_cache->set("spades", spades, 8);

    char diamonds[]{ "KJxx" };
    my_cache->set("diamonds", diamonds, 5);

    if(DEBUG_PRINT_MESSAGES) std::cout<<"array is full"<<std::endl;

    //at this point, the cache should be full with spades and diamonds
    //spades should be evicted so that hearts can be inserted.
    char hearts[]{ "QJx" };
    my_cache->set("hearts", hearts, 4);    

    if(DEBUG_PRINT_MESSAGES) std::cout<<"attempted to add hearts"<<std::endl;

    Cache::size_type size_hearts;
    Cache::size_type size_diamonds;
    Cache::size_type size_spades;
    auto hearts_gotten = (my_cache->get("hearts", size_hearts));
    auto diamonds_gotten = (my_cache->get("diamonds", size_diamonds));
    auto spades_gotten = (my_cache->get("spades", size_spades));
    
    if(DEBUG_PRINT_MESSAGES) std::cout<<"hearts should've been added, hearts_gotten is "<< hearts_gotten <<"."<<std::endl;
    if(DEBUG_PRINT_MESSAGES) std::cout<<"compare hearts_gotten to hearts, which is "<<hearts<<"."<<std::endl;

    std::string hearts_gotten_str(hearts_gotten);
    std::string diamonds_gotten_str(diamonds_gotten);

    //test that spades are removed
    //check that diamonds are still there
    bool hearts_in_cache = (hearts_gotten_str == "QJx");
    bool diamonds_in_cache = (diamonds_gotten_str == "KJxx");
    bool spades_not_in_cache = (spades_gotten == nullptr);

    delete my_cache;
    return (hearts_in_cache && diamonds_in_cache && spades_not_in_cache);

}

//test cache evictor where evictor needs to evict multiple items
//evicts the same item twice
bool testEvictorEvictingSameItemTwice(bool DEBUG_PRINT_MESSAGES, std::string host, std::string port)
{
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing what happens when the same key is in the evictor twice, and we need to remove things twice."<<std::endl;
    auto my_cache = makeCache(host, port);
    char value[]{ "six" };
    my_cache->set("apple", value, 4);

    //try adding something, see if it's in the cache and that the other thing's been removed

    char number_of_bananas[]{ "three" };
    Cache::size_type size;
    my_cache->set("banana", number_of_bananas, 6);

    //make sure bananas is in the cache
    auto bananas_from_cache = (my_cache->get("banana", size));
    std::string bananas(bananas_from_cache);
    //int banana_num = size;

    auto apples_from_cache = (my_cache->get("apple", size));

    //check that apples is not in the cache
    if (DEBUG_PRINT_MESSAGES && !(apples_from_cache == nullptr && bananas == "three" && size == 6)) std::cout<<"the first evict failed"<<std::endl;

    //now for the second evict
    char number_of_oranges[]{ "two" };
    my_cache->set("orange", number_of_oranges, 4);

    auto orange_from_cache = (my_cache->get("orange", size));
    std::string oranges(orange_from_cache);
    //int orange_num = size;
    //now check to make sure oranges is in the cache and that bananas is nullptr

    if(DEBUG_PRINT_MESSAGES) std::cout<<"this also tests that evictor tries again if there still isn't room for the next item"<<std::endl;

    delete my_cache;
    return (oranges == "two" && size ==4);

}

// Run all tests
// Change to true for debugging print statements
int main(int argc, char** argv)
{
    
    // Check command line arguments.
    std::string server_ip;
    std::string port;

    // optional command line arguments with flags and default values
    // got this logic from https://stackoverflow.com/questions/11280136/optional-command-line-arguments
    po::options_description desc("Options for my program");
    desc.add_options()
        // Option 'server' and 's' are equivalent.
        ("server_ip,s", po::value<std::string>(& server_ip)->default_value("127.0.0.1"),
         "server address")
        // Option 'port' and 'p' are equivalent.
        ("port,p", po::value<std::string>(& port)->default_value("2020"),
         "port number")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    assert(testGet(false, server_ip, port));
    assert(testGetNull(false, server_ip, port));
    assert(testDel(false, server_ip, port));
    assert(testDelNull(false, server_ip, port));
    assert(testSpaceUsed(false, server_ip, port));
    assert(testReset(false, server_ip, port));
    assert(testSameKey(false, server_ip, port));
    assert(testEvictorWithFullCache(false, server_ip, port));
    assert(testEvictorEvictingSameItemTwice(false, server_ip, port));


    std::cout<<"all tests pass!"<<std::endl;
    return 0;
}
