// https://www.boost.org/doc/libs/develop/libs/beast/example/http/client/sync/http_client_sync.cpp

#include "cache.hh"

#include <boost/program_options.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
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
    tcp::resolver resolver{ioc};
    beast::tcp_stream stream{ioc};

    
public:

    Impl(std::string host,
        std::string port) :
        host_(host), port_(port)
    {
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        stream.connect(results);
        std::cout<<"Client connected "<<std::endl;
    }

    ~Impl()
    {
        beast::error_code ec;

        stream.socket().shutdown(tcp::socket::shutdown_both, ec);
        std::cout<<"Shutdown"<<std::endl;

        // not_connected happens sometimes so don't bother reporting it.
        if(ec && ec != beast::errc::not_connected)
            fail(ec, "shutdown");
    }

    void fail(beast::error_code ec, char const* what)	
    {
        std::cerr << what << ": " 	<< ec.message() << "\n";
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

    char* get(key_type key, Cache::size_type& val_size)
    {
        http::request<http::empty_body> req;
        req.version(11);
        req.method(http::verb::get);
        std::cout<<"Key on client side  ["<<key<<"]"<<std::endl;
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

        val_size = 0;
        if (res.base().result() != http::status::ok) {
            return nullptr;
        }

        
        // Write the message to standard out
        std::string body = boost::beast::buffers_to_string(res.body().data());
        std::cout<<"body: ["<<body<<"]"<<std::endl;

        const std::string val_str = "value: \"";
        auto start = body.find(val_str);
        std::cout<<"start: "<<start<<std::endl;
        if (start == std::string::npos) {
            return nullptr;
        } else {
            start += val_str.size();
        }
        const auto end = body.find('"', start);
        assert(end != std::string::npos);
        std::string get_str = body.substr(start, end - start);
        val_size = get_str.size() + 1;

        

        return strdup(get_str.c_str());
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
        req.target("/");

        http::write(stream, req);

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer;
        std::cout<<"space used 2!!!"<<std::endl;
        // Declare a container to hold the response
        http::response<http::dynamic_body> res;
        std::cout<<"space used 3!!!"<<std::endl;
        // Receive the HTTP response

        http::read(stream, buffer, res);
        std::cout<<"space used 4!!!"<<std::endl;
        
        

        const auto res_str = res.at("Space-Used");
        int sz = 0;

        auto start_iterator = res_str.data();

        while (start_iterator <res_str.data() + res_str.size())
        {
            char num = *start_iterator;
            sz = (sz * 10) + num - 48;
            start_iterator++;
        }

        std::cout << "size is " <<sz<<std::endl;
        return sz;

        //return std::stoi(size_of_cache);
        
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

Cache::~Cache() {
    std::cout<<"deleting cache"<<std::endl;
}

void Cache::set(key_type key, val_type val, size_type size)
{
    //finds a space in data where we can put things
    pImpl_ -> set(key, val, size);

}

Cache::val_type Cache::get(key_type key, size_type& val_size) const
{
    val_type gotten = pImpl_->get(key,val_size);

    if (!gotten) std::cout<<"got a val of nullptr for key: "<<key<<std::endl;
    else std::cout<<"gotten a val of \""<<gotten<<"\" for key: "<<key<<std::endl;
    return gotten;
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
