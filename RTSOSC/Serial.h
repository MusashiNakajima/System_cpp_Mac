#ifndef _Serial_h
#define _Serial_h

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <pthread.h>
#include "Module.h"

#define BAUDRATE B9600           
#define MODEMDEVICE "/dev/cu.usbserial-A600afyl"
#define _POSIX_SOURCE 1

#define FALSE 0
#define TRUE 1

class Serial : public Module {

public:
    int         fd, c, res;
    volatile int active;
	volatile int done;
    pthread_t   thread;
    struct      termios oldtio,newtio;
    char        buf[1024];
	int			rp, wp;
    char        device[128];
    
    Serial(Server *s, const char *osc);
    Serial(Server *s, const char *osc, const char *d);

    ~Serial();
    void threadStop();
    bool prepareSerial();
    void threadStart();
    void setDevice(const char *d);
	void serialWrite(void *data, int s);

private:
    static int sWrite(const char   *path, 
                      const char   *types, 
                      lo_arg       **argv, 
                      int          argc,
                      void         *data, 
                      void         *user_data);

    static void *threadFunction(void *data);
    
};

#endif
