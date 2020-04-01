// cache_server.hh

#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <memory>
#include "cache.hh"

using key_type = std::string;
using val_type = std::string;


namespace po = boost::program_options;

class Cache_Server
{
public:
    //GET should take a key type and return a value type
    val_type GET(key_type key);

    //PUT puts a value in a key
    //sets in the cache 
    void PUT(key_type key, val_type value);

    void DELETE(key_value key);

    //this returns just the head, no body
    std::pair<int, std::string> HEAD();

    //resets the cache?
    void POST(std::string argument);


    //we need to listen to that port. 
    //figure out how to listen to ports

    //then we need to parse what we're hearing this should be a for loop or something, listening and parsing
    // what we're hearing might be in http, so we have to parse that?

    //determine what to do at some point, define what happens with get and put
    // need a cache to modify

};







