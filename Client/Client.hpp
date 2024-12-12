#include <Messanger_Server/include/includeBoost.hpp>

class Client 
{

private:
    boost::asio::io_context      m_context;    // input/output context
    boost::asio::ip::tcp::socket m_socket;     // connection socket
    boost::asio::streambuf       m_buffer;     // buffer for message
    std::deque<std::string>      m_write_msgs; // queue for messages
    std::thread                  m_io_thread;  // context threads

public:
    Client(const std::string& adress, short port)
    : m_context(), m_socket(m_context)
    {
        // create connction with server
        boost::asio::ip::tcp::resolver resolver(m_context);
        auto end_adress = resolver.resolve(adress, std::to_string(port));
        boost::asio::async_connect(
            m_socket,
            end_adress,
            [this](boost::system::error_code error_code, const boost::asio::ip::tcp::endpoint& endpoint)
            {
                if(!error_code)
                {
                    std::cout << "Connected to server " << end_adress << std::endl;
                    read_message_from_server();
                }
                else
                {
                    std::cerr << "Error when creating connection to server " << error_code.message() << std::endl;
                }
            }
        );
    }

public:
    ~Client()
    {
        this->close();
    }

private:
    void read_message_from_server()
    {
        // wait and read message from server
        boost::asio::async_read_until(
            m_socket, 
            m_buffer,
            '\n',
            [this](boost::system::error_code error_code, std::size_t length)
            {
                if(!error_code)
                {
                    std::istream read_stream(&m_buffer);
                    std::string  message;
                    std::getline(read_stream, message);

                    std::cout << "Message from server: " << message << std::endl;

                    // wait other message
                    read_message_from_server();
                }
                else
                {
                    std::cerr << "Error while reading message from server " << error_code.message() << std::endl;
                }
            }
        );
    }

private:
    void send_message(const std::string& message)
    {
        boost::asio::post(
            m_context,
            [this, message]()
            {
                bool writing_in_progress = !m_write_msgs.empty();
                m_write_msgs.push_back(message + '\n');
                if(!writing_in_progress)
                {
                    write_to_socket();
                }
            }
        );
    }

private:
    void write_to_socket()
    {
        // write message to socket
        boost::asio::async_write(
            m_socket,
            boost::asio::buffer(m_write_msgs.front()),
            [this](boost::system::error_code error_code, std::size_t length)
            {
                if(!error_code)
                {
                    m_write_msgs.pop_front();
                    if(!m_write_msgs.empty())
                    {
                        // recall write
                        write_to_socket();
                    }
                }
                else
                {
                    std::cerr << "Write error to socket " << error_code.message() << std::endl;
                }
            }
        );
    }


private:
    void close()
    {
        // TODO: this is correct?
        boost::asio::post(
            m_context,
            [this]()
            {
                m_socket.close();
            }
        );

        if(m_io_thread.joinable())
        {
            m_io_thread.join();
        }
    }
};