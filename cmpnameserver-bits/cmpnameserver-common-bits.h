#ifndef CMPNSERVER_COMMON_H
#define CMPNSERVER_COMMON_H
#include <sys/socket.h>
#include "../UNIXError.h"
struct CMPNserverOpts
{
  const char* port; //Port to listen or connect to.
};

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/*
 * These macros are used to easly wrap a POSIX system call with basic error handling.
 */
#define ERR(__statment, __retVar)  ((__retVar=(__statment))==-1 ? throw UNIXError() :  (__retVar))
#define ERR_GAI(__statment, __retVar)  ((__retVar=(__statment) != 0 ? throw GAIError(__retVar) :  (__retVar)))
#define ERR_ERRVAL(__statment, __retVar, __errval)  (((__retVar)=(__statment))==(__errval) ? throw UNIXError() :  (__retVar))
#endif

#define CMPListenBacklog 10