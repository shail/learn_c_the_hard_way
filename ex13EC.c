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
            switch(lower(letter)) {
                case 'a':
                    printf("%d: 'a'\n", i);
                    break;

                case 'e':
                    printf("%d: 'e'\n", i);
                    break;

                case 'i':
                    printf("%d: 'i'\n", i);
                    break;

                case 'o':
                    printf("%d: 'o'\n", i);
                    break;

                case 'u':
                    printf("%d: 'u'\n", i);
                    break;

                case 'y':
                    if(i > 2) {
                        // it's only sometimes Y
                        // if you move the break inside this if statement, then when false it will 
                        // fall through
                        printf("%d: 'y'\n", i);
                    }
                    break;

                default:
                    printf("%d: %c is not a vowel\n", i, letter);
            }
        }
    }
    return 0;
}
