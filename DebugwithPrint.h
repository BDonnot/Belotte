#ifndef DEBUGWITHPRINT_H_INCLUDED
#define DEBUGWITHPRINT_H_INCLUDED

#include <stdarg.h>

template<int Print>
class WrapperPrint
{
    public :
    void operator()( const char * format, ... ){}
};

template<>
class WrapperPrint<1>
{
    public :
    void operator()( const char * format, ... )
    {
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stderr, format, argptr);
        va_end(argptr);
    }
};

#endif // DEBUGWITHPRINT_H_INCLUDED
