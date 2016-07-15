#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"

#define LINE_BUF_SIZE_BYTES 256

int read_line(FILE *fhnd, char **buffer, size_t *usr_buff_size)
{
	char *result;
	char *line_buff;
	char *buff_pos;
	size_t buff_size;

	/* The initial buffer size may be different to the expansion size so
	 * for first iteration this is probably user defined, and then in latter
	 * iterations is just LINE_BUF_SIZE_BYTES */
	size_t block_size;

	if (!buffer || !usr_buff_size) {
		return -1;
	}

	if (feof(fhnd)) {
		return 0;
	}
	
	if (!*buffer) {
		line_buff = malloc(LINE_BUF_SIZE_BYTES);
		if (!line_buff) {
			return -1;
		}
		buff_size = LINE_BUF_SIZE_BYTES;
		block_size = LINE_BUF_SIZE_BYTES;
	}
	else {
		line_buff = *buffer;
		buff_size = *usr_buff_size;
		block_size = buff_size;
	}

	buff_pos = line_buff;

	while (1) 
	{
		result = fgets(buff_pos, block_size, fhnd);
		if (!result) {
			/* NULL means error or feof with zero chars read. */
			if (ferror(fhnd)) {
				goto _error;
			}
			else {
				/* If we read no bytes because we immediately hit EOF, make sure
				 * the buffer starts with a NULL terminating byte */
				line_buff[0] = '\0';
			}
		}

		if (feof(fhnd)) {
			break;
		}

		/* Find out if we read in a newline. Search through the string until
		 * the null byte. If a newline is found before that, then we have 
		 * completed a line, otherwise we need to enlarge our buffer */
		result = strchr(buff_pos, (int)'\n');
		if (!result) {
			/* No newline terminator found and no eof so buffer will be full */
			char *tmp_buff_ptr = realloc(line_buff, buff_size + LINE_BUF_SIZE_BYTES);
			if (!tmp_buff_ptr) {
				goto _error;
			}
			line_buff  = tmp_buff_ptr;
			buff_pos   = line_buff + buff_size - 1u;
			buff_size += LINE_BUF_SIZE_BYTES;
			block_size = LINE_BUF_SIZE_BYTES + 1u;
		}
		else {
			break;
		}
	}

	*buffer = line_buff;
	*usr_buff_size = buff_size;
	return 1;

_error:
	if (!*buffer)
		/* We allocated the buffer, not the user, so just free it. */
		free(line_buff);
	else 
		/* The user allocated the buffer so return a pointer to the last 
		 * properly realloc()'ed buffer (not updated in the loop to save the
		 * extra write */
		*buffer = line_buff;

	return -1;
}



