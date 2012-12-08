#include <iostream>
#include <string>
#include <sstream>

#include "ant.h"

using namespace std;

int stringToInt(const string &s)
{
    int ret;
    istringstream ss(s);
    ss >> ret;
    return ret;
}

int main(int argc, char *argv[])
{
    int port = -1, baudrate = -1;
    string action = "";

    // parse command line arguments
    for (int i = 1; i < argc; ++i)
    {
        cout << argv[i] << endl;
        if (argv[i] == string("--port"))
            port = stringToInt(argv[++i]);
        else if (argv[i] == string("--baudrate"))
            baudrate = stringToInt(argv[++i]);
        else if (argv[i] == string("--ping"))
            action = "ping";
        else if (argv[i] == string("--walk"))
            action = "walk";
        else if (argv[i] == string("--draw"))
            action == "draw";
    }

    if (port == -1) // no port number specified
        return 1;

    if (baudrate == -1) // no baud rate specified
        return 2;

    Ant ant(port, baudrate); // connect to ant
    if (ant.connect()) // serial port could not be opened
        return 3;

    // execute actions
    if (action == "ping")
    {
        int result = ant.ping();

        if (result) // ping command failed
            return 4;
    }
    else if (action == "walk")
    {
        string movement = argv[argc - 2]; // penult argument (WLK or TRN)
        string info = argv[argc - 1]; // last argument (speed or angle)

        int result = ant.move(movement, info);

        if (result) // walk command failed
            return 5;
    }
    else if (action == "draw")
    {

    }

    return 0;
}
