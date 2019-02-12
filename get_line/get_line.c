/* Copyright (c) 2018, James Hume
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "get_line.h"

#define LINE_BUF_SIZE_BYTES 256

static bool buffer_contains_newline(const char *const buffer)
{
	return strchr(buffer, (int)'\n') != NULL;
}

int read_line(FILE *const fhnd, char **const buffer, size_t *const usr_buff_size)
{
	int error_number;
	char *result;
	char *line_buff;
	char *buff_pos;
	size_t buff_size;

	/* The initial buffer size may be different to the expansion size so
	 * for first iteration this is probably user defined, and then in latter
	 * iterations is just LINE_BUF_SIZE_BYTES */
	size_t block_size;

	if (!buffer || !usr_buff_size) {
		return -EINVAL;
	}

	if (feof(fhnd)) {
		if (*buffer) {
			**buffer = '\0';
		}
		return 0;
	}

	if (!*buffer) {
		line_buff = malloc(LINE_BUF_SIZE_BYTES);
		if (!line_buff) {
			return -ENOMEM;
		}
		line_buff[0] = '\0';

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
		result = fgets(buff_pos, (int)block_size, fhnd);
		if (!result) {
			/* NULL means error or feof with zero chars read. */
			if (ferror(fhnd)) {
				goto _error;
			}
			/* else: covered by check for feof() at start of function */
		}

		if (feof(fhnd)) {
			break;
		}

		if (!buffer_contains_newline(buff_pos)) {
			/* No newline terminator found and no eof so buffer will be full - must resize so we
			 * can continue to read in this line */
			char *tmp_buff_ptr = realloc(line_buff, buff_size + LINE_BUF_SIZE_BYTES);
			if (!tmp_buff_ptr)
				goto _error;

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
	error_number = errno;

	if (!*buffer) {
		/* We allocated the buffer, not the user, so just free it. */
		free(line_buff);
	}
	else  {
		/* The user allocated the buffer so return a pointer to the last 
		 * properly realloc()'ed buffer (not updated in the loop to save the
		 * extra write */
		*buffer = line_buff;
	}

	return error_number;
}
