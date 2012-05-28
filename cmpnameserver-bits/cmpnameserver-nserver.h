#ifndef CMPNSERVER_H
#define CMPNSERVER_H
//System Includes
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <getopt.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <sysexits.h>
#include <syslog.h>
//Standard Includes
#include <cstring>
#include <map>
//Local Includes
#include "../CMPSocket.h"
#include "cmpnameserver-common-bits.h"

static int server;

void runNserverDetached(int);

void sigchldHandler(int sig)
{
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

void sigHandler(int sig)
{
  syslog(LOG_WARNING, "Signal (# %i, %s) caught and ignoring.", sig, strsignal(sig));
}

void sigtermHandler(int)
{
  syslog(LOG_INFO, "SIGTERM caught, terminating...");
  close(server);
}

void runNserver(CMPNserverOpts opts)
{
  cerr<<"INFO: Server started as PID "<<getpid()<<"."<<endl;
  try
  {
    int yes=1;
    int ret=0;
    addrinfo hints, *serverinfo, *p;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;
    ERR_GAI(getaddrinfo(NULL, opts.port, &hints, &serverinfo), ret);
    for(p=serverinfo; p != NULL; p=p->ai_next)
    {
      if((server=socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
      {
	cerr<<"WARNING: While atemping to create socket with one of the addresses, \""<<strerror(errno)<<"\"."<<endl;
	continue;
      }
      ERR(setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))==-1, ret);
      if(bind(server, p->ai_addr, p->ai_addrlen)==-1)
      {
	close(server);
	cerr<<"WARNING: While atemping to bind socket with one of the addresses, \""<<strerror(errno)<<"\"."<<endl;
	continue;
      }
      break;
    }
    if(p==NULL)
    {
      cerr<<"FATAL: Creating and/or binding the server failed. Abort."<<endl;
      exit(-1);
    }
    freeaddrinfo(serverinfo);
    ERR(listen(server, CMPListenBacklog), ret); 
    sighandler_t rvSig;
    ERR_ERRVAL(signal(SIGCHLD, sigchldHandler), rvSig, SIG_ERR);
    ERR_ERRVAL(signal(SIGHUP, sigHandler), rvSig, SIG_ERR);
    ERR_ERRVAL(signal(SIGTERM, sigHandler), rvSig, SIG_ERR);
    ERR_ERRVAL(signal(SIGINT, sigHandler), rvSig, SIG_ERR);
    openlog("cmpd", LOG_PID, LOG_DAEMON);
    cerr<<"Server setup complete, detaching..."<<endl;
    close(STDERR_FILENO);
    runNserverDetached();
  }
  catch(UNIXError e)
  {
    cerr<<"ERROR: Error with # "<<e.getErrorNum()<<" and message \""<<e.what()<<"\", Abort"<<endl;
    exit(-1);
  }
}

void runNserverDetached()
{
  try
  {
    socklen_t sin_size=sizeof(sockaddr_storage);
    sockaddr_storage them;
    int client;
    int rv;
    int fd_max=server;
    char addr[INET6_ADDRSTRLEN];
    fd_set master, temp;
    FD_ZERO(master);
    FD_ZERO(temp);
    FD_SET(server, master);
    while(true)
    {
	temp=master;
	sin_size=sizeof(sockaddr_storage);
	ERR(select(fd_max, temp, NULL, NULL, NULL), rv);
	if(FD_ISSET(server, master))
	{
	  //Got new connection
	  ERR(accept(server, (sockaddr*)them, &sin_size), client);
	  if(client > fd_max-1)
	    fd_max=client+1;
	  FD_SET(client, master);
	}
	else
	{
	  //Input from a client
	}
//      sin_size=sizeof(them);
//      ERR(accept(server, (sockaddr*)&them, &sin_size), client);
//      inet_ntop(them.ss_family, get_in_addr((sockaddr*)&them), addr, sizeof(addr));
//      syslog(LOG_INFO, "Got new connection from %s.", addr);
//      FD_SET(client, master);
    }
  }
  catch(UNIXError e)
  {
    syslog(LOG_CRIT, "Exception reached outermost catch with error # %i and message \"%s\". Aborting.", e.getErrorNum(), e.what());
    exit(EX_OSERR);
  }
}

#endif
