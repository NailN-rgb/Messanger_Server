#include <Messanger_Server/include/includeBoost.hpp>
#include <Messanger_Server/Server/ServerStarter/ServerStarter.hpp>

int main()
{
    try
    {
        boost::asio::io_context io_context; 
        ServerStarter server(io_context, 12345); 

        std::cout << "Server is running on port 12345..." << std::endl;

        server.start(); // Запускаем сервер
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("main: " + std::string(e.what()));
    }

    return 0;
}