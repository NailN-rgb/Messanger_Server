#pragma once
#include <Messanger_Server/include/includeBoost.hpp>

class ClientHandler : public std::enable_shared_from_this<ClientHandler>
{
private:
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::streambuf       m_buffer;

public:
    ClientHandler(boost::asio::ip::tcp::socket socket)
    : m_socket(std::move(socket))
    {}

    void start()
    {
        // start to listening user input
        read_message();
    }

private:
    // reading message from user
    // message must be end with '\n' symbol
    void read_message()
    {
        // get shared_ptr for this object
        auto self = shared_from_this();
        boost::asio::async_read_until(
            m_socket,
            m_buffer, 
            '\n',
            [this, self](boost::system::error_code error_code, std::size_t length)
            {
                if(!error_code)
                {
                    // message is readed succesfly
                    std::istream stream(&m_buffer);
                    std::string message;
                    std::getline(stream, message);

                    std::cout << "Message from " << m_socket.remote_endpoint() << " : " << message << std::endl;

                    write_message("Message recieved\n");
                }
                else
                {
                    std::cerr << "Error while reading message " << error_code.message() << std::endl;
                }
            }

        );
    }

    void write_message(const std::string& msg)
    {
        // shared ptr to ClientHandler in context
        auto self = shared_from_this();
        boost::asio::async_write(
            m_socket, 
            boost::asio::buffer(msg),
            [this, self](boost::system::error_code error_code, std::size_t length) // length variable may not be used
            {
                if(!error_code)
                {
                    // start reading message after writing
                }
                else
                {
                    std::cerr << "Error while writing message: " << error_code.message() << std::endl;
                }
            }
        );
    }
};