/* cd.c - this file contains a basic implementation of the
          'cd' builtin command */

#include "include/cd.h"
#include "include/util.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* cd

Change the directory to the destination specified
in the second element of the arguments array
(first elment contains the name of the command ('cd')).
*/
void cd(char** arguments)
{
    int success = -1;
    char* destination;
    char* home_directory;
    char* working_directory;
    char* destination_directory;
    int path_length;

    /* Test whether cd was called without arguments, this results
       in changing the directory to the home directory. */
    if (arguments[1] == NULL) {
        home_directory = getenv("HOME");
        if (home_directory == NULL) {
            puts("Can not cd to $HOME: environment variable not defined.");
        } else {
            success = chdir(home_directory);

            /* Show error message if an error occured. */
            if (success == -1) {
                perror(ERROR_STRING);
            }
        }
    } else {
        destination = arguments[1];

        /* Check if a single dot was entered as parameter,
           a single dot points to the current dir, so return. */
        if ((strlen(destination) == 1) && (destination[0] == '.')) {
            free(arguments);
            arguments = NULL;
            return;

        /* If the first character of the destination is a slash,
           an absolute path was entered and we can directly call chdir. */
        } else if (destination[0] == '/') {
            success = chdir(destination);

            /* Show error message if an error occured. */
            if (success == -1) {
                perror(ERROR_STRING);
            }
        /* Otherwise, a relative path was entered. Then, we have
           to calculate the new path and call chdir afterwards. */
        } else {
            /* Get current directory first. */
            working_directory = get_working_directory();

            /* Return if no memory for storing the path of the
               current working directory could be allocated.
               Free the other allocated memory before. */
            if (working_directory == NULL) {
                free(arguments);
                arguments = NULL;
                return;
            }

            /* Allocate memory for the new path. */
            path_length = strlen(working_directory) + strlen(destination) + 2;
            destination_directory = (char*) malloc(path_length);

            /* Return if no memory for storing the new path
               could be allocated.
               Free the other allocated memory before. */
            if (destination_directory == NULL) {
                perror(ERROR_STRING);
                free(arguments);
                arguments = NULL;
                free(working_directory);
                working_directory = NULL;
                return;
            }

            /* Compute the new absolute path from the current working
               directory and the relative path. */
            strcpy(destination_directory, working_directory);
            strcat(destination_directory, "/");
            strcat(destination_directory, destination);
            success = chdir(destination_directory);

            /* Show error message if an error occured. */
            if (success == -1) {
                perror(ERROR_STRING);
            }

            /* Free allocated memory. */
            free(working_directory);
            working_directory = NULL;
            free(destination_directory);
            destination_directory = NULL;
        }
    }

    /* Free allocated memory. */
    free(arguments);
    arguments = NULL;
}
