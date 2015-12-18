#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void die(const char *message) {
  if (errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

// a typedef creates a fake type, in this case for a function pointer
typedef int (*compare_cb)(int a, int b);
typedef int *(*sort_cb)(int *a, int b, compare_cb cmp);

// Bubble sort using compare_cb to do the sorting
int *bubble_sort(int *numbers, int count, compare_cb cmp) {
  // Creating variables on the stack and allocating space on the heap.
  int temp = 0;
  int i = 0;
  int j = 0;
  int *target = malloc(count * sizeof(int));

  if (!target) die("Memory error.");

  memcpy(target, numbers, count * sizeof(int));

  // cmp is just a pointer to a function, this lets the caller pass in anything they want as long as it
  // matches the signature of "compare_cb"
  for (i = 0; i < count; i++) {
    for (j = 0; j < count - 1; j++) {
      if (cmp(target[j], target[j+1]) > 0) {
        temp = target[j+1];
        target[j+1] = target[j];
        target[j] = temp;
      }
    }
  }

  return target;
}

int sorted_order(int a, int b) {
  return a - b;
}

int reverse_order(int a, int b) {
  return b - a;
}

int strange_order(int a, int b) {
  if (a == 0 || b == 0) {
    return 0;
  } else {
    return a % b;
  }
}

// Used to test that we are sorting things correctly
void test_sorting(int *numbers, int count, compare_cb cmp, sort_cb sort) {
  int i = 0;
  int *sorted = sort(numbers, count, cmp);

  if (!sorted) die("Failed to sort as requested");

  for (i = 0; i < count; i++) {
    printf("%d ", sorted[i]);
  }
  printf("\n");

  free(sorted);
}

int main(int argc, char *argv[]) {
  if (argc < 2) die("Usage: ex18 4 3 1 5");

  int count = argc - 1;
  int i = 0;
  char **inputs = argv + 1;

  int *numbers = malloc(count * sizeof(int));
  if (!numbers) die("Memory error.");

  for (i = 0; i < count; i++) {
    numbers[i] = atoi(inputs[i]);
  }


  test_sorting(numbers, count, sorted_order, bubble_sort);
  test_sorting(numbers, count, reverse_order, bubble_sort);
  test_sorting(numbers, count, strange_order, bubble_sort);

  free(numbers);
  return 0;
}

/* Functions in C are just pointers to a spot in the program where some code exists. You can point a pointer
 * at a function as well. Main use for this is to pass "callbacks" to other functions, or to simulate classes
 * and objects.
 *
 * Format of a function pointer:
 * int (*POINTER_NAME)(int a, int b)
 *  1.) Write a normal func declaration int callme(int a, int b);
 *  2.) Wrap function name with pointer syntax: int (*callme)(int a, int b)
 *  3.) Change the name to the pointer name: int (*compare_cb)(int a, int b)
 *
 * Key thing to remember is when you're done with this, the variable name for the pointer is called
 * compare_cb and you use it just like it is a function.
 *
 * int (*tester)(int a, int b) = sorted_order;
 * printf("TEST: %d is same as %d\n", tester(2,3), sorted_order(2, 3))
 *
 * This works even if the function pointer returns a pointer to something.
 *
 * It is hard to give functions as parameters to another function, this is where typedef is used. typedef is a
 * C keyword for making new names for other more complex types.
 */
