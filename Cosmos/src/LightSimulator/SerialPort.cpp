//
//  SerialPort.cpp
//  Cosmos
//
//  Created by Alexandru Mereacre on 12/02/2015.
//
//

#include <string.h>

#include "SerialPort.h"

static pthread_mutex_t gSerialWriteLock = PTHREAD_MUTEX_INITIALIZER;

void ClosePortDevice(int *portHandle, struct termios  *tio)
{
    /* Cleanup */
    tcdrain(*portHandle);
    tcsetattr(*portHandle,TCSANOW,tio);
    close(*portHandle);
    
    print("Closed serial port\n");
}

void SetDefaultPortSettings(int *portHandle, struct termios  *tio)
{
    struct termios newtio;
    
    ioctl(*portHandle, TIOCEXCL);
    fcntl(*portHandle, F_SETFL, 0);
    
    /* Set the baud rate and other serial port parameters */
    tcgetattr(*portHandle,tio); /* save current port settings */
    memset (&newtio, 0, sizeof newtio);
    
    newtio.c_iflag=0;
    newtio.c_oflag=0;
    newtio.c_cflag=CS8 | CREAD | CLOCAL;           /* 8n1, see termios.h for more information */
    newtio.c_lflag=0;
    newtio.c_cc[VMIN]=1;
    newtio.c_cc[VTIME]=0;
    
    cfsetospeed(&newtio,B57600);
    cfsetispeed(&newtio,B57600);
    
    tcflush(*portHandle, TCIOFLUSH);
    tcsetattr(*portHandle,TCSANOW,&newtio);

}

bool WriteToPort(int *portHandle, int nbytes, unsigned char *buf)
{
    ssize_t retBytes = -1;
    
    pthread_mutex_lock( &gSerialWriteLock );
    retBytes = write(*portHandle, buf, nbytes);
    pthread_mutex_unlock( &gSerialWriteLock );
    
    return (retBytes==-1) ? 0 : 1;
}

void ProcessSerial(struct ThreadSerialInfo *)
{

}
