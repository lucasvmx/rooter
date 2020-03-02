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

#include "console.h"
#include "file_io.h"

bool copy_file(const char *from, const char *to)
{
    FILE *in = NULL, *out = NULL;
    unsigned int ch;
    long bytes_to_copy;

    // Check if the file exists
    if((access(from, 0) == -1))
    {
        rooter_printf(ERROR, "The file '%s' doesn't exists\n", from);
        return false;
    }

    // Open the files
    in = fopen(from, "rb");
    if(!in)
        return false;

    out = fopen(to, "wb");
    if(!out)
    {
        fclose(in);
        return false;
    }

    // Obtain the file size
    fseek(in, 0, SEEK_END);
    bytes_to_copy = ftell(in);

    if(bytes_to_copy == -1)
    {
        rooter_printf(ERROR, "Failed to calculate file size\n");
        fclose(in);
        fclose(out);

        return false;
    }

    // Return to the begin of file
    fseek(in, 0, SEEK_SET);

    // Copy one to another
    for(long i = 0; i < bytes_to_copy; ++i)
    {
        ch = fgetc(in);
        fprintf(out, "%c", (ch > 0xff) ? 0:ch);
    }

    // Close the files
    fclose(in);
    fclose(out);

    return true;
}

bool move_file(const char *from, const char *to)
{
    bool bOk = false;

    // Copy the file
    bOk = copy_file(from, to);

    // Delete the older file
    remove(from);

    return bOk;
}
