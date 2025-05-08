#include <stdio.h>

int main() {
  float a = 1.0f;
  float b = 2.0f;
  float c = 3.0f;

  float result = 0.0f;

  asm volatile (
    "fmadd.s %0, %1, %2, %3"
    : "=f"(result)
    : "f"(a), "f"(b), "f"(c)
  );

  printf("Hex Result: %x\n", *(unsigned int *)&result);

  return 0;
}