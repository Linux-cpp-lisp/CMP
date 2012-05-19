#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "CMPSocket.h"
using namespace std;
int main()
{
  try
  {
//   sockaddr_in addr;
//   memset(&addr, 0, sizeof(addr));
//   cout<<inet_pton(AF_INET, "127.0.0.1", &addr)<<endl;
//   addr.sin_family=AF_INET;
//   addr.sin_port=4567;
  int rvGAI;
  char buffer[]="Hello World";
  addrinfo hints;
  addrinfo* servinfo;
  memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family=PF_INET;
  hints.ai_socktype=SOCK_STREAM;
  hints.ai_flags=AI_PASSIVE;
  if((rvGAI=getaddrinfo(NULL, "4567", &hints, &servinfo))!=0)
  {
    cerr<<gai_strerror(rvGAI)<<endl;
    exit(-1);
  }
  CMPSocket s(*servinfo->ai_addr);
  CMPMsg msg;
  msg.addr=78;
  msg.data=shared_array<char>(new char[strlen(buffer)]);
  memcpy(msg.data.get(), buffer, 11);
  msg.len=strlen(buffer);
  msg.type=CMPMsgInternalTypeAddred;
  s.sendMsg(msg);
  cerr<<"AFter sendMsg"<<endl;
  CMPMsg res;
  s.recvMsg(res);
  //[0, 0, 0, 78, 0, 0, 0, 0, 11, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
  cout<<"Msg recived: Addr: "<<res.addr<<" Type: "<<res.type<<" Length: "<<res.len;
  cout.write(res.data.get(), res.len);
  cout<<endl;
  }
  catch(UNIXError e)
  {
    cerr<<"UNIXError caught. Error Number "<<e.getErrorNum()<<" with error string: "<<e.what()<<endl;
    exit(-1);
  }
}