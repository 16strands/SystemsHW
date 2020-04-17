// cache_server.cc

//we need to listen to that port. 
//figure out how to listen to ports

//then we need to parse what we're hearing this should be a for loop or something, listening and parsing
// what we're hearing might be in http, so we have to parse that?

//determine what to do at some point, define what happens with get and put
// need a cache to modify


// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: HTTP server, asynchronous
//
//------------------------------------------------------------------------------

#include "cache.hh"
#include "fifo_evictor.hh"

#include <boost/program_options.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <boost/beast/http/field.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace po = boost::program_options;
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using response_t = http::response<http::string_body>;
const auto version = 11;

// ATTENTION!! IF YOU SEE mime_type REPLACE IT WITH "application/json" !!!!

// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
std::string
path_cat(
    beast::string_view base,
    beast::string_view path)
{
    if(base.empty())
        return std::string(path);
    std::string result(base);
#ifdef BOOST_MSVC
    char constexpr path_separator = '\\';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    for(auto& c : result)
        if(c == '/')
            c = path_separator;
#else
    char constexpr path_separator = '/';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
#endif
    return result;
}

//////////////////////////////////////////////////////////////////////////////
// Handle get request for a given cache
response_t
get_request(const Cache& cache, const std::string key)
{
    Cache::size_type sz;
    const auto value = cache.get(key, sz);
    if (!value || !sz) {
        return response_t(http::status::not_found, version);
    }

    response_t res(http::status::ok, version);
    res.body() = std::string("{\n\tkey: \"") + key + "\",\n" +
                 "\tvalue: \"" +value + "\"\n}\n";
    return res;
}


//////////////////////////////////////////////////////////////////////////////
// Handle get request for a given cache
response_t
put_request(Cache& cache, const std::string kv)
{
    auto split = kv.find('/');
    assert(split != std::string::npos);
    const auto key = kv.substr(0, split);
    const auto value = kv.substr(split + 1);
    cache.set(key, value.c_str(), value.size() + 1);
    response_t res(http::status::ok, version);
    res.body() = "";
    return res;
}


//////////////////////////////////////////////////////////////////////////////
// Handle delete request for a given cache
response_t
delete_request(Cache& cache, const std::string key)
{
    const auto ok = cache.del(key);
    if (!ok) {
        return response_t(http::status::not_found, version);
    }

    response_t res(http::status::ok, version);
    return res;
}


//////////////////////////////////////////////////////////////////////////////
// Handle post reset request for a given cache
response_t
post_request(Cache& cache, const std::string target)
{
    if (target != "reset") {
        return response_t(http::status::not_found, version);
    }

    cache.reset();
    return response_t(http::status::ok, version);
}


//////////////////////////////////////////////////////////////////////////////
// Handle a generic request for a given cache
template <class Body, class Allocator, class Send>
void
handle_request(Cache& cache, http::request<Body, http::basic_fields<Allocator>>&& req,
               Send&& send)
{
    const auto target = std::string(req.target()).substr(1);
    response_t res(http::status::ok, version);

    if (req.method() == http::verb::get) {
        res = get_request(cache, target);
    }

    else if (req.method() == http::verb::put) {
        res = put_request(cache, target);
    }

    else if (req.method() == http::verb::delete_) {
        res = delete_request(cache, target);
    }

    else if (req.method() == http::verb::head) {
        res.body() = "";
    }

    else if (req.method() == http::verb::post) {
        res = post_request(cache, target);
    }


    else {
        res = response_t(http::status::bad_request, version);
        res.body() = "Illegal request type\n";
    }

    res.keep_alive(req.keep_alive());
    res.set(http::field::content_type, "text/json");
    res.set("Space-Used", cache.space_used());
    res.prepare_payload();
    return send(std::move(res));
}



//------------------------------------------------------------------------------

// Report a failure
void
fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

// Handles an HTTP server connection
class session : public std::enable_shared_from_this<session>
{
    // This is the C++11 equivalent of a generic lambda.
    // The function object is used to send an HTTP message.
    struct send_lambda
    {
        session& self_;

        explicit
        send_lambda(session& self)
            : self_(self)
        {
        }

        template<bool isRequest, class Body, class Fields>
        void
        operator()(http::message<isRequest, Body, Fields>&& msg) const
        {
            // The lifetime of the message has to extend
            // for the duration of the async operation so
            // we use a shared_ptr to manage it.
            auto sp = std::make_shared<
                http::message<isRequest, Body, Fields>>(std::move(msg));

            // Store a type-erased version of the shared
            // pointer in the class to keep it alive.
            self_.res_ = sp;

            // Write the response
            http::async_write(
                self_.stream_,
                *sp,
                beast::bind_front_handler(
                    &session::on_write,
                    self_.shared_from_this(),
                    sp->need_eof()));
        }
    };

    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    std::shared_ptr<Cache> cache_root_;// = std::make_shared<Cache>(); //idk when the arguments go in
    http::request<http::string_body> req_;
    std::shared_ptr<void> res_;
    send_lambda lambda_;

public:
    // Take ownership of the stream
    session(
        tcp::socket&& socket,
        std::shared_ptr<Cache> cache_root)
        : stream_(std::move(socket))
        , cache_root_(cache_root)
        , lambda_(*this)
    {
    }

    // Start the asynchronous operation
    void
    run()
    {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this session. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
        net::dispatch(stream_.get_executor(),
                      beast::bind_front_handler(
                          &session::do_read,
                          shared_from_this()));
    }

    void
    do_read()
    {
        // Make the request empty before reading,
        // otherwise the operation behavior is undefined.
        req_ = {};

        // Set the timeout.
        stream_.expires_after(std::chrono::seconds(30));

        // Read a request
        http::async_read(stream_, buffer_, req_,
            beast::bind_front_handler(
                &session::on_read,
                shared_from_this()));
    }

    void
    on_read(
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        // This means they closed the connection
        if(ec == http::error::end_of_stream) {
            return do_close();
        }

        if(ec) {
            return fail(ec, "read");
        }

        // Send the response
        handle_request(*cache_root_, std::move(req_), lambda_);
    }

    void
    on_write(
        bool close,
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "write");

//        close = false;
        if(close)
        {
            // This means we should close the connection, usually because
            // the response indicated the "Connection: close" semantic.
            return do_close();
        }

        // We're done with the response so delete it
        res_ = nullptr;

        // Read another request
        do_read();
    }

    void
    do_close()
    {
        // Send a TCP shutdown
        beast::error_code ec;
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

        // At this point the connection is closed gracefully
    }
};

//------------------------------------------------------------------------------

// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener>
{
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    std::shared_ptr<Cache> cache_root_;

public:
    listener(
        net::io_context& ioc,
        tcp::endpoint endpoint,
        std::shared_ptr<Cache> cache_root)
        : ioc_(ioc)
        , acceptor_(net::make_strand(ioc))
        , cache_root_(cache_root)
    {
        beast::error_code ec;

        // Open the acceptor
        acceptor_.open(endpoint.protocol(), ec);
        if(ec)
        {
            fail(ec, "open");
            return;
        }

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if(ec)
        {
            fail(ec, "set_option");
            return;
        }

        // Bind to the server address
        acceptor_.bind(endpoint, ec);
        if(ec)
        {
            fail(ec, "bind");
            return;
        }

        // Start listening for connections
        acceptor_.listen(
            net::socket_base::max_listen_connections, ec);
        if(ec)
        {
            fail(ec, "listen");
            return;
        }
    }

    // Start accepting incoming connections
    void
    run()
    {
        do_accept();
    }

private:
    void
    do_accept()
    {
        // The new connection gets its own strand
        acceptor_.async_accept(
            net::make_strand(ioc_),
            beast::bind_front_handler(
                &listener::on_accept,
                shared_from_this()));
    }

    void
    on_accept(beast::error_code ec, tcp::socket socket)
    {
        if(ec)
        {
            fail(ec, "accept");
        }
        else
        {
            // Create the session and run it
            std::make_shared<session>(
                std::move(socket),
                cache_root_)->run();
            std::cout<<"Server connected "<<std::endl;
        }

        // Accept another connection
        do_accept();
    }
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
            ("maxmem,m", po::value<int>(& maxmem)->default_value(65536),
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


    //make fifo evictor
    Fifo_Evictor* fifo_evictor = new Fifo_Evictor();
    //here we make the cache
//    Cache server_cache = Cache(maxmem,0.75,fifo_evictor);

    // turn server into address object
    auto const address = net::ip::make_address(server);

    //instead of a document, we have a cache
    std::shared_ptr<Cache> cache_root = std::make_shared<Cache>( maxmem,0.75,fifo_evictor);

    // The io_context is required for all I/O
    net::io_context ioc{threads};

    // Create and launch a listening port
    std::make_shared<listener>(
        ioc,
        tcp::endpoint{address, port},
        cache_root)->run();

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
        v.emplace_back(
        [&ioc]
        {
            ioc.run();
        });
    ioc.run();

    return EXIT_SUCCESS;

}

// implement handle request function
// method requests are urls
// set some things right after cache construction so that we can test GET alone