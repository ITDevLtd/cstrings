#include <string.h>
#include <stdio.h>
#include "estrtok.h"

int main(int argc, char *argv[])
{
   int counter;
   char delim;
   char *save_ptr = NULL;
   char *tok = NULL;
   char str[] = ",File,,James,Poo,Head\nSome,one\nElse,lives,here,in,  the,jungle,";

   printf("### STRTOK\n");
   counter = 0;
   tok = strtok_r(str, ",", &save_ptr);
   while(tok)
   {
      printf("Token %d: \"%s\"\n", counter, tok);
      ++counter;
      tok = strtok_r(NULL, ",", &save_ptr);
   }

   printf("\n-------\n");
 
   /* So the drawbacks are that we dont get to see consecutive delimiters and we don't know which
    * delimeter we choked on 
    * So I would like a function that does both of these
    */   
   printf("### ENHANCED STRTOK TRAILING DELIM\n");
   strcpy(str, ",File,,James,Poo,Head\nSome,one\nElse,lives,here,in,  the,jungle,");
   counter = 0;
   tok = estrtok_r(str, ",\n", &save_ptr, &delim);
   while(tok)
   {
      printf("Token %d: \"%s\". Delim: \"%c\"\n", counter, tok, delim);
      ++counter;
      tok = estrtok_r(NULL, ",\n", &save_ptr, &delim);
   }

   printf("\n");

   printf("\n-------\n");
   printf("### ENHANCED STRTOK NO TRAILING DELIM\n");

   strcpy(str, ",File,,James,Poo,Head\nSome,one\nElse,lives,here,in,  the,jungle");
   save_ptr = NULL;
   counter = 0;
   tok = estrtok_r(str, ",\n", &save_ptr, &delim);
   while(tok)
   {
      printf("Token %d: \"%s\". Delim: \"%c\"\n", counter, tok, delim);
      ++counter;
      tok = estrtok_r(NULL, ",\n", &save_ptr, &delim);
   }

   printf("\n");


   printf("\n-------\n");
   printf("### ENHANCED STRTOK EMPTY STR\n");

   strcpy(str, "");
   save_ptr = NULL;
   counter = 0;
   tok = estrtok_r(str, ",\n", &save_ptr, &delim);
   while(tok)
   {
      printf("Token %d: \"%s\". Delim: \"%c\"\n", counter, tok, delim);
      ++counter;
      tok = estrtok_r(NULL, ",\n", &save_ptr, &delim);
   }

   printf("\n");

   printf("\n-------\n");
   printf("### ENHANCED STRTOK ONLY ONE DELIM\n");

   strcpy(str, ",");
   save_ptr = NULL;
   counter = 0;
   tok = estrtok_r(str, ",\n", &save_ptr, &delim);
   while(tok)
   {
      printf("Token %d: \"%s\". Delim: \"%c\"\n", counter, tok, delim);
      ++counter;
      tok = estrtok_r(NULL, ",\n", &save_ptr, &delim);
   }

   printf("\n");

   printf("\n-------\n");
   printf("### ENHANCED STRTOK ONLY ONE NEWLINE DELIM\n");

   strcpy(str, "\n");
   save_ptr = NULL;
   counter = 0;
   tok = estrtok_r(str, ",\n", &save_ptr, &delim);
   while(tok)
   {
      printf("Token %d: \"%s\". Delim: \"%c\"\n", counter, tok, delim);
      ++counter;
      tok = estrtok_r(NULL, ",\n", &save_ptr, &delim);
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
