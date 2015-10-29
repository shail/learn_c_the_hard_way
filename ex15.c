#include <stdio.h>

int main(int argc, char *argv[]) {
    int ages[] = {23, 43, 12, 89, 2};
    char *names[] = {
        "Alan", "Frank",
        "Mary", "John", "Lisa"
    };

    int count = sizeof(ages) / sizeof(int);
    int i = 0;

    for (i = 0; i < count; i++) {
        printf("%s has %d years alive.\n",
                names[i], ages[i]);
    }

    printf("---\n");

    // setup the pointers to the start of the arrays
    int *cur_age = ages;
    char **cur_name = names;

    // using pointers
    // *(cur_name + i) is the same as names[i]
    for (i = 0; i < count; i++) {
        printf("%s is %d years old.\n",
                *(cur_name+i), *(cur_age+i));
    }

    printf("---\n");

    // third way, pointers are arrays
    for (i = 0; i < count; i++) {
        printf("%s is %d years old again.\n",
                cur_name[i], cur_age[i]);
    }

    // 4th stupid way
    for (cur_name = names, cur_age = ages; (cur_age - ages) < count; cur_name++, cur_age++) {
        printf("%s lived %d years so far.\n",
                *cur_name, *cur_age);
    }

    return 0;
}
// When you reference something like ages[i] you are really saying I want the integer in ages that is
// at address 'i'. ages[] is a location the computer's memory where all of these integers start. ages is
// also an address. The C compiler will replace anywhere you type ages with the address of the very
// first integer in ages. ages[] is the address of the first integer in ages. ages[] is an address inside
// the ENTIRE computer, not just inside ages[] like 'i' is. A pointer stores the location of where something
// is stored in memory, and then you can do arthimetic and move it around to get the information you need. A
// pointer is simply an address pointing somewhere inside the computer's memory with a type specifier so
// you get the right size of data with it. Purpose of a pointer is to let you manually index into blocks of
// memory when an array won't do it right
//
// Practical Pointer Usage
// ask OS for chunk of memory and use a pointer to work with, includes strings and structs
// passing large blocks of memory to functions so you don't have to pass the whole thing
// taking the address of a function so you can use it as a dynamic callback
// complex scanning of chunks of memory such as converting bytes off a network socket into data structures
// or parsing files.
