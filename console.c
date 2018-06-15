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

#include <stdio.h>
#include <stdarg.h>
#include "console.h"

int rooter_printf(int type, const char *text, ...)
{
	int len = 0;
	va_list list;
	
	va_start(list,text);
	
	switch(type)
	{
		case WARNING:
			fprintf(stdout, ANSI_COLOR_YELLOW "[*] " ANSI_COLOR_RESET);
			break;
		
		case ERROR:
			fprintf(stdout, ANSI_COLOR_RED "[!] " ANSI_COLOR_RESET);
			break;
		
		case INFO:
			fprintf(stdout, ANSI_COLOR_CYAN "[*] " ANSI_COLOR_RESET);
			break;
		
		case LOG:
			fprintf(stdout, ANSI_COLOR_MAGENTA "[*] " ANSI_COLOR_RESET);
			break;

		case OK:
			fprintf(stdout, ANSI_COLOR_GREEN "[+] " ANSI_COLOR_RESET);
			break;

		case PROMPT:
			fprintf(stdout, ANSI_COLOR_YELLOW "[PROMPT] " ANSI_COLOR_RESET);
			break;
	}
	
	len = vfprintf(stdout, text, list);
	
	return len;
}

void jump_line(FILE *p)
{
	if(p != NULL)
		fprintf(p,"\n");
}

