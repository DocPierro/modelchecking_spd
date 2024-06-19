//
//  SocketPort.cpp
//  Cosmos
//
//  Created by Alexandru Mereacre on 26/02/2015.
//
//

#include "SocketPort.h"
#include <string.h>

bool CreateSocket(int *socketHandle, struct addrinfo *hostInfoList)
{
    struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
    
    // Set up the server
    memset(&host_info, 0, sizeof host_info);
    
    host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
    host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
    host_info.ai_flags = AI_PASSIVE;     // IP Wildcard
    
    // Now fill up the linked list of host_info structs with localhost address information.
    int sockStatus = getaddrinfo(NULL, PORT_NUMBER, &host_info, &hostInfoList);
    
    // getaddrinfo returns 0 on succes, or some other value when an error occured.
    // (translated into human readable text by the gai_gai_strerror function).
    if (sockStatus != 0) {
        print("Error: getaddrinfo\n");
        return 0;
    }
    
    print("Creating a server socket\n");
    
    *socketHandle = socket(hostInfoList->ai_family, hostInfoList->ai_socktype,
                             hostInfoList->ai_protocol);
    if (*socketHandle == -1) {
        print("Error: socket error\n");
        return 0;
    }
    
    // we use to make the setsockopt() function to make sure the port is not in use
    // by a previous execution of our code. (see man page for more information)
    int someYes = 1;
    if(setsockopt(*socketHandle, SOL_SOCKET, SO_REUSEADDR, &someYes, sizeof(someYes))==-1) {
        print("Reuse port Error\n");
        return 0;
    }
    
    sockStatus = bind(*socketHandle, hostInfoList->ai_addr, hostInfoList->ai_addrlen);
    if (sockStatus == -1) {
        print("Error: socket bind\n");
        return 0;
    }
    
    print("Listen()ing for Ev connections\n");
    sockStatus =  listen(*socketHandle, 5);
    
    if (sockStatus == -1) {
        print("Error: listen error\n");
        return 1;
    }

    return 1;
}

void CloseSocket(int *socketHandle, int *socketListenHandle, struct addrinfo *hostInfoList)
{
    freeaddrinfo(hostInfoList);
    close(*socketHandle);
    close(*socketListenHandle);
    
    print("Socket closed\n");
}

bool MainSocketRead(struct ThreadSerialInfo *sInfo)
{
    struct pollfd   fds;
    int             pollrc;

    fds.fd = sInfo->gSocketHandle;
    fds.events = POLLIN;
    
    while(sInfo->gEndThread) {
        pollrc = poll(&fds, 1, -1);
        
        
        // Check if bytes are available in the read queue
        if (pollrc < 0) {
            print("Error: setting the poll\n");
            return 0;
        }else if(pollrc > 0) {
            if (fds.revents & POLLIN && fds.fd==sInfo->gSocketHandle) {
                sInfo->gListenSocketHandle = accept(sInfo->gSocketHandle, NULL, NULL);
                
                if (sInfo->gListenSocketHandle < 0) {
                    if (errno != EWOULDBLOCK)
                        print("Error: accept() failed");
                    return 0;
                }
                
                print("Client socket connected\n");
                
                fds.fd = sInfo->gListenSocketHandle;
                fds.events = POLLIN;
            } else if (fds.revents & POLLIN && fds.fd==sInfo->gListenSocketHandle) {
                
                unsigned char dataSocket[8] = {0, 0, 0, 0, 0, 0, 0, 0};
                ssize_t rc = recv(fds.fd, (void*)&dataSocket, 8, 0);
                
                if (rc < 0) {
                    if (errno != EWOULDBLOCK)
                        print("Error:  recv() failed\n");
                    return 0;
                }
                if (rc == 0) {
                    print("Connection closed\n");
                    sInfo->pmySim->StopSimulation();
                    sInfo->gCommands = SIM_END;
                    sInfo->gEndThread = 0;                    
                    return 1;
                }
                
                if(dataSocket[0]==SIM_END) {
                    print("Ending simulation\n");
                    sInfo->pmySim->StopSimulation();
                    sInfo->gCommands = SIM_END;
                    sInfo->gEndThread = 0;
                    return 1;
                } else if(dataSocket[0]==SIM_STOP) {
                    print("Received simulation stop\n");
                    sInfo->pmySim->StopSimulation();
                    sInfo->gCommands = SIM_STOP;
                    
                } else if(dataSocket[0]==SIM_START) {
                    print("Received simulation start\n");
                    sInfo->gCommands = SIM_START;
                    
                } else if(dataSocket[0]==SIM_GET_ID) {
                    print("Received get ID\n");
                    sInfo->gCommands = SIM_GET_ID;
                                                   
                } else if (dataSocket[0]==SIM_SET_CPAR) {
                    print("Received set client parameter\n");
                    
                    sInfo->gParId = dataSocket[1];
                    memcpy((void*)&sInfo->gParBuf[0], (const void*)&dataSocket[4], 4);
                    
                    sInfo->gCommands = SIM_SET_CPAR;
                    
                } else if (dataSocket[0]==SIM_SET_PPAR) {
                    print("Received set arduino parameter\n");
                    
                    sInfo->gParId = dataSocket[1];
                    memcpy((void*)&sInfo->gParBuf[0], (const void*)&dataSocket[4], 4);

                    sInfo->gCommands = SIM_SET_PPAR;
                    
                } else
                    sInfo->gCommands = SIM_NONE;
                
            } else if(fds.revents & POLLHUP || fds.revents & POLLERR) {
                print("Error: socket poll read\n");
                return 0;
            }
        }
    }
    
    return 1;
}
