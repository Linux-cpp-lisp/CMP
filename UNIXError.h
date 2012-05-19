/* 
 * File:   UNIXError.h
 * Author: a
 *
 * Created on March 19, 2012, 5:59 PM
 */

#ifndef UNIXERROR_H
#define	UNIXERROR_H
#include <errno.h>
#include <stdexcept>
#include <cstring>
using namespace std;

class UNIXError : public runtime_error {
public:
    UNIXError();
    explicit UNIXError(int errno_c);
    int getErrorNum();
protected:
    explicit UNIXError(const char* msg, int e);
    int _error_num;
};

class GAIError : public UNIXError {
public:
    GAIError(int gai_eint);
};

#endif	/* UNIXERROR_H */

