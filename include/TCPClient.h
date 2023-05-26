#include <boost/asio.hpp>


class TCPClient
{
    public:
        virtual void connect() = 0;
        virtual void close() = 0;



};
