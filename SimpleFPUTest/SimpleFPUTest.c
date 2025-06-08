#include "data.h"
#include <stdio.h>

#define NRS 1 // 0 for IEEE754, 1 for POSIT

void check_float_result(float res, unsigned int expected, const char *operation) {
  unsigned int res_int = *(long *)&res;
  if (res_int != expected) {
    printf("Error in %s: expected 0x%x, got 0x%x\n", operation, expected, res_int);
  } else {
    printf("Success in %s: got 0x%x\n", operation, res_int);
  }
}

void check_int_result(unsigned int res, unsigned int expected, const char *operation) {
  if (res != expected) {
    printf("Error in %s: expected 0x%x, got 0x%x\n", operation, expected, res);
  } else {
    printf("Success in %s: got 0x%x\n", operation, res);
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

int main() { test_float_operations(); }