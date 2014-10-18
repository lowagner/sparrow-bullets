/*
Check the LICENSE file included for copyright information.  
*/
#include "util.h"
#include "../csparrow.h"
#include <sys/stat.h>


bool file_exists( const std::string& name ) 
{
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

bool file_exists( const char* name ) 
{
    struct stat buffer;   
    return (stat (name, &buffer) == 0); 
}


 /* The following utility functions were borrowed from snowman.
  * 
  * Snowman is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 2 of the License, or
  * (at your option) any later version.
  * 
  * Snowman is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  * 
  * You should have received a copy of the GNU General Public License
  * along with snowman.  If not, see <http://www.gnu.org/licenses/>
  * 
  * For feedback and questions about my Files and Projects please mail me,
  * Alexander Matthes (Ziz) , zizsdl_at_googlemail.com */

#if defined GCW || (defined X86CPU && !defined WIN32)
char* get_path(char* buffer,char* file)
{
    sprintf(buffer,"%s/.config/cubedump/%s",getenv("HOME"),file);
    return buffer;
}
#else
char* get_path(char* buffer,char* file)
{
    sprintf(buffer,"./%s",file);
    return buffer;
}
#endif

void savelevelsettimelives( char* levelname, float time, int livesleft )
{
    char buffer[256];
    spCreateDirectoryChain(get_path(buffer,""));
    SDL_RWops *file=SDL_RWFromFile(get_path(buffer,levelname),"wb");
    SDL_RWwrite(file,&time,sizeof(float),1);
    SDL_RWwrite(file,&livesleft,sizeof(int),1);
    SDL_RWclose(file);
}

void loadlevelsettimelives( char* levelname, float& time, int& livesleft )
{
    char buffer[256];
    SDL_RWops *file=SDL_RWFromFile(get_path(buffer,levelname),"rb");
    if (file==NULL) //Compatibility
        return;
    SDL_RWread(file,&time,sizeof(float),1);
    SDL_RWread(file,&livesleft,sizeof(int),1);
    SDL_RWclose(file);
}

void savetime( char* level, float t )
{
    if (level == NULL || level[0] == 0)
        return;
    char buffer[256];
    spCreateDirectoryChain(get_path(buffer,""));
    SDL_RWops *file=SDL_RWFromFile(get_path(buffer,level),"wb");
    SDL_RWwrite(file,&t,sizeof(float),1);
    SDL_RWclose(file);
}

float loadtime( char* level )
{
    char buffer[256];
    SDL_RWops *file=SDL_RWFromFile(get_path(buffer,level),"rb");
    if (file==NULL)
        return 1000.f; // evil time
    float t;
    SDL_RWread(file,&t,sizeof(float),1);
    SDL_RWclose(file);
    return t;
}

 

