// cache_server.cc
#include <boost/program_options.hpp>
#include <string>
#include <iostrea

namespace po = boost::program_options;

int main(int argc, char** argv){

    int maxmem;
    std::string server;
    unsigned short port;
    int threads;

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

    std::cout<<maxmem<<std::endl;
    std::cout<<server<<std::endl;
    std::cout<<port<<std::endl;
    std::cout<<threads<<std::endl;
}