/* 
 * File:   CMPNode.cpp
 * Author: a
 * 
 * Created on March 11, 2012, 8:58 PM
 */

#include "CMPSocket.h"

int sendall(int s, char *buf, int len)
{
  int total = 0;
  // how many bytes we've sent
  int bytesleft = len; // how many we have left to send
  int n;
  while(total < len) {
    n = send(s, buf+total, bytesleft, 0);
    if (n == -1) { break; }
    total += n;
    bytesleft -= n;
  }
  //*len = total; // return number actually sent here
  return n==-1?-1:0; // return -1 on failure, 0 on success
}


CMPMsg::CMPMsg() : data(0)
{
}

CMPSocket::CMPSocket(const sockaddr addr)
{
    if((sockServer=socket(addr.sa_family, SOCK_STREAM, 0))==-1)
        throw UNIXError();
    switch(addr.sa_family)
    {
        case AF_INET:
            if(connect(sockServer, &addr, sizeof(sockaddr_in))==-1)
                throw UNIXError();
            break;
        case AF_INET6:
            if(connect(sockServer, &addr, sizeof(sockaddr_in6))==-1)
                throw UNIXError();
            break;
        default:
            throw runtime_error("Unknown or unsupported address type. Options are INET or INET6");
    }
}
void CMPSocket::sendMsg(CMPMsg msg)
{
//     char* msgBuffer=new char[msg.len+9];
//     uint32_t temp=htonl(msg.addr);
//     memcpy(msgBuffer, &temp, 4);
//     memset(msgBuffer+4, msg.type, 1);
//     temp=htonl(msg.len);
//     memcpy(msgBuffer+5, &temp, 4);
//     memcpy(msgBuffer+9, msg.data, msg.len);
//     if(sendall(sockServer, msgBuffer, msg.len+9)==-1)
//     {
//       throw UNIXError();
//     }
  msg.addr=htonl(msg.addr);
  msg.len=htonl(msg.len);
  iovec vecs[]={{&msg.addr, 4}, {&msg.type, 1}, {&msg.len, 4}};
  if(writev(sockServer, vecs, 3)==-1)
    throw UNIXError();
  if(sendall(sockServer, msg.data.get(), ntohl(msg.len))==-1)
    throw UNIXError();
}

void CMPSocket::recvMsg(CMPMsg& out)
{
  int rvRead;
  iovec vecs[]={{&out.addr, 4}, {&out.type, 1}, {&out.len, 4}};
  if((rvRead=readv(sockServer, vecs, 3))<=0)
  {
    if(rvRead==0)
      throw runtime_error("Connection closed unexpectedly");
    else
      throw UNIXError();
  }
  out.addr=ntohl(out.addr);
  out.len=ntohl(out.len );
  out.data=shared_array<char>(new char[out.len]);
  if((rvRead=recv(sockServer, out.data.get(), out.len, MSG_WAITALL))<=0)
  {
    if(rvRead==0)
      throw runtime_error("Connection closed unexpectedly");
    else
      throw UNIXError();
  }
}

int CMPSocket::getSock()
{
  return sockServer;
}

CMPAddress CMPSocket::getCMPAddr()
{
  return addr;
}

void CMPSocket::closeSock()
{
  close(sockServer);
}



