//
//  Module.h
//  
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Module_h
#define _Module_h

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <math.h>
#include "lo.h"
#include "portaudio.h"
#include "RoutingTable.h"
#include "Data.h"
#include "Audio.h"
#include "Server.h"

#define SAMPLE_RATE	44100.0

class Module
{
public:
    char                IPAddr[16];
    char                OSCAddr[64];
    Server              *st;
    RoutingTable        *rTable;
    int					tID, mColor;
    
    Module(Server *s, const char *osc);
    ~Module();
    
    void setTID(int t);
    void setRoute(char *ip, char *osc);
    void addRoute(char *ip, char *osc);
    void deleteRoute(char *ip, char *osc);
	void deleteAllRoute(char *osc);
    void addMethodToServer(const char *path, const char *type, lo_method_handler h, void *user_data);
	void deleteMethodFromServer(const char *path, const char *type);
    void sendData(Data *d, lo_arg **argv, int argc);
    void sendAudio(Audio *a);
    void sendAudio(Audio *a1, Audio *a2);
    void sendAudio(lo_blob b1, lo_blob b2);
    void sendAudio(Audio *a1, Audio *a2, Audio *a3);
    void sendAudio(Audio *a1, Audio *a2, Audio *a3, Audio *a4);

    void sendAudio(short *a, unsigned long l);
    void sendSetMdtkn();
    void sendDeleteMdtkn();
	void connectTo(Module *m, const char *t);
	void disconnectFrom(Module *m, const char *t);

private:
    void getAddr();
    void setOSCAddr(const char *osc);


    static int setRoute(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static int addRoute(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static int deleteRoute(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
	static int deleteAllRoute(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);

};

#endif
