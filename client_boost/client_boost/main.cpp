//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class tcp_client {
public:
    tcp_client(boost::asio::io_service& io_service, tcp::resolver::iterator& endpoint_iterator) : socket_(io_service), endpoint_iterator_(endpoint_iterator) {}
    void get_time() {
        boost::asio::connect(socket_, endpoint_iterator_);
        for (;;)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;
            
            size_t len = socket_.read_some(boost::asio::buffer(buf), error);
            
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.
            
            std::cout.write(buf.data(), len);
        }
    }
private:
    tcp::socket socket_;
    tcp::resolver::iterator endpoint_iterator_;
};


int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: client <host> <PORT>" << std::endl;
            return 1;
        }
        
        boost::asio::io_service io_service;
        
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(argv[1], argv[2]);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        
        tcp_client client(io_service, endpoint_iterator);
        client.get_time();
        client.get_time();
        client.get_time();
        client.get_time();
        client.get_time();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}