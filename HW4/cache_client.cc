// https://www.boost.org/doc/libs/develop/libs/beast/example/http/client/sync/http_client_sync.cpp

#include "cache.hh"

#include <boost/program_options.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <cassert>

using cache_val_type = std::shared_ptr<Cache::byte_type>;
using map_val_type = std::pair<Cache::size_type,cache_val_type>;

namespace po = boost::program_options;
namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>


class Cache::Impl
{
private:

    std::string host_;
    std::string port_; 
    net::io_context ioc;
    // These objects perform our I/O
    tcp::resolver resolver = tcp::resolver(ioc);
    beast::tcp_stream stream = beast::tcp_stream(ioc);

    
public:

    Impl(std::string host,
        std::string port) :
        host_(host), port_(port)
    {
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        stream.connect(results);
    }

    void set(key_type key, Cache::val_type val, Cache::size_type size)
    {
        http::request<http::empty_body> req;
        req.version(11);
        req.method(http::verb::put);
        std::string val_s(val);
        req.target("/"+key+"/"+val_s);
        req.set(http::field::host, host_);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        (void) size;

        http::write(stream, req);

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer;

        // Declare a container to hold the response
        http::response<http::dynamic_body> res;

        // Receive the HTTP response
        http::read(stream, buffer, res);

    }

    std::string get(key_type key, Cache::size_type& val_size)
    {
        http::request<http::empty_body> req;
        req.version(11);
        req.method(http::verb::get);
        req.target("/"+key);
        req.set(http::field::host, host_);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        (void) val_size;

        http::write(stream, req);

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer;

        // Declare a container to hold the response
        http::response<http::dynamic_body> res;

        // Receive the HTTP response
        http::read(stream, buffer, res);

        
        // Write the message to standard out
        std::string body = boost::beast::buffers_to_string(res.body().data());

        std::string value = body.substr(body.find(":")+2,body.size()-3);
        return value;
    }

    bool del(key_type key)
    {
        http::request<http::empty_body> req;
        req.version(11);
        req.method(http::verb::delete_);
        req.target("/"+key);
        req.set(http::field::host, host_);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        http::write(stream, req);

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer;

        // Declare a container to hold the response
        http::response<http::dynamic_body> res;

        // Receive the HTTP response
        http::read(stream, buffer, res);

        return true;
    }

    Cache::size_type space_used()
    {
        http::request<http::empty_body> req;
        req.version(11);
        req.method(http::verb::head);
        req.set(http::field::host, host_);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        http::write(stream, req);

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer;

        // Declare a container to hold the response
        http::response<http::dynamic_body> res;

        // Receive the HTTP response
        http::read(stream, buffer, res);

        return std::stoi( std::string(res["space_used"]));
        
    }

    // Reset will set the mCache variable to a new unordered map, writing over the other one
    void reset() 
    {
        http::request<http::empty_body> req;
        req.version(11);
        req.method(http::verb::post);
        req.target("/reset");
        req.set(http::field::host, host_);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        http::write(stream, req);

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer;

        // Declare a container to hold the response
        http::response<http::dynamic_body> res;

        // Receive the HTTP response
        http::read(stream, buffer, res);
    }

};

Cache::Cache(std::string host, 
        std::string port):
    pImpl_ (new Impl(host, port))
{}

Cache::Cache(size_type maxmem,
             float max_load_factor,
             Evictor* evictor,
             hash_func hasher){
    (void) maxmem;
    (void) max_load_factor;
    (void) evictor;
    (void) hasher;
    assert(false);
}

Cache::~Cache()
{}

void Cache::set(key_type key, val_type val, size_type size)
{
    //finds a space in data where we can put things
    pImpl_ -> set(key, val, size);

}

Cache::val_type Cache::get(key_type key, size_type& val_size) const
{
    std::string value = pImpl_ -> get(key, val_size);
    char* val = new char[value.size() + 1];
    std::strcpy(val, value.c_str());
    val_size = value.size() + 1;
    return val;
}

bool Cache::del(key_type key)
{

    return pImpl_ -> del(key);
    
}

//all unused space should be '\0'
Cache::size_type Cache::space_used() const
{
    return pImpl_ -> space_used();

}

void Cache::reset()
{
    pImpl_ -> reset();
}
