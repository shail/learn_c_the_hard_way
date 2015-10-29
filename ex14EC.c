#include <stdio.h>
#include <ctype.h>
#include <string.h>

// forward_declarations: solves the problem of needing to use a function before it is defined
void print_letters(char arg[], int string_length);

void print_arguments(int argc, char *argv[])
{
    int i = 0;

    for (i = 0; i < argc; i++) {
        int string_length = strlen(argv[i]);
        print_letters(argv[i], string_length);
    }
}

void print_letters(char arg[], int string_length)
{
    int i = 0;

    for (i = 0; i < string_length; i++) {
        char ch = arg[i];
        int can_print = isalpha(ch) || isblank(ch) || isdigit(ch);        
        if (can_print) {
            printf("'%c' == %d ", ch, ch);
        }
    }

    printf("\n");
}

int main(int argc, char *argv[]) 
{
    print_arguments(argc, argv);
    return 0;
}
