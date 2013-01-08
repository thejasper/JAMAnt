#ifndef rs232_H
#define rs232_H

#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

int Cport[28], error;

struct termios new_port_settings, old_port_settings[28];

char comports[28][13] = {
    "/dev/ttyS0", "/dev/ttyS1", "/dev/ttyS2", "/dev/ttyS3", "/dev/ttyS4", "/dev/ttyS5",
    "/dev/ttyS6", "/dev/ttyS7", "/dev/ttyS8", "/dev/ttyS9", "/dev/ttyS10", "/dev/ttyS11",
    "/dev/ttyS12", "/dev/ttyS13", "/dev/ttyS14", "/dev/ttyS15", "/dev/ttyUSB0",
    "/dev/ttyUSB1", "/dev/ttyUSB2", "/dev/ttyUSB3", "/dev/ttyUSB4", "/dev/ttyUSB5",
    "/dev/ttyACM0", "/dev/ttyACM1", "/dev/ttyACM2", "/dev/ttyACM3", "/dev/ttyACM4", "/dev/ttyACM5",
};

// return het comport_number om in OpenComport te gebruiken
int GetPortNum(const char* name)
{
    for (int i = 0; i < 28; ++i)
        if (strcmp(name, comports[i]) == 0)
            return i;

    return -1;
}

int OpenComport(int comport_number, int baudrate)
{
    int baudr;

    if ((comport_number > 27) || (comport_number < 0))
    {
        printf("illegal comport number\n");
        return(1);
    }

    switch (baudrate)
    {
    case      50 :
        baudr = B50;
        break;
    case      75 :
        baudr = B75;
        break;
    case     110 :
        baudr = B110;
        break;
    case     134 :
        baudr = B134;
        break;
    case     150 :
        baudr = B150;
        break;
    case     200 :
        baudr = B200;
        break;
    case     300 :
        baudr = B300;
        break;
    case     600 :
        baudr = B600;
        break;
    case    1200 :
        baudr = B1200;
        break;
    case    1800 :
        baudr = B1800;
        break;
    case    2400 :
        baudr = B2400;
        break;
    case    4800 :
        baudr = B4800;
        break;
    case    9600 :
        baudr = B9600;
        break;
    case   19200 :
        baudr = B19200;
        break;
    case   38400 :
        baudr = B38400;
        break;
    case   57600 :
        baudr = B57600;
        break;
    case  115200 :
        baudr = B115200;
        break;
    case  230400 :
        baudr = B230400;
        break;
    case  460800 :
        baudr = B460800;
        break;
    case  500000 :
        baudr = B500000;
        break;
    case  576000 :
        baudr = B576000;
        break;
    case  921600 :
        baudr = B921600;
        break;
    case 1000000 :
        baudr = B1000000;
        break;
    default      :
        printf("invalid baudrate\n");
        return 1;
        break;
    }

    Cport[comport_number] = open(comports[comport_number], O_RDWR | O_NOCTTY | O_NDELAY);
    if (Cport[comport_number] == -1)
    {
        perror("unable to open comport ");
        return(1);
    }

    error = tcgetattr(Cport[comport_number], old_port_settings + comport_number);
    if (error == -1)
    {
        close(Cport[comport_number]);
        perror("unable to read portsettings ");
        return 1;
    }

    memset(&new_port_settings, 0, sizeof(new_port_settings));
    new_port_settings.c_cflag = baudr | CS8 | CLOCAL | CREAD;
    new_port_settings.c_iflag = IGNPAR;
    new_port_settings.c_oflag = 0;
    new_port_settings.c_lflag = 0;
    new_port_settings.c_cc[VMIN] = 0;
    new_port_settings.c_cc[VTIME] = 0;

    error = tcsetattr(Cport[comport_number], TCSANOW, &new_port_settings);
    if (error == -1)
    {
        close(Cport[comport_number]);
        perror("unable to adjust portsettings ");
        return 1;
    }

    return 0;
}

int PollComport(int comport_number, unsigned char *buf, int size)
{
#ifndef __STRICT_ANSI__
    if (size > SSIZE_MAX)
        size = (int)SSIZE_MAX;
#else
    if (size > 4096)
        size = 4096;
#endif

    return read(Cport[comport_number], buf, size);
}

int SendByte(int comport_number, unsigned char byte)
{
    int n = write(Cport[comport_number], &byte, 1);

    if (n < 0)
        return 1;
    return 0;
}

int SendBuf(int comport_number, unsigned char *buf, int size)
{
    return write(Cport[comport_number], buf, size);
}

void CloseComport(int comport_number)
{
    close(Cport[comport_number]);
    tcsetattr(Cport[comport_number], TCSANOW, old_port_settings + comport_number);
}

int cprintf(int comport_number, const char *text)
{
    int n = 0;

    while (*text != 0)
        n |= SendByte(comport_number, *(text++));

    return n;
}

#endif
