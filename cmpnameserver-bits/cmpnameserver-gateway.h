#ifndef CMPGATE_H
#define CMPGATE_H
//System Includes
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <getopt.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <syslog.h>
//Standard Includes
#include <cstring>
//Local Includes
#include "../CMPSocket.h"
#include "cmpnameserver-common-bits.h"

void runGatway(CMPNserverOpts opts)
{
  if(opts.areAddrs == 0)
  {
    
  }
}
#endif