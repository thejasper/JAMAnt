namespace cpp robotics

struct AntSettings
{
	1: required string port,
	2: optional i32 baudrate=115200,
}

service Ant
{
	bool init(1:AntSettings settings),

	bool walk(1:i32 speed),
	bool turn(1:i32 angle),
	bool ping(),
}

