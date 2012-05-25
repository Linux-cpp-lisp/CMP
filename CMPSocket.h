/* 
 * File:   CMPNode.h
 * Author: a
 *
 * Created on March 11, 2012, 8:58 PM
 */

#ifndef CMPSOCKET_H
#define CMPSOCKET_H
//-----Internet Includes:
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
//-----System includes (generic):
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
//--Darwin specific includes:
#ifdef Darwin
#include <sys/uio.h>
#endif
//-----Standard Includes:
#include <iostream>
//-----Boost Includes:
#include <boost/smart_ptr.hpp>
//-----Local includes:
#include "UNIXError.h"

#define CMPAddressCore 0
#define CMPAddressLoopback 1

#define CMPMsgInternalTypeAddred 0
#define CMPMsgInternalTypeUnaddred 1

#define CMPMsgInternalHeaderLen 9

using namespace std;
using namespace boost;

typedef uint32_t CMPAddress;



struct CMPMsg
{
public:
  CMPMsg();
  CMPAddress addr;
  char type;
  shared_array<char> data;
  uint32_t len;
};
    
int sendall(int s, char *buf, int len);

class CMPSocket {
public:
    CMPSocket(const sockaddr addr);
    void sendMsg(CMPMsg msg);
    void recvMsg(CMPMsg& out);
    bool setReciveUnaddressedMsgs(bool arg);
    int getSock();
    CMPAddress getCMPAddr();
    void closeSock();
private:
    int sockServer;
    CMPAddress addr;
};
#endif	/* CMPNODE_H */

