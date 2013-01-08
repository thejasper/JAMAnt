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
#include <unistd.h>

#include "rs232.h"

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
        while (1)
        {
            // lezen tot er niets meer is
            int n = PollComport(portNumber, rcv, SIZE - 1);
            rcv[SIZE-1] = 0;

            if (n <= 0)
                break;
        }
    }

    std::string readString(int timeout = 10)
    {
        std::string res;
        int tried = 0;

        while (1)
        {
            ++tried;

            // inlezen
            int n = PollComport(portNumber, rcv, SIZE - 1);
            rcv[SIZE-1] = 0;

            if (n > 0)
                res += reinterpret_cast<const char*>(rcv);

            // stoppen als de timeout verstreken is
            if (tried == timeout)
                break;

            // stoppen als we iets hebben ontvangen en als er niets meer is
            if (!res.empty() && n != SIZE-1)
                break;

            // 100ms wachten
            usleep(100000);
        }

        std::cout << res << std::endl;
        return res;
    }

    bool sendString(std::string s)
    {
        // string verzenden, maar voor de zekerheid eerst buffer legen zodat
        // we weten dat de volgende ontvangen string het resultaat is van deze schrijfactie
        clearReceiveBuffer();
        return cprintf(portNumber, s.c_str());
    }

    bool sendCommand(std::string s, std::string answer)
    {
        // verzend commando
        if (sendString(s))
            return false;

        // kijk of het verwachte antwoord werd teruggestuurd
        if (readString().find(answer) == std::string::npos)
            return false;

        return true;
    }

    bool move(int x, int y, int z)
    {
        std::ostringstream ss;
        ss << "MOV " << x << ',' << y << ',' << z << '\n';
        std::cout << ss.str() << std::endl;

        return sendCommand(ss.str(), "MOV OK");
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
        printf("Opened\n");

        int newPortNumber = GetPortNum(settings.port.c_str());

        // poort bestaat niet
        if (newPortNumber == -1)
            return false;

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
                return false;

            portNumber = newPortNumber;
        }

        // wachten en kijken of de seriele connectie werkt
        // geen idee waarom, maar hij returnt altijd 2x CND
        for (int i = 0; i < 2; ++i)
            if (readString().find("CND") == std::string::npos)
                return false;

        return true;
    }

    bool stop()
    {
        printf("stop\n");

        // commando verzenden
        std::ostringstream ss;
        ss << "STP" << '\n';

        return sendCommand(ss.str(), "STP OK");
    }

    bool walk(const int32_t speed)
    {
        printf("Walk\n");

        // commando verzenden
        std::ostringstream ss;
        ss << "WLK " << speed << '\n';

        return sendCommand(ss.str(), "WLK OK");
    }

    bool turn(const int32_t angle)
    {
        printf("Turn\n");

        // commando verzenden
        std::ostringstream ss;
        ss << "TRN " << angle << '\n';

        return sendCommand(ss.str(), "TRN OK");
    }

    bool calibrateHeight(const int32_t height) 
    {
        printf("calibrateHeight\n");

        drawHeight = height;
        moveHeight = drawHeight + 10;

        // commando verzenden
        std::ostringstream ss;
        ss << "DST " << height << '\n';

        return sendCommand(ss.str(), "DST OK");
    }

    bool draw(const std::vector<std::vector<int32_t> > &points, const int32_t width, const int32_t height)
    {
        printf("draw\n");

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
        printf("getComPorts\n");

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
        printf("Ping\n");

        // controleer of de mier geconnecteerd is
        std::vector<std::string> listComPorts;
        getComPorts(listComPorts, "/dev/*");
        if (std::find(listComPorts.begin(), listComPorts.end(), port) == listComPorts.end())
            return 0; 

        // kijk of de mier reageert
        if (!sendCommand("PNG\n", "PNG OK"))
            return 1;

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
    std::cout << flag << std::endl;

    int ret = ant.ping();
    std::cout << ret << std::endl;

    ret = ant.walk(10);
    std::cout << ret << std::endl;

    ret = ant.stop();
    std::cout << ret << std::endl;

    ret = ant.calibrateHeight(-10);
    std::cout << ret << std::endl;

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

