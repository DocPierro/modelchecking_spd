//
//  SerialPort.h
//  Cosmos
//
//  Created by Alexandru Mereacre on 12/02/2015.
//
//

#ifndef __Cosmos__SerialPort__
#define __Cosmos__SerialPort__

#include <stdio.h>
#include <termios.h>
#include <sysexits.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#include "Print.h"
//#include "macro.hpp"

void ClosePortDevice(int *portHandle, struct termios  *tio);
void SetDefaultPortSettings(int *portHandle, struct termios  *tio);
bool WriteToPort(int *portHandle, int nbytes, unsigned char *buf);

void ProcessSerial(struct ThreadSerialInfo *sInfo);

#endif /* defined(__Cosmos__SerialPort__) */
