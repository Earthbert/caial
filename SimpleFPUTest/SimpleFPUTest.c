#include "data.h"
#include <stdint.h>
#include <stdio.h>

#define NRS 1 // 0 for IEEE754, 1 for POSIT

int return_value = 0;

void check_float_result(float res, u_int32_t expected, const char *operation) {
  u_int32_t res_int = *(long *)&res;
  if (res_int != expected) {
    printf("Error in %s: expected 0x%x, got 0x%x\n", operation, expected, res_int);
    return_value = 1;
  } else {
    printf("Success in %s: got 0x%x\n", operation, res_int);
  }
}

void check_double_result(double res, u_int64_t expected, const char *operation) {
  u_int64_t res_int = *(u_int64_t *)&res;
  if (res_int != expected) {
    printf("Error in %s: expected 0x%lx, got 0x%lx\n", operation, expected, res_int);
    return_value = 1;
  } else {
    printf("Success in %s: got 0x%lx\n", operation, res_int);
  }
}

void check_int_result(u_int32_t res, u_int32_t expected, const char *operation) {
  if (res != expected) {
    printf("Error in %s: expected 0x%x, got 0x%x\n", operation, expected, res);
    return_value = 1;
  } else {
    printf("Success in %s: got 0x%x\n", operation, res);
  }
}

void check_long_result(u_int64_t res, u_int64_t expected, const char *operation) {
  if (res != expected) {
    printf("Error in %s: expected 0x%lx, got 0x%lx\n", operation, expected, res);
    return_value = 1;
  } else {
    printf("Success in %s: got 0x%lx\n", operation, res);
  }
}

void test_float_operations() {
  printf("Testing float operations...\n");

  for (int32_t i = 0; i < sizeof(float_inputs) / sizeof(float_inputs[0]); i++) {
    float a = float_inputs[i][0];
    float b = float_inputs[i][1];
    float c = float_inputs[i][2];

    float f_res;
    asm volatile("fmadd.s %0, %1, %2, %3" : "=f"(f_res) : "f"(a), "f"(b), "f"(c));
    check_float_result(f_res, expected_float_op_results[i][0][NRS], "fmadd");
    asm volatile("fmsub.s %0, %1, %2, %3" : "=f"(f_res) : "f"(a), "f"(b), "f"(c));
    check_float_result(f_res, expected_float_op_results[i][1][NRS], "fmsub");
    asm volatile("fnmadd.s %0, %1, %2, %3" : "=f"(f_res) : "f"(a), "f"(b), "f"(c));
    check_float_result(f_res, expected_float_op_results[i][2][NRS], "fnmadd");
    asm volatile("fnmsub.s %0, %1, %2, %3" : "=f"(f_res) : "f"(a), "f"(b), "f"(c));
    check_float_result(f_res, expected_float_op_results[i][3][NRS], "fnmsub");
    asm volatile("fadd.s %0, %1, %2" : "=f"(f_res) : "f"(a), "f"(b));
    check_float_result(f_res, expected_float_op_results[i][4][NRS], "fadd");
    asm volatile("fsub.s %0, %1, %2" : "=f"(f_res) : "f"(a), "f"(b));
    check_float_result(f_res, expected_float_op_results[i][5][NRS], "fsub");
    asm volatile("fmul.s %0, %1, %2" : "=f"(f_res) : "f"(a), "f"(b));
    check_float_result(f_res, expected_float_op_results[i][6][NRS], "fmul");

    u_int32_t i_res;
    asm volatile("feq.s %0, %1, %2" : "=r"(i_res) : "f"(a), "f"(b));
    check_int_result(i_res, expected_float_op_results[i][7][NRS], "feq");
    asm volatile("flt.s %0, %1, %2" : "=r"(i_res) : "f"(a), "f"(b));
    check_int_result(i_res, expected_float_op_results[i][8][NRS], "flt");
    asm volatile("fle.s %0, %1, %2" : "=r"(i_res) : "f"(a), "f"(b));
    check_int_result(i_res, expected_float_op_results[i][9][NRS], "fle");
    asm volatile("fabs.s %0, %1" : "=f"(f_res) : "f"(a));
    check_float_result(f_res, expected_float_op_results[i][10][NRS], "fabs");
    asm volatile("fneg.s %0, %1" : "=f"(f_res) : "f"(a));
    check_float_result(f_res, expected_float_op_results[i][11][NRS], "fneg");
    asm volatile("fmax.s %0, %1, %2" : "=f"(f_res) : "f"(a), "f"(b));
    check_float_result(f_res, expected_float_op_results[i][12][NRS], "fmax");
    asm volatile("fmin.s %0, %1, %2" : "=f"(f_res) : "f"(a), "f"(b));
    check_float_result(f_res, expected_float_op_results[i][13][NRS], "fmin");
    asm volatile("fdiv.s %0, %1, %2" : "=f"(f_res) : "f"(a), "f"(b));
    check_float_result(f_res, expected_float_op_results[i][14][NRS], "fdiv");
    asm volatile("fsqrt.s %0, %1" : "=f"(f_res) : "f"(a));
    check_float_result(f_res, expected_float_op_results[i][15][NRS], "fsqrt");
  }
}

void test_double_operations() {
  printf("Testing double operations...\n");

  for (int32_t i = 0; i < sizeof(double_inputs) / sizeof(double_inputs[0]); i++) {
    double a = double_inputs[i][0];
    double b = double_inputs[i][1];
    double c = double_inputs[i][2];

    double d_res;
    asm volatile("fmadd.d %0, %1, %2, %3" : "=f"(d_res) : "f"(a), "f"(b), "f"(c));
    check_double_result(d_res, expected_double_op_results[i][0][NRS], "fmadd");
    asm volatile("fmsub.d %0, %1, %2, %3" : "=f"(d_res) : "f"(a), "f"(b), "f"(c));
    check_double_result(d_res, expected_double_op_results[i][1][NRS], "fmsub");
    asm volatile("fnmadd.d %0, %1, %2, %3" : "=f"(d_res) : "f"(a), "f"(b), "f"(c));
    check_double_result(d_res, expected_double_op_results[i][2][NRS], "fnmadd");
    asm volatile("fnmsub.d %0, %1, %2, %3" : "=f"(d_res) : "f"(a), "f"(b), "f"(c));
    check_double_result(d_res, expected_double_op_results[i][3][NRS], "fnmsub");
    asm volatile("fadd.d %0, %1, %2" : "=f"(d_res) : "f"(a), "f"(b));
    check_double_result(d_res, expected_double_op_results[i][4][NRS], "fadd");
    asm volatile("fsub.d %0, %1, %2" : "=f"(d_res) : "f"(a), "f"(b));
    check_double_result(d_res, expected_double_op_results[i][5][NRS], "fsub");
    asm volatile("fmul.d %0, %1, %2" : "=f"(d_res) : "f"(a), "f"(b));
    check_double_result(d_res, expected_double_op_results[i][6][NRS], "fmul");

    u_int32_t i_res;
    asm volatile("feq.d %0, %1, %2" : "=r"(i_res) : "f"(a), "f"(b));
    check_long_result(i_res, expected_double_op_results[i][7][NRS], "feq");
    asm volatile("flt.d %0, %1, %2" : "=r"(i_res) : "f"(a), "f"(b));
    check_long_result(i_res, expected_double_op_results[i][8][NRS], "flt");
    asm volatile("fle.d %0, %1, %2" : "=r"(i_res) : "f"(a), "f"(b));
    check_long_result(i_res, expected_double_op_results[i][9][NRS], "fle");
    asm volatile("fabs.d %0, %1" : "=f"(d_res) : "f"(a));
    check_double_result(d_res, expected_double_op_results[i][10][NRS], "fabs");
    asm volatile("fneg.d %0, %1" : "=f"(d_res) : "f"(a));
    check_double_result(d_res, expected_double_op_results[i][11][NRS], "fneg");
    asm volatile("fmax.d %0, %1, %2" : "=f"(d_res) : "f"(a), "f"(b));
    check_double_result(d_res, expected_double_op_results[i][12][NRS], "fmax");
    asm volatile("fmin.d %0, %1, %2" : "=f"(d_res) : "f"(a), "f"(b));
    check_double_result(d_res, expected_double_op_results[i][13][NRS], "fmin");
    asm volatile("fdiv.d %0, %1, %2" : "=f"(d_res) : "f"(a), "f"(b));
    check_double_result(d_res, expected_double_op_results[i][14][NRS], "fdiv");
    asm volatile("fsqrt.d %0, %1" : "=f"(d_res) : "f"(a));
    check_double_result(d_res, expected_double_op_results[i][15][NRS], "fsqrt");
  }
}

void test_int_conversions() {
  printf("Testing int conversions...\n");

  for (int32_t i = 0; i < sizeof(int_inputs) / sizeof(int_inputs[0]); i++) {
    int32_t a = int_inputs[i];

    float f_res;
    f_res = (float)a;
    check_float_result(f_res, expected_int_to_float_results[i][NRS], "fcvt.s");

    double d_res;
    d_res = (double)a;
    check_double_result(d_res, expected_int_to_double_results[i][NRS], "fcvt.d");
  }
}

void test_long_conversions() {
  printf("Testing long conversions...\n");

  for (int32_t i = 0; i < sizeof(long_inputs) / sizeof(long_inputs[0]); i++) {
    int64_t a = long_inputs[i];

    float f_res;
    f_res = (float)a;
    check_float_result(f_res, expected_long_to_float_results[i][NRS], "fcvt.s");

    double d_res;
    d_res = (double)a;
    check_double_result(d_res, expected_long_to_double_results[i][NRS], "fcvt.d");
  }
}

void test_float_conversions() {
  printf("Testing float conversions...\n");

  for (int32_t i = 0; i < sizeof(float_inputs) / sizeof(float_inputs[0]); i++) {
    float a = float_inputs[i][0];

    int32_t i_res;
    i_res = (int32_t)a;
    check_int_result(i_res, expected_float_to_int_results[i][NRS], "fcvt.w");

    int64_t l_res;
    l_res = (int64_t)a;
    check_long_result(l_res, expected_float_to_long_results[i][NRS], "fcvt.l");

    double d_res;
    d_res = (double)a;
    check_double_result(d_res, expected_float_to_double_results[i][NRS], "fcvt.d");
  }
}

void test_double_conversions() {
  printf("Testing double conversions...\n");

  for (int32_t i = 0; i < sizeof(double_inputs) / sizeof(double_inputs[0]); i++) {
    double a = double_inputs[i][0];

    int32_t i_res;
    i_res = (int32_t)a;
    check_int_result(i_res, expected_double_to_int_results[i][NRS], "fcvt.w");

    int64_t l_res;
    l_res = (int64_t)a;
    check_long_result(l_res, expected_double_to_long_results[i][NRS], "fcvt.l");

    float f_res;
    f_res = (float)a;
    check_float_result(f_res, expected_double_to_float_results[i][NRS], "fcvt.s");
  }
}

int main() {
  test_float_operations();
  test_double_operations();
  test_int_conversions();
  test_long_conversions();
  test_float_conversions();
  test_double_conversions();

  return return_value;
}