
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

int main(int argc, char* argv[]){ // sender[],int senderLen, char content[], int contentLen){

    // Set pin functions
    // 130:rx
    // 131:tx

    // Parse CLI args
    if (argc!=3){
        printf("Error: Invalid number of arguments");
        return(-2);
    }

    // Get Arguments
    string sender_s = argv[1];
    string content_s = argv[2];

    // Get sender
    int senderLen = sender_s.length()+1;
    char sender[senderLen];
    strcpy(sender,sender_s.c_str());

    // Get content
    int contentLen = content_s.length()+1;
    char content[contentLen]; 
    strcpy(content,content_s.c_str());

    int serialPort = open(PORT_NAME,O_RDWR);

    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    // Check for error
    if (serialPort<0){
        printf("Error %i from open: %s\n",errno,strerror(errno));
        return -1;
    }

    // Get attributes for file
    if (tcgetattr(serialPort, &tty) != 0){
        printf("Error %i from tcgetattr: %s\n",errno, strerror(errno));
    }

    // Set options
    tty.c_cflag &= ~PARENB;     // No Parity Bits
    tty.c_cflag &= ~CSTOPB;     // 1 Stop Bit
    tty.c_cflag |= CS8;         // 8 Data Bits
    tty.c_cflag &= ~CRTSCTS;    // Turn off flow control

    tty.c_lflag &= ~ECHO;       // Disable echo

    tty.c_oflag |= OPOST;       // Special intepretation of output bytes 

    //tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    //tty.c_cc[VMIN] = 0;


    cfsetispeed(&tty, B9600);   // Input baud
    cfsetospeed(&tty, B9600);   // Output baud

    // Get attributes for file
    if (tcsetattr(serialPort, TCSANOW, &tty) != 0){
        printf("Error %i from tcsetattr: %s\n",errno, strerror(errno));
    }

    // Concatenate char arrays with delimiter characters
    char outBuf[senderLen+contentLen+1];

    // Put sender into data packet
    for (int i = 0; i<(senderLen-1); i++){
        outBuf[i] = sender[i];
    }

    // Add sender stop char
    outBuf[senderLen-1] = ETX;

    // Put conent into data packet
    for (int j = 0; j<(contentLen-1);j++){
        outBuf[senderLen+j] = content[j];
    }

    // Add packet stop char
    outBuf[senderLen+contentLen-1] = EOT;

    // Add null terminator
    outBuf[senderLen+contentLen] = '\0';

    // Write
    int rc = write(serialPort,outBuf,(senderLen+contentLen+1));

    // Close
    close(serialPort);

    return rc;

}