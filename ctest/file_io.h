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
#ifndef __JEHTECH_FILE_IO_H__
#define __JEHTECH_FILE_IO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/*
 * @brief Reads a file line at a time
 *
 * Function will read the file identified by the @c FILE handle @c fhnd line
 * by line (or until EOF) into a buffer. The buffer can either be allocated 
 * automatically by this function if @c *buffer is @c NULL, or by the user. 
 * Whether the buffer is auto allocated or not, it is ALWAYS THE USER'S 
 * RESPONSIBILITY TO FREE THE LINE BUFFER once it is no longer needed (buffer
 * always returned via the parameter @c buffer).
 *
 * @param fhnd   The @c FILE handle of the file to be read
 * @param buffer Must never be @c NULL. If @p *buffer is @c NULL, a new buffer
 *               will be allocated automatically and returned via @p *buffer.
 *               If @p *buffer was not @c NULL, then the buffer allocated by 
 *               the user is used and the parameter @p user_buff_size must give
 *               the exact size of the buffer in bytes. The user must note that
 *               the buffer may be realloc()'ated. This means that 
 *               a STATIC BUFFER MAY *NOT* BE USED and the address of the
 *               original buffer should NOT BE CACHED anywhere else as the
 *               realloc()'ation for long lines may, but not always, change the
 *               address of the buffer and when this happens the old buffer is
 *               freed.
 *
 *               The newline character is always returned in the buffer unless
 *               EOF was reached.
 *
 * @param usr_buff_size If @p *buffer is not @c NULL then this parameter must
 *                      give the initial size of the buffer in bytes. It will
 *                      also return the size of the buffer returned via
 *                      @p buffer. Note that this may be larger than the
 *                      original buffer if the line length read from the file
 *                      required more memory.
 *
 * @return negative value on error, 0 when there are no newlines be read, 1 on
 *         success but when there are potentially more lines to be read. Thus
 *         the function can be read until 0 is returned.
 *
 * @todo buffer size can currently grow to an unrestrained size. may want to
 *       be able to specify a limit.
 */
int read_line(FILE *fhnd, char **buffer, size_t *usr_buff_size);

#ifdef __cplusplus
}
#endif

#endif /* __JEHTECH_FILE_IO_H__ */
