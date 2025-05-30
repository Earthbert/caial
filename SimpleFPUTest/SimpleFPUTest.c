#include <stdio.h>

long long const expected_results[][2] = {
    {0x427e1e38d7be0000, 0x7ff1c3c71af7c000}, // Double fmadd
    {0x44357666, 0x72d5d99a},                 // Float add
    {0x43ed1333, 0x71b44ccd},                 // Float sub
    {0x47937b00, 0x7c137b00},                 // Float multiply
    {0xc793ac8e},                             // Float fnmadd ?
    {0x44268000, 0x729a0000},                 // Float IntToFP convert
    {0x430a36e2eb1c4328, 0x7ffcd1b71758e219}, // Double IntToFP convert
    {0xc4160000, 0xf2580000},                 // Float negative
    {0xc12e848000000000, 0xfdf4240000000000}, // Double negative
    {0x42fbb333, 0x6bdd9998},                 // Float min
    {0x42303a435be00000, 0x7fe00e90d6f80000}, // Double max
    {0x5181d21b, 0x7fe00e91},                 // Double to Float convert
    {0x42303a4360000000, 0x7fe00e9100000000}, // Float to Double convert
    {0x3f000000, 0x38000000},                 // Float divide
    {0x42f6e666, 0x621fac66},                 // Float sqrt
    {0x4000000000000000, 0x4800000000000000}, // Double divide
    {0x41101d07589db272, 0x7d00e83ac4ed9391}  // Double sqrt
};

#define NRS 1

int main() {
  double a = 1000000.0;
  double b = 2000000.0;
  double c = 69696969696.0;

  double result = 0.0;

  asm volatile("fmadd.d %0, %1, %2, %3" : "=f"(result) : "f"(a), "f"(b), "f"(c));

  int *result_ptr = (int *)&result;
  result_ptr += 1;

  printf("Double fmadd: result = 0x%08x%08x - expected = 0x%08x%08x\n", *(result_ptr), *(result_ptr - 1), (unsigned int)(expected_results[0][NRS] >> 32),
         (unsigned int)expected_results[0][NRS]);

  float f_a = 600.0f;
  float f_b = 125.85f;
  float f_c = 99.12f;
  float f_result = 0.0f;

  f_result = f_a + f_b;
  printf("Float add: result = 0x%08x - expected = 0x%08x\n", *((unsigned int *)&f_result), (unsigned int)expected_results[1][NRS]);

  f_result = f_a - f_b;
  printf("Float sub: result = 0x%08x - expected = 0x%08x\n", *((unsigned int *)&f_result), (unsigned int)expected_results[2][NRS]);

  f_result = f_a * f_b;
  printf("Float multiply result = 0x%08x - expected = 0x%08x\n", *((unsigned int *)&f_result), (unsigned int)expected_results[3][NRS]);

  asm volatile("fnmadd.s %0, %1, %2, %3" : "=f"(f_result) : "f"(f_a), "f"(f_b), "f"(f_c));

  printf("Float fnmadd: result = 0x%08x - expected = 0x%08x\n", *((unsigned int *)&f_result), (unsigned int)expected_results[4][NRS]);

  int i_a = 666;
  f_result = (float)i_a;

  printf("Float IntToFP convert: result = 0x%08x - expected = 0x%08x\n", *((unsigned int *)&f_result), (unsigned int)expected_results[5][NRS]);

  long long l_b = 922337203685477;
  result = (double)l_b;

  printf("Double IntToFP convert: result = 0x%08x%08x - expected = "
         "0x%08x%08x\n",
         *(result_ptr), *(result_ptr - 1), (unsigned int)(expected_results[6][NRS] >> 32), (unsigned int)expected_results[6][NRS]);

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

  printf("Float negative: result = 0x%08x - expected = 0x%08x\n", *((unsigned int *)&f_result), (unsigned int)expected_results[7][NRS]);

  result = -a;
  result_ptr = (int *)&result;
  result_ptr += 1;
  printf("Double negative: result = 0x%08x%08x - expected = 0x%08x%08x\n", *(result_ptr), *(result_ptr - 1), (unsigned int)(expected_results[8][NRS] >> 32),
         (unsigned int)expected_results[8][NRS]);

  asm volatile("fmin.s %0, %1, %2" : "=f"(f_result) : "f"(f_a), "f"(f_b));

  printf("Float min: result = 0x%08x - expected = 0x%08x\n", *((unsigned int *)&f_result), (unsigned int)expected_results[9][NRS]);

  asm volatile("fmax.d %0, %1, %2" : "=f"(result) : "f"(c), "f"(b));

  result_ptr = (int *)&result;
  result_ptr += 1;

  printf("Double max: result = 0x%08x%08x - expected = 0x%08x%08x\n", *(result_ptr), *(result_ptr - 1), (unsigned int)(expected_results[10][NRS] >> 32),
         (unsigned int)expected_results[10][NRS]);

  f_result = result;
  printf("Double to Float convert: result = 0x%08x - expected = 0x%08x\n", *((unsigned int *)&f_result), (unsigned int)expected_results[11][NRS]);

  result = f_result;
  printf("Float to Double convert: result = 0x%08x%08x - expected = "
         "0x%08x%08x\n",
         *(result_ptr), *(result_ptr - 1), (unsigned int)(expected_results[12][NRS] >> 32), (unsigned int)expected_results[12][NRS]);

  f_result = a / b;
  printf("Float divide: result = 0x%08x - expected = 0x%08x\n", *((unsigned int *)&f_result), (unsigned int)expected_results[13][NRS]);

  asm volatile("fsqrt.s %0, %1" : "=f"(f_result) : "f"(f_a));
  printf("Float sqrt: result = 0x%08x - expected = 0x%08x\n", *((unsigned int *)&f_result), (unsigned int)expected_results[14][NRS]);

  result = b / a;
  result_ptr = (int *)&result;
  result_ptr += 1;
  printf("Double divide: result = 0x%08x%08x - expected = 0x%08x%08x\n", *(result_ptr), *(result_ptr - 1), (unsigned int)(expected_results[15][NRS] >> 32),
         (unsigned int)expected_results[15][NRS]);

  asm volatile("fsqrt.d %0, %1" : "=f"(result) : "f"(c));
  printf("Double sqrt: result = 0x%08x%08x - expected = 0x%08x%08x\n", *(result_ptr), *(result_ptr - 1), (unsigned int)(expected_results[16][NRS] >> 32),
         (unsigned int)expected_results[16][NRS]);

  return 0;
}