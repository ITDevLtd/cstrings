#ifndef __JEHTECH_FILE_IO_H__
#define __JEHTECH_FILE_IO_H__

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
 */
int read_line(FILE *fhnd, char **buffer, size_t *usr_buff_size);


#endif /* __JEHTECH_FILE_IO_H__ */
