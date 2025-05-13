#include <stdio.h>

int main() {
  double a = 1000000.0;
  double b = 2000000.0;
  double c = 69696969696.0;

  double result = 0.0;

  asm volatile (
    "fmadd.d %0, %1, %2, %3"
    : "=f"(result)
    : "f"(a), "f"(b), "f"(c)
  );

  int *result_ptr = (int *)&result;
  result_ptr += 1;

  printf("Double fmadd: result = 0x%x%x - expected = 0x427e1e38d7be0000\n", *(result_ptr), *(result_ptr - 1));

  float f_a = 600.0f;
  float f_b = 125.85f;
  float f_c = 99.12f;
  float f_result = 0.0f;

  f_result = f_a + f_b;
  printf("Float add: result = 0x%x - expected = 0x44357666\n", *((unsigned int *)&f_result));

  f_result = f_a - f_b;
  printf("Float sub: result = 0x%x - expected = 0x43ed1333\n", *((unsigned int *)&f_result));
  
  f_result = f_a * f_b;
  printf("Float multiply result = 0x%x - expected = 0x47937b00\n", *((unsigned int *)&f_result));

  asm volatile (
    "fnmadd.s %0, %1, %2, %3"
    : "=f"(f_result)
    : "f"(f_a), "f"(f_b), "f"(f_c)
  );

  // TODO should be 0xc793ac8f
  printf("Float fnmadd: result = 0x%x - expected = 0xc793ac8e\n", *((unsigned int *)&f_result));

  int i_a = 666;
  f_result = (float) i_a;

  printf("Float IntToFP convert: result = 0x%x - expected = 0x44268000\n", *((unsigned int *)&f_result));

  long long l_b = 922337203685477;
  result = (double) l_b;

  printf("Double IntToFP convert: result = 0x%x%x - expected = 0x44268000\n", *(result_ptr), *(result_ptr - 1));

  return 0;
}