#include "Ant.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <string>
#include <sstream>
#include "TimeoutSerial.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::robotics;

using boost::shared_ptr;

class AntHandler : virtual public AntIf
{
private:
    TimeoutSerial serial;

    bool checkResponse(const std::string& expected)
    {
        return serial.readStringUntil("\n") == expected;
    }

public:
    AntHandler()
    {
    }

    ~AntHandler()
    {
        serial.close();
    }

    bool init(const AntSettings &settings)
    {
        serial.open(settings.port, settings.baudrate);
        serial.setTimeout(boost::posix_time::seconds(5));

        if (!serial.isOpen())
            return false;

        return true;
    }

    bool walk(const int32_t speed)
    {
        std::ostringstream ss;
        ss << "WLK " << speed << "\n";
        serial.writeString(ss.str());

        return true;
    }

    bool turn(const int32_t angle)
    {
        std::ostringstream ss;
        ss << "TRN " << angle << "\n";
        serial.writeString(ss.str());

        return true;
    }

    bool ping()
    {
        std::ostringstream ss;
        ss << "PNG";
        serial.writeString(ss.str());

        return true;
    }

};

int main(int argc, char **argv)
{
    int port = 9070;
    shared_ptr<AntHandler> handler(new AntHandler());
    shared_ptr<TProcessor> processor(new AntProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();

//    AntSettings settings;
//    settings.port = "/dev/ttyACM0";
//    settings.baudrate = 115200;
//
//    AntHandler ant;
//    ant.init(settings);
//    
//    ant.walk(10);

    return 0;
}

