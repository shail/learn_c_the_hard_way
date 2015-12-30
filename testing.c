#include <stdio.h>
#include <stdint.h>
int main(int argc, char *argv[]) {
  printf("INTMAX_MAX: %lld\n", INTMAX_MAX);
  printf("INTMAX_MIN: %lld\n", INTMAX_MIN);
  printf("INT_FAST(N)_MAX: %lld\n", INT_FAST64_MAX); 
  return 0;
}
