#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    int age;
    int height;
    int weight;
} Person;

void Person_print(Person who) {
    printf("Name: %s\n", who.name);
    printf("\tAge: %d\n", who.age);
    printf("\tHeight: %d\n", who.height);
    printf("\tWeight: %d\n", who.weight);
}

int main(int argc, char *argv[]) {
    // Make two people structures
    Person joe;
    joe.name = "Joe Alex";
    joe.age = 32;
    joe.height = 64;
    joe.weight = 140;

    Person frank;
    frank.name = "Frank Blank";
    frank.age = 20;
    frank.height = 72;
    frank.weight = 180;

    Person_print(joe);

    Person_print(frank);

    // make everyone age 20 years and print them out again
    joe.age += 20;
    joe.height -= 2;
    joe.weight += 40;
    Person_print(joe);

    frank.age += 20;
    frank.weight += 20;
    Person_print(frank);

    return 0;
}
