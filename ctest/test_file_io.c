#include <stdio.h>
#include <stdlib.h>
#include "file_io.h"

int main(int argc, char *argv[])
{
	char *line = NULL;
	size_t buff_size = 0;
	int result;

	FILE *fhnd = fopen(argv[1], "r");
	if (!fhnd) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	while (result > 0) {
		result = read_line(fhnd, &line, &buff_size);
		if (result < 0) {
			printf("### AN ERROR OCCURRED ###\n");
			break;
		}
		printf("%s", line);
	}

	free(line);

	return EXIT_SUCCESS;
}
/*
fscanf - test against expected read values otherwise infinite loop as eof not returned if unexpected type of data read

feof
fprintf
fputc
fputs
fetc
fgets
For a start, gets doesn't check the size of the buffer you give it, so it's easy to get a buffer overflow. For this reason never ever use gets!

// binary IO
fread
fwrite

//misco
fileno
ferror
clearerr

perror - print system error message
The routine perror() produces a message on the standard error output, describing the last error encountered during a call to a system or library function. First (if s is not NULL and *s is not a null byte ('\0')) the argument string s is printed, followed by a colon and a blank. Then the message and a new-line. 
*/
