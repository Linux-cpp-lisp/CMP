//System Includes
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <getopt.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <syslog.h>
//Standard Includes
#include <cstring>
//Local Includes
#include "CMPSocket.h"
#include "cmpnameserver-bits/cmpnameserver-nserver.h"
#include "cmpnameserver-bits/cmpnameserver-common-bits.h"
#include "fork-parent/fork-parent.h"
using namespace std;




void printHelp(int status)
{
  cout<<"Usage: cmpd [options]\n";
  cout<<"Options: \n   -h: Print this help.\n   -p: Set the port for the nameserver to run on."<<endl;
  exit(status);
}



int main(int argc, char** argv)
{
  int nopt;
  int ret;
  CMPNserverOpts opts;
  opts.port="4567";
  sighandler_t rvSig;
  const char* const short_opts="hp:";
  const option getopt_long_opts[]={
    {"help", 0, NULL, 'h'},
    {"port", 1, NULL, 'p'},
    {NULL, 0, NULL, 0},
  };
  do
  {
    nopt=getopt_long(argc, argv, short_opts, getopt_long_opts, NULL);
    switch(nopt)
    {
      case 'h':
	printHelp(0);
	break;
      case 'p':
	opts.port=optarg;
	break;
      case '?':
	printHelp(-1);
	break;
      case -1:
	break;
      default:
	cerr<<"FATAL: The function `getopt_long()' of the C library has returned a completly unexpected value. This is most likly a bug with your operating system's implementation of `getopt_long'.\n";
	cerr<<"   Please check and make sure that you are running a relativly modern version of a UNIX or POSIX compatable system, or that you are on a reasonably updated version of a UNIX emulator."<<endl;
	exit(-1);
    }
  } while(nopt != -1);
  cerr<<"Launching server..."<<endl;
  try
  {
    if(fork_parent()==0) //Child
    {
      runNserver(opts);
    }
    else //Parent, shouldn't happen, fork_parent() only returns on error.
    {
      cerr<<"FATAL: The `fork_parent()' call has returned in the parent. Abort."<<endl;
      exit(-1);
    }
  }
  catch(UNIXError e)
  {
    cerr<<"ERROR: Error # "<<e.getErrorNum()<<" with message \""<<e.what()<<"\", Abort."<<endl;
    exit(-1);
  }
}
