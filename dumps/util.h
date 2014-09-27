/*
Check the LICENSE file included for copyright information.  
*/
#ifndef __UTIL__
#define __UTIL__
#include <sys/stat.h>

#define USERDATA( str ) "./" str

inline bool file_exists( const std::string& name ) 
{
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

#endif
