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

int main(void) {
  printf("%c\n", lower('c'));
}
