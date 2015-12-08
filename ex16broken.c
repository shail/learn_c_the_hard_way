#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// A structure in C is a collection of other data types that are stored in one block of memory but let you
// access each variable independently by name. Struct person is a compound data type, which means you can
// refer to Person in the same kinds of expressions you would other data types. You can access the individual
// parts of a struct by their names by using x->y if dealing with a pointer otherwise you can use the x.y
// (period) syntax.
//
struct Person {
    char *name;
    int age;
    int height;
    int weight;
};

// returns a pointer to a struct
struct Person *Person_create(char *name, int age, int height, int weight) {
    struct Person *who = malloc(sizeof(struct Person));
    assert(who != NULL);

    who->name = strdup(name);
    who->age = age;
    who->height = height;
    who->weight = weight;

    return who;
}

void Person_destroy(struct Person *who) {
    assert(who != NULL);

    free(who);
}

void Person_print(struct Person *who) {
    printf("Name: %s\n", who->name);
    printf("\tAge: %d\n", who->age);
    printf("\tHeight: %d\n", who->height);
    printf("\tWeight: %d\n", who->weight);
}

int main(int argc, char *argv[]) {
    // Make two people structures
    struct Person *joe = Person_create("Joe Alex", 32, 64, 140);
    struct Person *frank = Person_create("Frank Blank", 20, 72, 180);

    // Print them out and where they are in memory
    printf("Joe is at memory location %p:\n", joe);
    Person_print(joe);

    printf("Frank is at memory location %p:\n", frank);
    Person_print(frank);

    // make everyone age 20 years and print them out again
    joe->age += 20;
    joe->height -= 2;
    joe->weight += 40;
    Person_print(joe);

    frank->age += 20;
    frank->weight += 20;
    Person_print(frank);

    // destroy them both so we can clean up
    //Person_destroy(NULL); // this should abort compilation but for some reason doesnt
    Person_destroy(joe); // --leak-check=full to see where memory leak is happening
    Person_destroy(frank);

    return 0;
}
