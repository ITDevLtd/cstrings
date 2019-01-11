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
#include <string.h>
#include "estrtok.h"

char* estrtok_r(char *const str, char const *const delims, char **save_ptr, char *const delim_found)
{
   char *del;

   /* First call `str` should not be NULL. In subsequent calls `save_ptr` is used to remember
    * the position after the last delimeter found in the previous call. */
   char *start = str ? str : *save_ptr;

   if (start == NULL)
      return NULL;

   if (*start == '\0') {
      /* We hit the end of the string being searched or are searching an emptry string. */
      if (*delim_found != '\0') {
         *delim_found = '\0';
         *save_ptr = NULL;  
         return start;   
      }
      else
         return NULL;
   }
   
   /* Search string for any chars in delims */
   del = strpbrk(start, delims);
   if(!del) {
      /* No delimiter was found */
      *delim_found = '\0';
      *save_ptr = NULL;  
      return start;
   }

   /* Delimeter was found... */
   *delim_found = *del; /* Save the delimeter character */
   *del = '\0';         /* Set the delimeter to NULL */
   *save_ptr = del + 1; /* Set the saved position to one after the delimeter */
   return start;        /* Return the string - the delimeter was replaced with
                         * NULL like strtok would have done */
}
