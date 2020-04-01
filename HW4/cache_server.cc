// cache_server.cc

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







int main(int argc, char** argv){

    int maxmem;
    std::string server;
    unsigned short port;
    int threads;

    // optional command line arguments with flags and default values
    // got this logic from https://stackoverflow.com/questions/11280136/optional-command-line-arguments
    po::options_description desc("Options for my program");
    desc.add_options()
            // Option 'maxmem' and 'm' are equivalent.
            ("maxmem,m", po::value<int>(& maxmem)->default_value(4096),
             "cache size")
            // Option 'server' and 's' are equivalent.
            ("server,s", po::value<std::string>(& server)->default_value("127.0.0.1"),
             "server address")
            // Option 'port' and 'p' are equivalent.
            ("port,p", po::value<unsigned short>(& port)->default_value(2020),
             "port number")
            // Option 'threads' and 't' are equivalent.
            ("threads,t", po::value<int>(& threads)->default_value(1),
                    "number of threads")
            ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // turn server into address object
    auto const address = net::ip::make_address(server);
    auto const doc_root = std::make_shared<std::string>(".");

    // The io_context is required for all I/O
    net::io_context ioc{threads};

    // Create and launch a listening port
    std::make_shared<listener>(
            ioc,
            tcp::endpoint{address, port},
            doc_root)->run();
}