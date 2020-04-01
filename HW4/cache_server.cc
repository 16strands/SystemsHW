// cache_server.cc

#include "cache_server.hh"

using key_type = std::string;
using val_type = std::string;


namespace po = boost::program_options;

class Cache_Server
{
public:
    //GET should take a key type and return a value type
    val_type GET(key_type key) 
    {
        return "";
    }

    //PUT puts a value in a key
    //sets in the cache 
    void PUT(key_type key, val_type value)
    { //from eitan's in class example, when he went to s.e.rv.e.r:PORT/key, it showed the value
        //this puts it in the internet

        //we somehow need to put the vlaue in subdirectory "key" of our server and port
    }

    void DELETE(key_value key)
    {
        //deletes the key
    }

    //this returns just the head, no body
    std::pair<int, std::string> HEAD()
    {
        return std::pair<int,std::string>(0,"");
    }

    //resets the cache?
    void POST(std::string argument)
    {
        //if the argument isn't reset, then do nothing
        if(argument == reset) {
            cache.reset();
        }
    }


    //we need to listen to that port. 
    //figure out how to listen to ports

    //then we need to parse what we're hearing this should be a for loop or something, listening and parsing
    // what we're hearing might be in http, so we have to parse that?

    //determine what to do at some point, define what happens with get and put
    // need a cache to modify

};



