/* 
 * File:   UNIXError.cpp
 * Author: a
 * 
 * Created on March 19, 2012, 5:59 PM
 */

#include "UNIXError.h"
#include <netdb.h>

UNIXError::UNIXError() : runtime_error(strerror(errno))
{
  _error_num=errno;
}
UNIXError::UNIXError(int errno_c) : runtime_error(strerror(errno_c))
{
  _error_num=errno;
}
int UNIXError::getErrorNum()
{
    return _error_num;
}

UNIXError::UNIXError(const char* msg, int e): runtime_error(msg)
{
  _error_num=e;
}


GAIError::GAIError(int gai_eint) : UNIXError(gai_strerror(gai_eint), gai_eint)
{
}
