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
#ifndef ESTRTOK_H
#define ESTRTOK_H

/**
 * @brief Enhanced strtok_r() - parse accepting more than 1 delimeter
 * 
 * The function estrtok_r() is an enhanced version of the re-entrant strtok()
 * tokenizer. Instead of finding only one delimeter character it can "break"
 * on a set of delimeter characters. Like strtok(), when the token is returned
 * the delimeter after the token has been replaced with the null character.
 * Unlike strtok() however, the delimeter that preceeded the token is also
 * returned. This allows it to detect multiple sequential tokens from a set.
 * This is useful for CSV file parsing, for example, where a line must be
 * delimeted on both a comma and a new line and consequitive commas indicate
 * empty tokens that are significant and must not be ignored.
 *
 * Like strtok() this function must be called with str pointing to string
 * to be searched on the first call and then NULL on all subsequent calls.
 *
 * @param str         The string to be tokenized. Must not be NULL on the first
 *                    call and NULL in all subsequent calls.
 * @param delims      A set of delimeter characters.
 * @param save_ptr    Used internally by the function to record the start of
 *                    the string after the last returned token so that search
 *                    can continue.
 * @param delim_found Returns the delimeter that preceeded the token returned
 *
 * @return A pointer to the next token found. This may be the empty string if
 *         consequtive delimeters are found. It will be NULL after the last
 *         token has been found.
 */
char* estrtok_r(char *const str, char const *const delims, char **save_ptr, char *const delim_found);

#endif /* ESTRTOK_H */