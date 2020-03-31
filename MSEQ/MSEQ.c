// Part of this code is taken from: https://www.codeproject.com/Articles/813185/Calculating-the-Number-PI-Through-Infinite-Sequenc
// Approximation of the number PI
// Language: C
// Author: Jose Cintra (jose.cintra@html-apps.info)

#define WITH_POSIT_32
#define SER4
#ifdef VIETE
#define WITH_SQRT
#endif

#ifdef PFDEBUG
#include <stdio.h>
#endif

#include<stdint.h>

float sqrt_asm(float x) {
    float res;
    asm("fsqrt.s %0,%1\n\t" : "=f" (res) : "f" (x) : "cc");
    return res;
}

typedef float element_t;

// variables
element_t pi = 0.0;
element_t a_n = 0.0;
element_t a_n_minus_one = 0.0;
element_t a_n_minus_two = 0.0;
element_t s_n = 0.0;
element_t sign = 1.0;
element_t i = 1.0;
element_t prev_power = 0.0;
element_t prev_fac = 0.0;
element_t power = 0.0;
element_t fac = 0.0;
element_t x_element = 0.0;
element_t result = 0.0;

// constants
element_t minus_one = -1.0;
element_t one = 1.0;
element_t two = 2.0;
element_t three = 3.0;
element_t four = 4.0;

#if (defined WITH_POSIT_32)
/*
// posit(32,2)
uint32_t posit_zero = 0x00000000;
uint32_t posit_one = 0x40000000;
uint32_t posit_two = 0x48000000;
uint32_t posit_three = 0x4c000000;
uint32_t posit_four = 0x50000000;
*/
// posit(32,3)
uint32_t posit_zero = 0x00000000;
uint32_t posit_one = 0x40000000;
uint32_t posit_two = 0x44000000;
uint32_t posit_three = 0x46000000;
uint32_t posit_four = 0x48000000;
uint32_t posit_minus_one = 0xc0000000;
#elif (defined WITH_POSIT_16)
uint32_t posit_zero = 0x00000000;
uint32_t posit_one = 0x00004000;
uint32_t posit_two = 0x00004800;
uint32_t posit_three = 0x00004c00;
uint32_t posit_four = 0x00005000;
uint32_t posit_minus_one = 0x0000c000;
#elif (defined WITH_POSIT_8)
uint32_t posit_zero = 0x00000000;
uint32_t posit_one = 0x00000040;
uint32_t posit_two = 0x00000048;
uint32_t posit_three = 0x0000004c;
uint32_t posit_four = 0x00000050;
uint32_t posit_minus_one = 0x000000c0;
#else
uint32_t fp32_zero = 0x00000000;
uint32_t fp32_one = 0x3f800000;
uint32_t fp32_two = 0x40000000;
uint32_t fp32_three = 0x40400000;
uint32_t fp32_four = 0x40800000;
uint32_t fp32_minus_one = 0xbf800000;
#endif /* WITH_POSIT */

void init() {
#if (defined WITH_POSIT_8 || defined WITH_POSIT_16 || defined WITH_POSIT_32)
  *((uint32_t*)&pi) = posit_zero;
  *((uint32_t*)&one) = posit_one;
  *((uint32_t*)&two) = posit_two;
  *((uint32_t*)&three) = posit_three;
  *((uint32_t*)&four) = posit_four;
  *((uint32_t*)&sign) = posit_one;
  *((uint32_t*)&minus_one) = posit_minus_one;
#else
  *((uint32_t*)&pi) = fp32_zero;
  *((uint32_t*)&one) = fp32_one;
  *((uint32_t*)&two) = fp32_two;
  *((uint32_t*)&three) = fp32_three;
  *((uint32_t*)&four) = fp32_four;
  *((uint32_t*)&sign) = fp32_one;
  *((uint32_t*)&minus_one) = fp32_minus_one;
#endif /* WITH_POSIT */
}

void calculate_sequence_1(int n) {
#if (defined WITH_POSIT_8 || defined WITH_POSIT_16 || defined WITH_POSIT_32)
  *((uint32_t*)&a_n) = posit_zero;
  *((uint32_t*)&i) = posit_one;
#else
  *((uint32_t*)&a_n) = fp32_zero;
  *((uint32_t*)&i) = fp32_one;
#endif
  int index;
  for(index = 1; index <= n; index++){
      a_n = ((i*i*three) - one) / (i + i*i*two);
      i = i + one;
  }
  result = a_n;
#ifdef PFDEBUG
  printf("\nAproximated value of PI = %1.16lf\n", pi);  
#endif
}

void calculate_sequence_2(int n) {
#if (defined WITH_POSIT_8 || defined WITH_POSIT_16 || defined WITH_POSIT_32)
  *((uint32_t*)&a_n) = posit_zero;
  *((uint32_t*)&a_n_minus_one) = posit_one;
#else
  *((uint32_t*)&a_n) = fp32_zero;
  *((uint32_t*)&a_n_minus_one) = fp32_one;
#endif
  int index;
  for(index = 1; index <= n; index++){
      a_n = (a_n_minus_one + two/a_n_minus_one)/two;
      a_n_minus_one = a_n;
  }
  result = a_n;
#ifdef PFDEBUG
  printf("\nAproximated value of PI = %1.16lf\n", pi);  
#endif
}


void calculate_sequence_3(int n) {
#if (defined WITH_POSIT_8 || defined WITH_POSIT_16 || defined WITH_POSIT_32)
  *((uint32_t*)&a_n) = posit_zero;
  *((uint32_t*)&a_n_minus_one) = posit_one;
  *((uint32_t*)&a_n_minus_two) = posit_one;
  *((uint32_t*)&s_n) = posit_zero;
#else
  *((uint32_t*)&a_n) = fp32_zero;
  *((uint32_t*)&a_n_minus_one) = fp32_one;
  *((uint32_t*)&a_n_minus_two) = fp32_one;
  *((uint32_t*)&s_n) = fp32_zero;
#endif
  int index;
  for(index = 1; index <= n; index++){
      a_n = a_n_minus_one + a_n_minus_two;
      s_n = a_n/a_n_minus_one;
      a_n_minus_two = a_n_minus_one;
      a_n_minus_one = a_n;
  }
  result = s_n;
#ifdef PFDEBUG
  printf("\nAproximated value of PI = %1.16lf\n", pi);  
#endif
}


void calculate_series_1(int n) {
#if (defined WITH_POSIT_8 || defined WITH_POSIT_16 || defined WITH_POSIT_32)
  *((uint32_t*)&a_n) = posit_zero;
  *((uint32_t*)&s_n) = posit_zero;
  *((uint32_t*)&i) = posit_one;
#else
  *((uint32_t*)&a_n) = fp32_zero;
  *((uint32_t*)&s_n) = fp32_zero;
  *((uint32_t*)&i) = fp32_one;
#endif
  int index;
  for(index = 1; index <= n; index++){
      a_n = one / (i * i);
      s_n = s_n + a_n;
      i = i + one;
  }
  result = s_n;
#ifdef PFDEBUG
  printf("\nAproximated value of PI = %1.16lf\n", pi);  
#endif
}


void calculate_series_2(int n) {
#if (defined WITH_POSIT_8 || defined WITH_POSIT_16 || defined WITH_POSIT_32)
  *((uint32_t*)&a_n) = posit_zero;
  *((uint32_t*)&s_n) = posit_zero;
  *((uint32_t*)&i) = posit_one;
#else
  *((uint32_t*)&a_n) = fp32_zero;
  *((uint32_t*)&s_n) = fp32_zero;
  *((uint32_t*)&i) = fp32_one;
#endif
  int index;
  for(index = 1; index <= n; index++){
      a_n = one / (i * (i + one));
      s_n = s_n + a_n;
      i = i + one;
  }
  result = s_n;
#ifdef PFDEBUG
  printf("\nAproximated value of PI = %1.16lf\n", pi);  
#endif
}


void calculate_series_3(int n) {
#if (defined WITH_POSIT_8 || defined WITH_POSIT_16 || defined WITH_POSIT_32)
  *((uint32_t*)&a_n) = posit_zero;
  *((uint32_t*)&s_n) = posit_one;
  *((uint32_t*)&i) = posit_two;
  *((uint32_t*)&sign) = posit_one;
#else
  *((uint32_t*)&a_n) = fp32_zero;
  *((uint32_t*)&s_n) = fp32_one;
  *((uint32_t*)&i) = fp32_two;
  *((uint32_t*)&sign) = fp32_one;
#endif
  int index;
  for(index = 1; index <= n; index++){
      sign = sign * minus_one;
      a_n = sign * (one / i);
      s_n = s_n + a_n;
      i = i + one;
  }
  result = s_n;
#ifdef PFDEBUG
  printf("\nAproximated value of PI = %1.16lf\n", pi);  
#endif
}


void calculate_series_4(int n) {
#if (defined WITH_POSIT_8 || defined WITH_POSIT_16 || defined WITH_POSIT_32)
  *((uint32_t*)&a_n) = posit_zero;
  *((uint32_t*)&x_element) = posit_one;
  *((uint32_t*)&s_n) = posit_one;
  *((uint32_t*)&i) = posit_two;
  *((uint32_t*)&sign) = posit_one;
#else
  *((uint32_t*)&a_n) = fp32_zero;
  *((uint32_t*)&x_element) = fp32_one;
  *((uint32_t*)&s_n) = fp32_one;
  *((uint32_t*)&i) = fp32_two;
  *((uint32_t*)&sign) = fp32_one;
#endif
  int index;
  s_n = x_element;
  prev_power = x_element;
  prev_fac = one;

  for(index = 1; index <= n; index++){
      sign = sign * minus_one;
      power = prev_power * x_element * x_element;
      fac = prev_fac * i * (i + one);
      s_n = s_n + sign * (power/fac);
      prev_power = power;
      prev_fac = fac;
      i = i + two;
  }
  result = s_n;
#ifdef PFDEBUG
  printf("\nAproximated value of PI = %1.16lf\n", pi);  
#endif
}


void test_conversion(int n) {
#if (defined WITH_POSIT_8 || defined WITH_POSIT_16 || defined WITH_POSIT_32)
  *((uint32_t*)&a_n) = posit_zero;
  *((uint32_t*)&x_element) = posit_one;
  *((uint32_t*)&s_n) = posit_one;
  *((uint32_t*)&i) = posit_one;
  *((uint32_t*)&sign) = posit_one;
#else
  *((uint32_t*)&a_n) = fp32_zero;
  *((uint32_t*)&x_element) = fp32_one;
  *((uint32_t*)&s_n) = fp32_one;
  *((uint32_t*)&i) = fp32_two;
  *((uint32_t*)&sign) = fp32_one;
#endif
  int index;
  int jindex;

  for(index = 1; index <= n; index++){
      x_element = (float) index;
      jindex = (int) i;
      i = i + one;
  }
#ifdef PFDEBUG
  printf("\nAproximated value of PI = %1.16lf\n", pi);  
#endif
}


int main() {
  init();

  test_conversion(100);

  #ifdef SEQ1
  calculate_sequence_1(100);
  #endif
  #ifdef SEQ2
  calculate_sequence_2(100);
  #endif
  #ifdef SEQ3
  calculate_sequence_3(100);
  #endif
  #ifdef SER1
  calculate_series_1(100);
  #endif
  #ifdef SER2
  calculate_series_2(100);
  #endif
  #ifdef SER3
  calculate_series_3(100);
  #endif
  #ifdef SER4
  calculate_series_4(100);
  #endif
  return 0;
}