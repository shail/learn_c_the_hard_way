// By creating a variable argument function, you can create nicer interfaces to your library. They are handy
// for certain types of "builder" functions, formatting functions, and anything that takes variable arguments.
// Difference between calloc() and malloc() is calloc() zero initializes the buffer, performance impact
// because it might take a while to fill it up with zeros

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "dbg.h"

#define MAX_DATA 100

// Pointer to a pointer because if you enter multiple words, it will be pointers to multiple strings
// Use ** when you want to preserve (OR retain change in) the memory-allocation or assignment outside of a
// function call. One reason is you want to change the value of the pointer passed to a function as the
// function argument.
//
// Pointers to pointers also come in handy as "handles" to memory between functions to re-locatable memory.
// This means that the function can change the memory that is being pointed to by the pointer inside the
// handle variable, and every function or object that is using the handle will properly point to the newly
// relocated (or allocated) memory.
int read_string(char **out_string, int max_buffer) {
  *out_string = calloc(1, max_buffer + 1);
  check_mem(*out_string);

  char *result = fgets(*out_string, max_buffer, stdin);
  check(result != NULL, "Input error.");

  return 0;

error:
  if (*out_string) free(*out_string);
  *out_string = NULL;
  return -1;
}

int read_int(int *out_int) {
  char *input = NULL;
  int rc = read_string(&input, MAX_DATA);
  check(rc == 0, "Failed to read number.");

  *out_int = atoi(input);
  free(input);
  return 0;

error:
  if (input) free(input);
  return -1;
}

// Set the last parameter of the function to ..., and this indicates to C that this function will take any
// number of arguments after the fmt argument. Can't put any more arguments after this
int read_scan(const char *fmt, ...) {
  int i = 0;
  int rc = 0;
  int *out_int = NULL;
  char *out_char = NULL;
  char **out_string = NULL;
  int max_buffer = 0;

  // This handles setting up the gear to handle variable arguments
  va_list argp;
  va_start(argp, fmt);

  // Loop through the format string fmt and process formats
  for (i = 0; fmt[i] != '\0'; i++) {
    if (fmt[i] == '%') {
      i++;
      // To get a variable from the va_list argp, use the macro va_arg(argp, TYPE) where TYPE is the exact
      // type of what I will assign this function parameter to. The downside to this design is you're flying
      // blind, so if you don't have enough parameters then oh well, you'll most likely crash.
      // Interesting difference from scanf is people want read_scan to create the strings it reads when it
      // hits a 's' sequence. When you give this sequence, the function takes two parameters off the va_list
      // argp stack, max function size to read, and the output character string pointer. This makes read_scan
      // more consistent than scanf since you always give an address-of & on variables to have them set
      // appropriately.
      switch(fmt[i]) {
        case '\0':
          sentinel("Invalid format, you ended with %%.");
          break;
        case 'd':
          out_int = va_arg(argp, int *);
          rc = read_int(out_int);
          check(rc == 0, "Failed to read int.");
          break;
        case 'c':
          out_char = va_arg(argp, char *);
          *out_char = fgetc(stdin);
          break;
        case 's':
          max_buffer = va_arg(argp, int);
          out_string = va_arg(argp, char **);
          rc = read_string(out_string, max_buffer);
          check(rc == 0, "Failed to read string.");
          break;
        default:
          sentinel("Invalid format.");
      }
    } else {
      fgetc(stdin);
    }

    check(!feof(stdin) && !ferror(stdin), "Input error.");
  }

  va_end(argp);
  return 0;

error:
  va_end(argp);
  return -1;
}

int main(int argc, char *argv[]) {
  char *first_name = NULL;
  char initial = ' ';
  char *last_name = NULL;
  int age = 0;

  printf("What's your first name? ");
  int rc = read_scan("%s", MAX_DATA, &first_name);
  check(rc == 0, "Failed first name.");

  printf("What's your initial? ");
  rc = read_scan("%c\n", &initial);
  check(rc == 0, "Failed initial");

  printf("What's your last name? ");
  rc = read_scan("%s", MAX_DATA, &last_name);
  check(rc == 0, "Failed last name.");

  printf("How old are you? ");
  rc = read_scan("%d", &age);

  printf("---- RESULTS ----\n");
  printf("First Name: %s", first_name);
  printf("Initial: '%c' \n", initial);
  printf("Last Name: %s", last_name);
  printf("Age: %d\n", age);

  free(first_name);
  free(last_name);
  return 0;
error:
  return -1;
}
