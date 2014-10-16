/*
Check the LICENSE file included for copyright information.  
*/
#ifndef __UTIL__
#define __UTIL__
#include <string>

bool file_exists( const std::string& name );
bool file_exists( const char* name );

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

char* get_path( char* buffer,char* file );

void savelevelsettimelives( char* levelname, float time, int livesleft );

void loadlevelsettimelives( char* levelname, float& time, int& livesleft );

void savetime( char* level, float t );

float loadtime( char* level );

#endif 
