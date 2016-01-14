#include <stdio.h>
#include "dbg.h"

#define MAX_DATA 100

typedef enum EyeColor {
  BLUE_EYES, GREEN_EYES, BROWN_EYES,
  BLACK_EYES, OTHER_EYES
} EyeColor;

const char *EYE_COLOR_NAMES[] = {
  "Blue", "Green", "Brown", "Black", "Other"
};

typedef struct Person {
  int age;
  char first_name[MAX_DATA];
  char last_name[MAX_DATA];
  EyeColor eyes;
  float income;
} Person;

int main(int argc, char *argv[]) {
  Person you = {.age = 0};
  int i = 0;
  char *in = NULL;

  // Get first name using the fgets function, reads a string from the input and makes sure it doesn't overflow
  // the given buffer
  printf("What's you First Name? ");
  in = fgets(you.first_name, MAX_DATA-1, stdin);
  check(in != NULL, "Failed to read first name.");

  printf("What's your Last Name? ");
  in = fgets(you.last_name, MAX_DATA-1, stdin);
  check(in != NULL, "Failed to read last name.");
  
  // Uses fscanf to read an integer from stdin and put it into you.age. You can see that the same format
  // string is used as printf to print an integer. fscanf requires a pointer, so need to pass the address of
  // you.age
  printf("How old are you? ");
  int rc = fscanf(stdin, "%d", &you.age);
  check(rc > 0, "You have to enter a number.");

  printf("What color are your eyes:\n");
  for (i = 0; i <= OTHER_EYES; i++) {
    printf("%d) %s\n", i+1, EYE_COLOR_NAMES[i]);
  }
  printf("> ");

  // Using fscanf again, get a number for the you.eyes, but make sure the input is valid. Important because
  // someone can enter a value outside the EYE_COLOR_NAMES array and cause a segfault
  int eyes = -1;
  rc = fscanf(stdin, "%d", &eyes);
  check(rc > 0, "You have to enter a number");

  you.eyes = eyes - 1;
  check(you.eyes <= OTHER_EYES && you.eyes >= 0, "That's not an option");

  printf("How much do you make an hour? ");
  rc = fscanf(stdin, "%f", &you.income);
  check(rc > 0, "Enter a floating point number.");

  printf("---- Results ----\n");

  printf("First Name: %s", you.first_name);
  printf("Last Name: %s", you.last_name);
  printf("Age: %d\n", you.age);
  printf("Eyes: %s\n", EYE_COLOR_NAMES[you.eyes]);
  printf("Income: %f\n", you.income);

  return 0;
error:
  return -1;
}
