#include <stdio.h>

int main(int argc, char *argv[])
{
// There isn't a boolean type in C. Either 0 (false) or anything else (true)
// for-loops better than while because it is easy to forget to initialize the first
// int i, forget to initialize second loop so it will take the value from before,
// forget to increment in the loop and infinite loop
// In copying array of string literals, you aren't actually copying, just changing the pointers
    int i = argc - 1;
    while(i > 0) {
        printf("arg %d: %s\n", i, argv[i]);
        i--;
    }
    i = 1;
    // TODO: why does this seg fault when new_states doesn't have at least
    // one initial value
    char *new_states[] = {"Test"};
    while(i < argc) {
      new_states[i] = argv[i];
      printf("new_states %s\n", new_states[i]);
      i++;
    }

    char *states[] = {
        "California", "Oregon",
        "Washington", "Texas"
    };

    int num_states = 4;
    i = 0;
    while(i < num_states) {
        printf("state %d: %s\n", i, states[i]);
        i++;
    }

    return 0;
}
