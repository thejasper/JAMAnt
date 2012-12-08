#pragma once

#include <string>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"

class Ant
{
private:
    const int port, baudrate;
    static const int bufSize = 4096;
    unsigned char buffer[bufSize];

    int getResponse(std::string& response);
    int pollForAnswer(std::string& response, int wait);

public:
    Ant(int p, int b)
        : port(p), baudrate(b) { }

    int connect();

    int ping();
    int move(std::string movement, std::string info);

    void disconnect();
};
