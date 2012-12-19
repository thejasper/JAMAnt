// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "Ant.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::robotics;

class AntHandler : virtual public AntIf
{
public:
    AntHandler()
    {
        // Your initialization goes here
    }

    bool init(const AntSettings &settings)
    {
        // Your implementation goes here
        printf("init\n");
    }

    bool stop()
    {
        // Your implementation goes here
        printf("stop\n");
    }

    bool walk(const int32_t speed)
    {
        // Your implementation goes here
        printf("walk\n");
    }

    bool turn(const int32_t angle)
    {
        // Your implementation goes here
        printf("turn\n");
    }

    void getComPorts(std::vector<std::string> &_return, const std::string &wildcard)
    {
        // Your implementation goes here
        printf("getComPorts\n");
    }

    int32_t ping()
    {
        // Your implementation goes here
        printf("ping\n");
    }

};

int main(int argc, char **argv)
{
    int port = 9090;
    shared_ptr<AntHandler> handler(new AntHandler());
    shared_ptr<TProcessor> processor(new AntProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
    return 0;
}

