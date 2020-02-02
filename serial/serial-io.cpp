
#include "serial-io.h"
using namespace std;

void relay_message(string sender_s, string content_s){

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
        //return -1;
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

}