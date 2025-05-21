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

  printf("Double fmadd: result = 0x%08x%08x - expected = 0x427e1e38d7be0000\n", *(result_ptr), *(result_ptr - 1));

  float f_a = 600.0f;
  float f_b = 125.85f;
  float f_c = 99.12f;
  float f_result = 0.0f;

  f_result = f_a + f_b;
  printf("Float add: result = 0x%08x - expected = 0x44357666\n", *((unsigned int *)&f_result));

  f_result = f_a - f_b;
  printf("Float sub: result = 0x%08x - expected = 0x43ed1333\n", *((unsigned int *)&f_result));
  
  f_result = f_a * f_b;
  printf("Float multiply result = 0x%08x - expected = 0x47937b00\n", *((unsigned int *)&f_result));

  asm volatile (
    "fnmadd.s %0, %1, %2, %3"
    : "=f"(f_result)
    : "f"(f_a), "f"(f_b), "f"(f_c)
  );

  // TODO should be 0xc793ac8f
  printf("Float fnmadd: result = 0x%08x - expected = 0xc793ac8e\n", *((unsigned int *)&f_result));

  int i_a = 666;
  f_result = (float) i_a;

  printf("Float IntToFP convert: result = 0x%08x - expected = 0x44268000\n", *((unsigned int *)&f_result));

  long long l_b = 922337203685477;
  result = (double) l_b;

  printf("Double IntToFP convert: result = 0x%08x%08x - expected = 0x430a36e2eb1c4328\n", *(result_ptr), *(result_ptr - 1));

  int i_result = f_a;
  printf("Float FPToInt convert: result = 0x%08x - expected = 0x00000258\n", i_result);

  long long l_result = a;
  result_ptr = (int *)&l_result;
  result_ptr += 1;
  printf("Double FPToInt convert: result = 0x%08x%08x - expected = 0x00000000000f4240\n", *(result_ptr), *(result_ptr - 1));

  l_result = -76543238144.413415f;
  printf("Float FPToInt convert: result = 0x%08x%08x - expected = 0xffffffee2daae000\n", *(result_ptr), *(result_ptr - 1));

  float f_d = 99.12f;

  printf("Float compare: result = %d - expected = 0\n", f_a == f_b);

  printf("Float compare: result = %d - expected = 1\n", f_c == f_d);

  printf("Float compare: result = %d - expected = 1\n", f_a > f_b);

  printf("Float compare: result = %d - expected = 0\n", f_c > f_d);

  printf("Float compare: result = %d - expected = 1\n", f_c >= f_d);

  printf("Double compare: result = %d - expected = 0\n", a == b);

  printf("Double compare: result = %d - expected = 1\n", c > a);

  printf("Double compare: result = %d - expected = 0\n", b < a);

  f_result = -f_a;

  printf("Float negative: result = 0x%08x - expected = 0xc4160000\n", *((unsigned int *)&f_result));

  result = -a;
  result_ptr = (int *)&result;
  result_ptr += 1;
  printf("Double negative: result = 0x%08x%08x - expected = 0xc12e848000000000\n", *(result_ptr), *(result_ptr - 1));

  asm volatile (
    "fmin.s %0, %1, %2"
    : "=f"(f_result)
    : "f"(f_a), "f"(f_b)
  );

  printf("Float min: result = 0x%08x - expected = 0x42fbb333\n", *((unsigned int *)&f_result));

  asm volatile (
    "fmax.d %0, %1, %2"
    : "=f"(result)
    : "f"(c), "f"(b)
  );

  result_ptr = (int *)&result;
  result_ptr += 1;

  printf("Double max: result = 0x%08x%08x - expected = 0x42303a435be00000\n", *(result_ptr), *(result_ptr - 1));

  f_result = result;
  printf("Double to Float convert: result = 0x%08x - expected = 0x5181d21b\n", *((unsigned int *)&f_result));

  result = f_result;
  printf("Float to Double convert: result = 0x%08x%08x - expected = 0x42303a4360000000\n", *(result_ptr), *(result_ptr - 1));

  f_result = a / b;
  printf("Float divide: result = 0x%08x - expected = 0x3f000000\n", *((unsigned int *)&f_result));

  asm volatile (
    "fsqrt.s %0, %1"
    : "=f"(f_result)
    : "f"(f_a)
  );
  printf("Float sqrt: result = 0x%08x - expected = 0x42f6e666\n", *((unsigned int *)&f_result));

  result = b / a;
  result_ptr = (int *)&result;
  result_ptr += 1;
  printf("Double divide: result = 0x%08x%08x - expected = 0x4000000000000000\n", *(result_ptr), *(result_ptr - 1));

  asm volatile (
    "fsqrt.d %0, %1"
    : "=f"(result)
    : "f"(c)
  );
  printf("Double sqrt: result = 0x%08x%08x - expected = 0x41101d07589db272\n", *(result_ptr), *(result_ptr - 1));

  return 0;
}