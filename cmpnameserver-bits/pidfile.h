#ifndef PIDFILE_H_
#define PIDFILE_H_
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include "cmpnameserver-common-bits.h"
extern const char* progName;
bool tryPidfile()
{
  int fd;
  char* path=new char[13+strlen(progName)];
  sprintf(path, "/var/run/%s.pid", progName);
  if((fd=open(path, O_CREAT | O_EXCL))==-1)
  {
    return false;
  }
  else
  {
    FILE* f=fdopen(fd, "w");
    fprintf(f, "%i\n", getpid());
    fclose(f);
    return true;
  }
}
#endif

//    /var/run/.pid