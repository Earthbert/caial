#include "data.h"
#include <stdio.h>

#define NRS 0 // 0 for IEEE754, 1 for POSIT

void check_float_result(float res, unsigned int expected, const char *operation) {
  unsigned int res_int = *(long *)&res;
  if (res_int != expected) {
    printf("Error in %s: expected 0x%x, got 0x%x\n", operation, expected, res_int);
  } else {
    printf("Success in %s: got 0x%x\n", operation, res_int);
  }
}

void check_double_result(double res, unsigned long long expected, const char *operation) {
  unsigned long long res_int = *(unsigned long long *)&res;
  if (res_int != expected) {
    printf("Error in %s: expected 0x%llx, got 0x%llx\n", operation, expected, res_int);
  } else {
    printf("Success in %s: got 0x%llx\n", operation, res_int);
  }
}

void check_int_result(unsigned int res, unsigned int expected, const char *operation) {
  if (res != expected) {
    printf("Error in %s: expected 0x%x, got 0x%x\n", operation, expected, res);
  } else {
    printf("Success in %s: got 0x%x\n", operation, res);
  }
}

void check_long_result(unsigned long long res, unsigned long long expected, const char *operation) {
  if (res != expected) {
    printf("Error in %s: expected 0x%llx, got 0x%llx\n", operation, expected, res);
  } else {
    printf("Success in %s: got 0x%llx\n", operation, res);
  }
}

void test_float_operations() {
  printf("Testing float operations...\n");

  for (int i = 0; i < sizeof(float_inputs) / sizeof(float_inputs[0]); i++) {
    float a = float_inputs[i][0];
    float b = float_inputs[i][1];
    float c = float_inputs[i][2];

    float f_res;
    asm volatile("fmadd.s %0, %1, %2, %3" : "=f"(f_res) : "f"(a), "f"(b), "f"(c));
    check_float_result(f_res, expected_float_results[i][0][NRS], "fmadd");
    asm volatile("fmsub.s %0, %1, %2, %3" : "=f"(f_res) : "f"(a), "f"(b), "f"(c));
    check_float_result(f_res, expected_float_results[i][1][NRS], "fmsub");
    asm volatile("fnmadd.s %0, %1, %2, %3" : "=f"(f_res) : "f"(a), "f"(b), "f"(c));
    check_float_result(f_res, expected_float_results[i][2][NRS], "fnmadd");
    asm volatile("fnmsub.s %0, %1, %2, %3" : "=f"(f_res) : "f"(a), "f"(b), "f"(c));
    check_float_result(f_res, expected_float_results[i][3][NRS], "fnmsub");
    asm volatile("fadd.s %0, %1, %2" : "=f"(f_res) : "f"(a), "f"(b));
    check_float_result(f_res, expected_float_results[i][4][NRS], "fadd");
    asm volatile("fsub.s %0, %1, %2" : "=f"(f_res) : "f"(a), "f"(b));
    check_float_result(f_res, expected_float_results[i][5][NRS], "fsub");
    asm volatile("fmul.s %0, %1, %2" : "=f"(f_res) : "f"(a), "f"(b));
    check_float_result(f_res, expected_float_results[i][6][NRS], "fmul");

    unsigned int i_res;
    asm volatile("feq.s %0, %1, %2" : "=r"(i_res) : "f"(a), "f"(b));
    check_int_result(i_res, expected_float_results[i][7][NRS], "feq");
    asm volatile("flt.s %0, %1, %2" : "=r"(i_res) : "f"(a), "f"(b));
    check_int_result(i_res, expected_float_results[i][8][NRS], "flt");
    asm volatile("fle.s %0, %1, %2" : "=r"(i_res) : "f"(a), "f"(b));
    check_int_result(i_res, expected_float_results[i][9][NRS], "fle");
    asm volatile("fabs.s %0, %1" : "=f"(f_res) : "f"(a));
    check_float_result(f_res, expected_float_results[i][10][NRS], "fabs");
    asm volatile("fneg.s %0, %1" : "=f"(f_res) : "f"(a));
    check_float_result(f_res, expected_float_results[i][11][NRS], "fneg");
    asm volatile("fmax.s %0, %1, %2" : "=f"(f_res) : "f"(a), "f"(b));
    check_float_result(f_res, expected_float_results[i][12][NRS], "fmax");
    asm volatile("fmin.s %0, %1, %2" : "=f"(f_res) : "f"(a), "f"(b));
    check_float_result(f_res, expected_float_results[i][13][NRS], "fmin");
    asm volatile("fdiv.s %0, %1, %2" : "=f"(f_res) : "f"(a), "f"(b));
    check_float_result(f_res, expected_float_results[i][14][NRS], "fdiv");
    asm volatile("fsqrt.s %0, %1" : "=f"(f_res) : "f"(a));
    check_float_result(f_res, expected_float_results[i][15][NRS], "fsqrt");
  }
}

void test_double_operations() {
  printf("Testing double operations...\n");

  for (int i = 0; i < sizeof(double_inputs) / sizeof(double_inputs[0]); i++) {
    double a = double_inputs[i][0];
    double b = double_inputs[i][1];
    double c = double_inputs[i][2];

    double d_res;
    asm volatile("fmadd.d %0, %1, %2, %3" : "=f"(d_res) : "f"(a), "f"(b), "f"(c));
    check_double_result(d_res, expected_double_results[i][0][NRS], "fmadd");
    asm volatile("fmsub.d %0, %1, %2, %3" : "=f"(d_res) : "f"(a), "f"(b), "f"(c));
    check_double_result(d_res, expected_double_results[i][1][NRS], "fmsub");
    asm volatile("fnmadd.d %0, %1, %2, %3" : "=f"(d_res) : "f"(a), "f"(b), "f"(c));
    check_double_result(d_res, expected_double_results[i][2][NRS], "fnmadd");
    asm volatile("fnmsub.d %0, %1, %2, %3" : "=f"(d_res) : "f"(a), "f"(b), "f"(c));
    check_double_result(d_res, expected_double_results[i][3][NRS], "fnmsub");
    asm volatile("fadd.d %0, %1, %2" : "=f"(d_res) : "f"(a), "f"(b));
    check_double_result(d_res, expected_double_results[i][4][NRS], "fadd");
    asm volatile("fsub.d %0, %1, %2" : "=f"(d_res) : "f"(a), "f"(b));
    check_double_result(d_res, expected_double_results[i][5][NRS], "fsub");
    asm volatile("fmul.d %0, %1, %2" : "=f"(d_res) : "f"(a), "f"(b));
    check_double_result(d_res, expected_double_results[i][6][NRS], "fmul");

    unsigned int i_res;
    asm volatile("feq.d %0, %1, %2" : "=r"(i_res) : "f"(a), "f"(b));
    check_long_result(i_res, expected_double_results[i][7][NRS], "feq");
    asm volatile("flt.d %0, %1, %2" : "=r"(i_res) : "f"(a), "f"(b));
    check_long_result(i_res, expected_double_results[i][8][NRS], "flt");
    asm volatile("fle.d %0, %1, %2" : "=r"(i_res) : "f"(a), "f"(b));
    check_long_result(i_res, expected_double_results[i][9][NRS], "fle");
    asm volatile("fabs.d %0, %1" : "=f"(d_res) : "f"(a));
    check_double_result(d_res, expected_double_results[i][10][NRS], "fabs");
    asm volatile("fneg.d %0, %1" : "=f"(d_res) : "f"(a));
    check_double_result(d_res, expected_double_results[i][11][NRS], "fneg");
    asm volatile("fmax.d %0, %1, %2" : "=f"(d_res) : "f"(a), "f"(b));
    check_double_result(d_res, expected_double_results[i][12][NRS], "fmax");
    asm volatile("fmin.d %0, %1, %2" : "=f"(d_res) : "f"(a), "f"(b));
    check_double_result(d_res, expected_double_results[i][13][NRS], "fmin");
    asm volatile("fdiv.d %0, %1, %2" : "=f"(d_res) : "f"(a), "f"(b));
    check_double_result(d_res, expected_double_results[i][14][NRS], "fdiv");
    asm volatile("fsqrt.d %0, %1" : "=f"(d_res) : "f"(a));
    check_double_result(d_res, expected_double_results[i][15][NRS], "fsqrt");
  }
}

int main() {
  // test_float_operations();
  test_double_operations();
}