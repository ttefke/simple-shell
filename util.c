/* util.c - this file contains implementations of different
            helper functions used within the program */

#include "include/util.h"

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <unistd.h>

/* prompt

Shows the input promt.
Consists of the user, the hostname,
the working directory and a dollar sign.
*/
void prompt(void)
{
    /* Get current user. */
    char* user = getenv("USER");

    /* Get hostname. */
    char* hostname = getenv("HOSTNAME");

    /* Get current working directory. */
    char* working_directory = get_working_directory();
    
    /* Show error if no memory for the current working directory
       could be allocated. Then, display an error message and
       only a dollar sign. */
    if (working_directory == NULL) {
        printf("$ ");
        return;
    }
    
    /* Prompt for user input. If username or hostname were not set
       trough environment variables, omit hostname or both user
       and hostname. */
    if (user == NULL) {
        /* Print current working directory. */
	    printf("%s$ ", working_directory);
    } else if ((user != NULL) && (hostname == NULL)) {
        /* Print current working directory with username. */
	    printf("%s:%s$ ", user, working_directory);
    } else {
        /* Print current working directory with user and hostname. */
	    printf("%s@%s:%s$ ", user, hostname, working_directory);
    }

	/* Free allocated memory. */
	free(working_directory);
	working_directory = NULL;
}

/* get_working_directory

Returns the current working directory as string.
In case the current directory can not be fetched,
the error message is printed and a null pointer is
returned instead.
*/
char* get_working_directory(void) {
    char* result = getcwd(NULL, 0);

    if (result == NULL) {
        perror("Could not get current working directory");
    }

    return result;
}

/* read_input

Reads input from the command line until either enter was pressed
or the maximum input length has been reached.
Not necessary spaces and tabs are skipped.
Returns a null pointer if no memory could be allocated
for reading the input string.
*/
char* read_input(void)
{
	char* input = (char*) malloc(INITIAL_INPUT_LENGTH);
	int position = 0;
	int input_buffer_length = INITIAL_INPUT_LENGTH;
	char current_char;
	bool last_input_was_letter = false;
	
	/* Return null pointer if the memory allocation failed. */
	if (input == NULL) {
	    perror("Could not read input");
	    return NULL;
	}
	
	do {
	    /* Get last pressed key. */
	    current_char = getwc(stdin);

	    /* Terminate the string if enter was pressed. */
	    if (current_char == '\n') {
	        input[position] = '\0';
	    /* Add a space or tab only to the input string
	       if a letter was entered before. */
	    } else if (isblank(current_char)) {
	        if (last_input_was_letter) {
	            input[position] = current_char;
	            position++;
	            last_input_was_letter = false;
	        }
	    } else {
	    /* Otherwise, add the character. */
	        input[position] = current_char;
	        position++;
	        last_input_was_letter = true;
	    }

	    /* Increase input buffer if necessary. */
	    if (position == input_buffer_length) {
	        input_buffer_length *= 2;
	        input = realloc(input, input_buffer_length);

	        /* Return null pointer if reallocation failed. */
	        if (input == NULL) {
	            perror("Could not read input");
	            return NULL;
	        }
	    }
	} while (current_char != '\n');

	return input;
}


/* get_nr_of_spaces

Counts the space characters in a given string and returns them
*/
int get_nr_of_spaces(char* input) {
    int result = 0;
    int i = 0;
    do {
        if (input[i] == ' ') {
            result++;
        }
        i++;
    } while(i <= strlen(input));
    return result;
}

/* get_arguments_from_input

Splits the input string into an array of strings.
The space symbol is used as delimiter here, so the string is cut
whereever a space is found.
This function is used for splitting the user input into an array.
The first element contains the program name.
The following elements contain the arguments handed over to the program.
If no arguments are handed over, the array contains only one element.
A null pointer is returned in case not enough memory for the
array can be allocated.
*/
char** get_arguments_from_input(char* input)
{
    const char delimiter[] = " ";
    char** result = (char**) malloc((get_nr_of_spaces(input) + 2) * sizeof(char**));
    int i = 0;
    char* part = strtok(input, delimiter);
    
    /* Return null pointer if the memory allocation failed. */
    if (result == NULL) {
        perror("Can not get input");
        return NULL;
    }
    
    /* Populate the array with the program and the comman-line options */
    do {
        result[i] = part;
        i++;
        part = strtok(NULL, delimiter);
    } while (part != NULL);
    result[i] = NULL;
    
    return result;
 }
