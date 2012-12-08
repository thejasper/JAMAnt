#include <iostream>

#include "ant.h"

using namespace std;

int Ant::connect()
{
    return OpenComport(port, baudrate);
}

int Ant::getResponse(string& response)
{
    int n = PollComport(port, buffer, bufSize - 1);

    if (n == 0)
        return 1;

    buffer[n] = 0;
    response = string((char*)buffer);

    return 0;
}

int Ant::pollForAnswer(string& response, int wait)
{
    int numTries = wait * 10;

    for (int i = 0; i < numTries; ++i)
    {
        int res = getResponse(response);

        if (res == 0)
            return 0;

        // sleep for 100ms
#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);
#endif
    }

    return 1; // time out
}

int Ant::ping()
{
    string answer;

    SendBuf(port, (unsigned char*)"PNG\n", 3); // send command
    int res = pollForAnswer(answer, 60); // wait 1 second for a response

    if (!res && answer == "PNG OK")
        return 0;
    else
        return 1;
}

int Ant::move(string movement, string info)
{
    string combination = movement + ' ' + info + '\n', answer;

    SendBuf(port, (unsigned char*)combination.c_str(), combination.size()); // send command
    int res = pollForAnswer(answer, 5); // wait 5 seconds for a response

    if (!res && answer == "WLK OK")
        return 0;
    else
        return 1;
}

void Ant::disconnect()
{
    CloseComport(port);
}
