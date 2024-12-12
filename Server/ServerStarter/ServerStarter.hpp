#pragma once
#include <Messanger_Server/include/includeBoost.hpp>

// Client Handler Class 
#include <Messanger_Server/Server/ClientHandler/ClientHandler.hpp>

class ServerStarter
{

private:
    boost::asio::ip::tcp::acceptor m_acceptor;

public:
    ServerStarter(boost::asio::io_context& io_context, std::size_t port)
    : m_acceptor(
        io_context, 
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
        start_accept_input();
    }
    
    void start()  
    {
        m_acceptor.get_io_context().run();
    }

    void handle_accept(const boost::system::error_code& error, boost::asio::ip::tcp::socket&& socket)
    {
        if(!error)
        {
            std::cout << "New client connected " << socket.remote_endpoint() << std::endl;
            
            std::make_shared<ClientHandler>(std::move(socket))->start();
        }
        else
        {
            std::cerr << "Error while accepting connection " << error.message() << std::endl;
        }

        start_accept_input();
    }

    // start listening input connections to server
    void start_accept_input()
    {
        // create new cosket for client
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(m_acceptor.get_io_context());

        m_acceptor.async_accept(
            *socket,
            [this, socket](boost::system::error_code error) {
                this->handle_accept(error, std::move(*socket));
            }
        );
    }

};