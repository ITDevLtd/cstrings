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
#include <stdio.h>


char* e_strtok_r(char *const str, char const *const delims, char **save_ptr, char *const delim_found)
{
   char *del;
   char *start = str ? str : *save_ptr;

   if (start == NULL)
      return NULL;

   if (*start == '\0') {
      if (*delim_found != '\0') {
         *delim_found = '\0';
         *save_ptr = NULL;  
         return start;   
      }
      else
         return NULL;
   }
   del = strpbrk(start, delims);
   if(!del) {
      *delim_found = '\0';
      *save_ptr = NULL;  
      return start;
   }

   *delim_found = *del;
   *del = '\0';
   *save_ptr = del + 1;
   return start;
}


int main(int argc, char *argv[])
{
   char delim;
   char *save_ptr = NULL;
   char *tok = NULL;
   char str[] = ",File,,James,Poo,Head\nSome,one\nElse,lives,here,in,  the,jungle,";

   printf("### STRTOK\n");
   tok = strtok_r(str, ",", &save_ptr);
   while(tok)
   {
      printf("%s - ", tok);
      tok = strtok_r(NULL, ",", &save_ptr);
   }

   printf("\n-------\n");
 
   /* So the drawbacks are that we dont get to see consecutive delimiters and we don't know which
    * delimeter we choked on 
    * So I would like a function that does both of these
    */   
   printf("### ENHANCED STRTOK TRAILING DELIM\n");
   strcpy(str, ",File,,James,Poo,Head\nSome,one\nElse,lives,here,in,  the,jungle,");
   tok = e_strtok_r(str, ",\n", &save_ptr, &delim);
   while(tok)
   {
      printf("%s (%c) - ", tok, delim);
      tok = e_strtok_r(NULL, ",\n", &save_ptr, &delim);
   }

   printf("\n");

   printf("\n-------\n");
   printf("### ENHANCED STRTOK NO TRAILING DELIM\n");

   strcpy(str, ",File,,James,Poo,Head\nSome,one\nElse,lives,here,in,  the,jungle");
   save_ptr = NULL;
   tok = e_strtok_r(str, ",\n", &save_ptr, &delim);
   while(tok)
   {
      printf("%s (%c) - ", tok, delim);
      tok = e_strtok_r(NULL, ",\n", &save_ptr, &delim);
   }

   printf("\n");


   printf("\n-------\n");
   printf("### ENHANCED STRTOK EMPTY STR\n");

   strcpy(str, "");
   save_ptr = NULL;
   tok = e_strtok_r(str, ",\n", &save_ptr, &delim);
   while(tok)
   {
      printf("%s (%c) - ", tok, delim);
      tok = e_strtok_r(NULL, ",\n", &save_ptr, &delim);
   }

   printf("\n");

   printf("\n-------\n");
   printf("### ENHANCED STRTOK ONLY ONE DELIM\n");

   strcpy(str, ",");
   save_ptr = NULL;
   tok = e_strtok_r(str, ",\n", &save_ptr, &delim);
   while(tok)
   {
      printf("%s (%c) - ", tok, delim);
      tok = e_strtok_r(NULL, ",\n", &save_ptr, &delim);
   }

   printf("\n");

   printf("\n-------\n");
   printf("### ENHANCED STRTOK ONLY ONE NEWLINE DELIM\n");

   strcpy(str, "\n");
   save_ptr = NULL;
   tok = e_strtok_r(str, ",\n", &save_ptr, &delim);
   while(tok)
   {
      printf("%s (%c) - ", tok, delim);
      tok = e_strtok_r(NULL, ",\n", &save_ptr, &delim);
   }

   printf("\n");

   

   return 0;
}

/*
   So strtok, doesn't quite do what we want it to do... but it is pretty easy to roll out own
   strspn searches for first char not in group of accept delimeters and return length
   strcspn search for first char in group of reject delimeters and return length
   strpbrk searches for first occurnce of a delimeter in a string and returns a pointer
   strchr searches for first occurence of char in string
   strrchr searches for last occurrence of char in string
   memchr searches first n bytes for memory for first occurence of byte calu
   
index(3), memchr(3), rindex(3), string(3), strlen(3), strpbrk(3), strsep(3), strspn(3), strstr(3)
index(3), memchr(3), rindex(3), string(3), strlen(3), strpbrk(3), strsep(3), strspn(3), strstr(3)
index(3), memchr(3), rindex(3), strchr(3), string(3), strpbrk(3), strsep(3), strstr(3), strtok(3), wcscspn(3), wcsspn(3)
*/

