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

/*
    Requires glibc 2.1.95 or later
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/mount.h>
#include <stdbool.h>
#include <string.h>
#include <linux/limits.h>
#include <signal.h>

#define MAX_USERNAME_LEN    64
#define MAX_DEV_NAME_LEN    32
#define MAX_OUTPUT_LEN      1024

#ifdef DEBUG
    static const int debug = 1;
#else
    static const int debug = 0;
#endif

struct menu
{
	int dev_id;
	char device_path[MAX_DEV_NAME_LEN];
};

int main()
{
    char username[MAX_USERNAME_LEN];
    __id_t uid;
    const char *cmd = "ls /dev | grep sd > cmd.txt";
    FILE *file_pointer = NULL;
    char output_data[MAX_OUTPUT_LEN] = {0};
	int id = 0;
	struct menu m[128];
	int selected_id;
	char id_buff[4];
	int mount_result;
    char working_dir[PATH_MAX] = {0};
    char last_dir[PATH_MAX] = {0};
    const char *temp_folder = "root_temp_folder";
    char buff[PATH_MAX]={0}; /* For generic usage */
    char sudoers_path[PATH_MAX] = {0};
    char new_sudoers_path[PATH_MAX] = {0};
	char shell_cmd[PATH_MAX] = {0};	/* Used to store shell commands */

    uid = getuid();
    if(uid != 0 && !debug)
    {
        fprintf(stdout, "Sorry. We need root privileges :(\nDon't forget to run this program from a live usb OS\n\n");
        return 1;
    }

    if(debug)
        fprintf(stdout, "Running in debug mode ...\n");

    /* Check if temporary folder exists */
    if(access(temp_folder,00) != -1)
    {
        snprintf(shell_cmd, PATH_MAX, "umount %s && rmdir %s", temp_folder, temp_folder);
        system(shell_cmd);
    }

    fprintf(stdout, "Give-me your username (Warning: max USERNAME length is 64)\n");
    fgets(username, MAX_USERNAME_LEN, stdin);
    fprintf(stdout, "Tell me the name of the device where linux is installed.\nSelect one of the list below:\n");
    system(cmd);

    file_pointer = fopen("cmd.txt", "r");
    if(file_pointer == NULL)
    {
        fprintf(stdout, "Failed to get devices installed in this machine ...\n");
        return 1;
    }

    sleep(1);

	while(fgets(output_data, MAX_OUTPUT_LEN, file_pointer) != NULL)
    {
    	fprintf(stdout, "%d - /dev/%s", id, output_data);
    	snprintf(m[id].device_path, MAX_DEV_NAME_LEN, "/dev/%s", output_data);
    	m[id].dev_id = id;
    	id++;
    }

    fclose(file_pointer);
    remove( "cmd.txt");

    fprintf(stdout, "\n");
	fprintf(stdout, "Select one (default is 1): ");
	selected_id = getchar();
	snprintf(id_buff, (sizeof(id_buff) / sizeof(id_buff[0])), "%c", selected_id);
	selected_id = (int)strtol(id_buff, NULL, 10); /* WARNING: We are inserting a Long to an int */
	fprintf(stdout, "You have selected %d which is %s", selected_id, m[selected_id].device_path);
    fprintf(stdout, "Trying to give root access to %s", username);

	if(access(temp_folder, 00) == -1)
	{
        fprintf(stdout, "Creating temporary directory ...\n");
        snprintf(shell_cmd, sizeof(shell_cmd), "mkdir %s", temp_folder);
        system(shell_cmd);
        sleep(1);

        if(access(temp_folder, 00) == -1)
        {
            perror( "Failed to create directory");
            return 1;
        }
	}

	/* Remove '\n' character from the end of vector */
	char device[PATH_MAX] = {0};
	int len = strlen(m[selected_id].device_path);

	for(int i = 0; i < len; i++)
	{
		if(m[selected_id].device_path[i] == '\n')
		{
			device[i] = '\0';
			break;
		}

		device[i] = m[selected_id].device_path[i];
	}

	mount_result = mount(device, temp_folder, NULL, 0, NULL);
	if(mount_result == -1)
	{
		snprintf(shell_cmd, PATH_MAX, "mount %s %s", device, temp_folder);
		if(debug)
		{
			fprintf(stdout, "Running: %s\n", shell_cmd);
		}

		system(shell_cmd);
		snprintf(shell_cmd, PATH_MAX, "%s/etc", temp_folder);
		if(access(shell_cmd, 00) != -1)
			mount_result = 0;

        __fpurge(stdin); /* Required to call getchar(). Otherwise, getchar will be ignored */

		if(mount_result == -1)
		{
			fprintf(stdout, "Run shell and type this command: mount %s %s\n", device,temp_folder);
            fprintf(stdout, "Press enter when you finish.");
            getchar();

			snprintf(buff, PATH_MAX, "%s/etc/sudoers", temp_folder);
			if(debug)
				fprintf(stdout, "Checking if %s exists.\n", buff);

			if(access(buff, 00) == -1)
			{
				fprintf(stdout, "Mount failed again. :(\n");
				snprintf(shell_cmd, PATH_MAX, "rmdir %s", temp_folder);
				system(shell_cmd);
			} else {
				mount_result = 0;
			}
		}
	}

	if(mount_result == -1)
	{
		perror( "Failed to mount" );
	} else
    {
        fprintf(stdout, "Mounted %s\n", device);
        if(getcwd(last_dir, sizeof(last_dir)) == NULL) /* Saving the path of the last folder ... */
        {
            perror( "Failed to get current directory");
            return 1;
        }

        if(debug)
        {
            if(NULL != getcwd(working_dir, sizeof(working_dir)))
                fprintf(stdout, "(%d): Current directory: %s\n", __LINE__, working_dir);

            fprintf(stdout, "(%d): Last directory: %s\n", __LINE__, last_dir);
        }

        fprintf(stdout, "Trying to give root ...\n");

        /* Build sudoers file path */
        snprintf(sudoers_path, sizeof(sudoers_path), "%s/%s/etc/sudoers", working_dir, temp_folder);
        snprintf(new_sudoers_path, sizeof(new_sudoers_path), "%s/%s/etc/sudoers_new", working_dir, temp_folder);

        if(access(sudoers_path,00) == -1)
        {
            fprintf(stdout, "The file %s does not exists.\n", sudoers_path);
            return 1;
        }

        FILE *sudoers = NULL;
        FILE *new_sudoers = NULL;
        FILE *backup_file = NULL;

        char line[PATH_MAX];
        const char *root_line = "root ALL=(ALL) ALL";
        const int size = strlen(root_line);
        bool bRoot = false;
		char backup_file_path[PATH_MAX];

		memset(line,0,PATH_MAX);
		memset(backup_file_path,0,PATH_MAX);

        sudoers = fopen(sudoers_path, "r");
        if(sudoers == NULL)
        {
            perror( "Failed to open file for read");
            return 1;
        }

        new_sudoers = fopen(new_sudoers_path, "w");
        if(new_sudoers == NULL)
        {
            perror( "Failed to open file for write");
            return 1;
        }

		snprintf(backup_file_path, PATH_MAX, "%s/%s/etc/sudoers_bak", working_dir,temp_folder);
		fprintf(stdout, "Creating backup file: %s\n", backup_file_path);

		backup_file = fopen(backup_file_path, "w");
        if(backup_file == NULL)
        {
            fprintf(stdout, "Failed to create backup file\n");
			if(new_sudoers)
				fclose(new_sudoers);
			if(sudoers)
				fclose(sudoers);

			return 1;
        }

		fprintf(stdout, "Parsing data ...\n");
        while(fgets(line, PATH_MAX, sudoers) != NULL)
        {
            fprintf(backup_file, "%s", line);

            if(strncmp(line, root_line, size) == 0)
            {
                fprintf(new_sudoers, "%s ALL=(ALL) ALL\n", username);
                bRoot = true;
            }

            fprintf(new_sudoers, "%s", line);
        }

		if(new_sudoers)
        	fclose(new_sudoers);
        if(sudoers)
			fclose(sudoers);

		if(backup_file)
			fclose(backup_file);

        sleep(1);

        //system( "mv new_sudoers sudoers");

        if(bRoot) {
            fprintf(stdout, "It's all done. Reboot into your native OS, open shell and type: 'sudo -s'. Give your password, and you'll be root. :)\n");
		}
        else {
            fprintf(stdout, "Failed to get root. I'm sorry :(\n");
		}
    }

    return 0;
}
