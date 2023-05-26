#include <boost/asio.hpp>
#include <iostream>
#include <string>

class SyncTCPClient
{
public:
    using TCPSocket = boost::asio::ip::tcp::socket;
    using EndPoint = boost::asio::ip::tcp::endpoint;
    using IOService = boost::asio::io_service;

    SyncTCPClient (const std::string & ip_addr, unsigned short port):
        endpoint( boost::asio::ip::address::from_string(ip_addr), port ),
        socket(io_service)

    {
        socket.open(endpoint.protocol() ); 
    }


    void connect()
    {
        socket.connect(endpoint);
    }

    void close()
    {
        socket.shutdown(TCPSocket::shutdown_both);
        socket.close();
    }

    
    std::string Communicate(std::string request )
    {
        request = request + "\n";
        
        send(request);

        return receive();

    }

private:

    void send(const std::string  request )
    {
        boost::asio::write(socket, boost::asio::buffer(request));
    }

    std::string receive()
    {
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, '\n');

        std::istream stream (&buffer);
        std::string response ;

        std::getline(stream, response);

        return response;
    }

private:

    IOService io_service;
    EndPoint endpoint;
    TCPSocket socket;

};



int main ()
{

    std::string  endpoint = "127.0.0.1";
    unsigned short port = 3333;

    try
    {
        SyncTCPClient client ( endpoint, port);

        client.connect();

        std::string response = client.Communicate("Hello ! awaiting response" );

        std::cout<<"Server Response: [" << response <<"] \n";

        client.close();
    }
    catch( boost::system::system_error &e)
    {
        std::cout<<"client failed to communicate: " << e.what() <<", code: "<<e.code() <<"\n";
    }


    return 0;
}
