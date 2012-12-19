#include "Ant.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <boost/thread.hpp>
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

class AntCommunicator 
{
private:
    TimeoutSerial serial;
    std::string currentCommand;

    std::string executeShellCmd(const char* cmd) 
    {
        FILE* pipe = popen(cmd, "r");

        if (!pipe) 
            return "Could execute shell command";

        char buffer[128];
        std::string result = "";
        while (!feof(pipe)) 
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        pclose(pipe);

        return result;
    }

public:
    // stuur currentCommand naar de seriele poort, wacht op een antwoord en doe het opnieuw
    void runAntCommand()
    {
        while (1)
        {
            std::cout << "lol" << std::endl;
            boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        }
    }

    // set het commando dat door de thread naar de seriele poort wordt verstuurd
    void setAntCommand(std::string cmd)
    {
        currentCommand = cmd;
    }

    // controleer of "port" tussen de connected devices staat
    bool isConnected(std::string port)
    {
        return getComPorts().find(port) != std::string::npos;
    }
    
    // krijg alle poorten die matchen met de wildcard
    std::string getComPorts()
    {
        return executeShellCmd("ls /dev/tty*");
    }

} antCommunicator;

class AntHandler : virtual public AntIf
{
private:
    bool checkResponse(const std::string& expected)
    {
        //return serial.readStringUntil("\n") == expected;
    }

public:
    AntHandler()
    {
    }

    ~AntHandler()
    {
        //serial.close();
    }

    bool init(const AntSettings &settings)
    {
        //if (!serial.isOpen())
        {
            //serial.open(settings.port, settings.baudrate);
            //serial.setTimeout(boost::posix_time::seconds(5));
        }
        printf("Opened\n");

        return true;
    }

    bool walk(const int32_t speed)
    {
        printf("Walk\n");

        std::ostringstream ss;
        ss << "WLK " << speed << "\n";
        //serial.writeString(ss.str());

        return true;
    }

    bool turn(const int32_t angle)
    {
        printf("Turn\n");

        std::ostringstream ss;
        ss << "TRN " << angle << "\n";
        //serial.writeString(ss.str());

        return true;
    }

    bool ping()
    {
        printf("Ping\n");

        std::ostringstream ss;
        ss << "PNG";
        //serial.writeString(ss.str());

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
    
    // laat de runAntCommand functie in een aparte thread lopen
    // deze functie stuurt constant commando's naar de mier
    // het commando kan veranderd worden door de php webserver
    //std::thread t(&AntCommunicator::runAntCommand, &antCommunicator);
    //t.join();

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

