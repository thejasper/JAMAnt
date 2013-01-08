namespace cpp robotics
namespace php robotics

struct AntSettings
{
    1:string port,
    2:i32 baudrate=115200,
}

service Ant
{
    bool init(1:AntSettings settings),

    // het equivalent van walk(0)
    bool stop(),

    // speed tussen 0 en 10
    bool walk(1:i32 speed),

    // angle tussen -45 en 45 graden
    bool turn(1:i32 angle),

	// tekenhoogte instellen 
    bool calibrateHeight(1:i32 height),

    // points zijn de x en y coordinaten van verschillende aaneensluitende lijnen
    // width en height is de resolutie van de client
    bool draw(1:list<list<i32>> points, 2:i32 width, 3:i32 height),

    // return de comportdevices
    list<string> getComPorts(1:string wildcard="/dev/ttyACM*"),

    // 0: Ant not connected
    // 1: Timeout
    // 2: Connected
    i32 ping(),
}

