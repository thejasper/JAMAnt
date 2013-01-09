#include <csignal>
#include "Ant.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <boost/algorithm/string.hpp>

#include <sstream>
#include <stdio.h>
#include <string>
#include <thread>
#include <unistd.h>

#include "rs232.h"

#define red     "\033[1;31m"       
#define cyan    "\033[1;36m"       
#define green   "\033[1;32m"       
#define blue    "\033[1;34m"       
#define black   "\033[1;30m"
#define brown   "\033[1;33m"
#define magenta "\033[1;35m"
#define gray    "\033[1;37m"
#define normal  "\033[0m"        

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::robotics;

using boost::shared_ptr;

// 1x initialisatie nodig, niet telkens opnieuw voor elke client
static std::string port;
static int portNumber = -1;
static int drawHeight, moveHeight = -1;

class AntHandler : virtual public AntIf
{
private:
    static const int SIZE = 4096;
    unsigned char rcv[SIZE];

    void clearReceiveBuffer()
    {
        // lezen tot er niets meer is
        while (PollComport(portNumber, rcv, SIZE - 1) > 0);
    }

    std::string readString(int timeout = 10)
    {
        // direct returnen als hij niet verbonden is, anders blockt de read
        if (!connected())
            return "";

        std::string res;
        int tried = 0;

        while (1)
        {
            ++tried;

            // inlezen
            int n = PollComport(portNumber, rcv, SIZE - 1);

            if (n > 0)
            {
                rcv[n] = 0;
                res += reinterpret_cast<const char*>(rcv);
            }

            // stoppen als de timeout verstreken is
            if (tried == timeout)
                break;

            // stoppen als we iets hebben ontvangen en als er niets meer is
            if (!res.empty() && n != SIZE-1)
                break;

            // 100ms wachten
            usleep(100000);
        }

        //std::cout << res << std::endl;
        return res;
    }

    bool sendString(std::string s)
    {
        std::string msg = "Sending.. " + s;
        printf("%s%s%s", cyan, msg.c_str(), normal);

        if (!connected())
            return false;

        // string verzenden, maar voor de zekerheid eerst buffer legen zodat
        // we weten dat de volgende ontvangen string het resultaat is van deze schrijfactie
        clearReceiveBuffer();
        return cprintf(portNumber, s.c_str());
    }

    bool sendCommand(std::string s, std::string answer)
    {
        // verzend commando
        if (sendString(s))
            return print(false, "Commando kon niet worden verzonden\n");

        // kijk of het verwachte antwoord werd teruggestuurd
        if (readString().find(answer) == std::string::npos)
            return print(false, "Geen reactie op commando\n");

        return print(true, "OK\n");
    }

    bool move(int x, int y, int z)
    {
        std::ostringstream ss;
        ss << "MOV " << x << ',' << y << ',' << z << '\n';
        //std::cout << ss.str() << std::endl;

        return sendCommand(ss.str(), "MOV OK");
    }

    void printHeader(std::string message)
    {
        printf("%s***%s***%s\n", blue, boost::to_upper_copy(message).c_str(), normal);
    }

    bool print(bool correct, std::string message)
    {
        if (!correct)
            printf("%s%s%s", red, message.c_str(), normal);
        else
            printf("%s%s%s", green, message.c_str(), normal);

        return correct;
    }

    bool connected()
    {
        std::vector<std::string> listComPorts;
        getComPorts(listComPorts, "/dev/*");

        if (portNumber == -1)
            return false;

        // connected als hij gevonden werd in de lijst
        return std::find(listComPorts.begin(), listComPorts.end(), port) != listComPorts.end();
    }

public:
    AntHandler()
    {
    }

    ~AntHandler()
    {
        if (portNumber != -1)
        {
            CloseComport(portNumber);
            portNumber = -1;
        }
    }

    bool init(const AntSettings &settings)
    {
        printHeader("Opened");

        int newPortNumber = GetPortNum(settings.port.c_str());

        // poort bestaat niet
        if (newPortNumber == -1)
            return print(false, "Poort bestaat niet\n");

        // vorige open poort eventueel eerst sluiten
        if (newPortNumber != portNumber && portNumber != -1)
        {
            CloseComport(portNumber);
            portNumber = -1;
        }

        // als het een andere poort is, proberen te openen
        if (portNumber == -1)
        {
            if (OpenComport(newPortNumber, settings.baudrate))
                return print(false, "Poort kon niet worden geopend\n");

            portNumber = newPortNumber;
            port = settings.port;
        }

        // wachten en kijken of de seriele connectie werkt
        // geen idee waarom, maar hij returnt altijd 2x CND
        for (int i = 0; i < 2; ++i)
            if (readString().find("CND") == std::string::npos)
                return print(false, "Geen CND commando's ontvangen van de mier\n");

        return print(true, "OK\n");
    }

    bool stop()
    {
        printHeader("stop");

        // commando verzenden
        std::ostringstream ss;
        ss << "STP" << '\n';

        return sendCommand(ss.str(), "STP OK");
    }

    bool walk(const int32_t speed)
    {
        printHeader("Walk");

        // commando verzenden
        std::ostringstream ss;
        ss << "WLK " << speed << '\n';

        return sendCommand(ss.str(), "WLK OK");
    }

    bool turn(const int32_t angle)
    {
        printHeader("Turn");

        // commando verzenden
        std::ostringstream ss;
        ss << "TRN " << angle << '\n';

        return sendCommand(ss.str(), "TRN OK");
    }

    bool calibrateHeight(const int32_t height) 
    {
        printHeader("calibrateHeight");

        drawHeight = height;
        moveHeight = drawHeight + 10;

        // commando verzenden
        std::ostringstream ss;
        ss << "DST " << height << '\n';

        return sendCommand(ss.str(), "DST OK");
    }

    bool draw(const std::vector<std::vector<int32_t> > &points, const int32_t width, const int32_t height)
    {
        printHeader("draw");

        // stuur resolutie
        std::ostringstream ss;
        ss << "DRW " << width << ',' << height << '\n';

        if (!sendCommand(ss.str(), "DRW OK"))
            return false;

        // default pose
        if (!move(0, 0, moveHeight))
            return false;

        for (const std::vector<int32_t>& line : points)
        {
            int n = line.size();

            // checken of er punten te tekenen zijn
            if (n == 0)
                continue;

            // naar eerste punt gaan
            if (!move(line[0], line[1], moveHeight))
                return false;

            // pen zakken
            if (!move(line[0], line[1], drawHeight))
                return false;

            // de rest van de punten tekenen op drawhoogte
            for (int i = 2; i < (int)line.size(); i += 2) 
                if (!move(line[i], line[i+1], drawHeight))
                    return false;

            // pen opheffen
            if (!move(line[n-2], line[n-1], moveHeight))
                    return false;
        }

        // back to default pose
        if (!move(0, 0, moveHeight))
            return false;

        return true;
    }

    void getComPorts(std::vector<std::string> &_return, const std::string &wildcard)
    {
        //printHeader("getComPorts");

        const std::string cmd = "ls " + wildcard;
        FILE *pipe = popen(cmd.c_str(), "r");

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
        printHeader("Ping");

        // controleer of de mier geconnecteerd is
        if (!connected())
        {
            print(false, "Mier is niet geconnecteerd of je koos de verkeerde poort\n");
            return 0; 
        }

        // kijk of de mier reageert
        if (!sendCommand("PNG\n", "PNG OK"))
        {
            print(false, "Mier reageert niet op ping commando\n");
            return 1;
        }

        // alles ok, device is connected en reageert
        return 2; 
    }
};

void localTest()
{
    AntSettings settings;
    settings.port = "/dev/ttyACM0";
    settings.baudrate = 115200;

    AntHandler ant;
    bool flag = ant.init(settings);
    //std::cout << flag << std::endl;

    int ret = ant.ping();
    //std::cout << ret << std::endl;

    ret = ant.walk(10);
    //std::cout << ret << std::endl;

    ret = ant.stop();
    //std::cout << ret << std::endl;

    ret = ant.calibrateHeight(-10);
    //std::cout << ret << std::endl;

    std::vector<int32_t> line1 = {2, 3, 5, 6};
    std::vector<int32_t> line2 = {2, 3, 5, 6, 7, 8};
    std::vector<std::vector<int32_t> > points = {line1, line2};
    ant.draw(points, 800, 600);
}

int main(int argc, char **argv)
{
    int port = 9070;
    shared_ptr<AntHandler> handler(new AntHandler());
    shared_ptr<TProcessor> processor(new AntProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);

#ifdef LOCALTEST
    localTest();
#else
    server.serve();
#endif

    return 0;
}

