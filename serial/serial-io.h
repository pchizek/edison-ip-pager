
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <string>
//#include <iostream>

#define PORT_NAME "/dev/ttyMFD1"

#define ETX 0x03
#define EOT 0x04

#define PINMUX_MODE_PATH "/sys/class/gpio/gpio%d/direction"

using namespace std;

void relay_message(string serial, string message);