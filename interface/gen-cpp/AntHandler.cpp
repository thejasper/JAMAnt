#include <csignal>
#include "Ant.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <boost/algorithm/string.hpp>

#include <string>
#include <sstream>
#include <thread>
#include "TimeoutSerial.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::robotics;

using boost::shared_ptr;

TimeoutSerial serial;

class AntHandler : virtual public AntIf
{
private:
    std::string port;

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
        printf("Opened\n");

        port = settings.port;
        serial.open(port, settings.baudrate);
        serial.setTimeout(boost::posix_time::seconds(5));

        return true;
    }

    bool stop()
    {
        printf("stop\n");

        return walk(0);
    }

    bool walk(const int32_t speed)
    {
        printf("Walk\n");

        std::ostringstream ss;
        ss << "WLK " << speed << "\n";
        serial.writeString(ss.str());

        return true;
    }

    bool turn(const int32_t angle)
    {
        printf("Turn\n");

        std::ostringstream ss;
        ss << "TRN " << angle << "\n";
        serial.writeString(ss.str());

        return true;
    }

    void getComPorts(std::vector<std::string> &_return, const std::string &wildcard)
    {
        printf("getComPorts\n");

        FILE* pipe = popen(wildcard.c_str(), "r");

        char buffer[128];
        std::string result = "";
        while (!feof(pipe)) 
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        pclose(pipe);

        boost::split(_return, result, boost::is_any_of("\n"));
    }

    int32_t ping()
    {
        printf("Ping\n");

#ifndef CONNECTED
        return 2;
#endif
        std::vector<std::string> listComPorts;
        getComPorts(listComPorts, "/dev/*");

        if (std::find(listComPorts.begin(), listComPorts.end(), port) == listComPorts.end())
            return 0; // not connected

        serial.writeString("PNG\n");

        //return 1; // timeout
        return 2; // connected
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

