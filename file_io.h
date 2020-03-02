/*
    This file is part of rooter.

    Rooter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Rooter is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Rooter.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

/**
 * @brief 
 * 
 * @param from 
 * @param to 
 * @return true 
 * @return false 
 */
extern bool copy_file(const char *from, const char *to);

/**
 * @brief 
 * 
 * @param from 
 * @param to 
 * @return true 
 * @return false 
 */
extern bool move_file(const char *from, const char *to);

#endif