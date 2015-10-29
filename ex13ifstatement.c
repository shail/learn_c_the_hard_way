#include <stdio.h>

char lower(char a)
{
  if (a >= 97 && a <= 122) {
    return a;
  } else if (a >= 65 && a <= 90) {
    return  a + 32;
  } else {
    return a;
  }
}

int main(int argc, char *argv[])
{
    int i = 0;
    int j = 1;
    char letter;
    for(j = 1; j < argc; j++) { 
        for(i = 0; letter = argv[j][i], argv[j][i] != '\0'; i++) {
          char lower_letter = lower(letter);
          if (lower_letter == 'a') {
              printf("%d: 'a'\n", i);
          } else if (lower_letter == 'e') {
              printf("%d: 'e'\n", i);
          } else if (lower_letter == 'i') {
              printf("%d: 'i'\n", i);
          } else if (lower_letter == 'o') {
              printf("%d: 'o'\n", i);
          } else if (lower_letter == 'u') {
              printf("%d: 'u'\n", i);
          } else if (lower_letter == 'y') {
            if (i > 2) {
                printf("%d: 'y'\n", i);
            }
          } else {
                printf("%d: %c is not a vowel\n", i, letter);
          }
        }
    }
    return 0;
}
