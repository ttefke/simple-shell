/* exec.c - this file contains a wrapper for calling the
            exec() syscall in order to replace the current
            process */

#include "include/exec.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* exec

Runs the program handed over with the string array.
The array handed over contains the commands with its arguments.
*/
void exec(char** command)
{
    int execution_result = -1;

    /* Run the entered command. */
    execution_result = execvp(command[0], command);

    /* Show error message if an error occured. */
    if (execution_result == -1) {
        fprintf(stderr, "Could not run %s: %s\n", command[0], strerror(errno));
    }

    /* Free allocated memory. */
    free(command);
    command = NULL;
}
