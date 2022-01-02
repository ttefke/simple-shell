/* sish.c - this file contains the implementation of a simple shell
            that provides only basic featurs: running commands and
            changing the directory. Advanced features, such as shell
            scripts, accessing or defining (environment) variables etc.
            are not supported. */

#include "include/cd.h"
#include "include/exec.h"
#include "include/sish.h"
#include "include/util.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/* main

Main function - start the program.
*/
int main(void)
{
    char* input;
    int input_length;
    pid_t pid;
    int status;
    char** arguments;

    while (TRUE)
    {
	    /* Ask for user input. */
        prompt();

        /* Read user input. */
        input = read_input();

        /* Terminate program if we can not read the input. */
        if (input == NULL) {
            perror("Could not read input");
            exit(EXIT_FAILURE);
        }

        /* Skip current iteration if the user only pressed enter. */
        if (strlen(input) == 0) {
            free(input);
            input = NULL;
            continue;
        }

        /* Terminate the program if the user requested so. */
        if (strcmp(input, "exit") == 0) {
            free(input);
            input = NULL;
            exit(EXIT_SUCCESS);
        }

        /* Split the user input into an array containing the
           program name as first element and the command-line
           arguments (one element for each argument). */
        arguments = get_arguments_from_input(input);

        /* Terminate the program if we can not split the input
           string into an array with the arguments. */
        if (arguments == NULL) {
            perror("Could not evaluate input");
            exit(EXIT_FAILURE);
        }

        /* cd is not a regular program, so we have to implement
           out own variant as built-in command. */
        if (strcmp(arguments[0], "cd") == 0) {
            cd(arguments);

        /* If no built-in command was called, call fork to get
           a copy of this process which's content we can replace
           then with exec if fork succeeded. */
        } else {
            /* Fork the process. */
            pid = fork();

            /* Handle fork errors. */
            if (pid < 0)
            {
                perror("Can not fork");
                continue;
            }

            /* Replace the process copy (child) using exec if
               forking succeeded. */
            if (pid == 0) {
                exec(arguments);

                /* Running exec failed if the code continues. */
                puts("Starting the new process failed.");
                _exit(EXIT_FAILURE);
            }

            /* Let the parent wait until the child process exited. */
            if (pid > 0) {
                status = 0;
                do {
                    waitpid(pid, &status, WUNTRACED);
#if DEBUG == 1
                    if (status < 1) {
                        perror("Parent process did not wait until child process exited");
                    }
#endif
                } while (
                    !WIFEXITED(status) &&
                    !WIFSIGNALED(status) &&
                    !WIFSTOPPED(status));
            }
        }

        /* Free allocated memory. */
        free(input);
        input = NULL;
    }
	return 0;
}
