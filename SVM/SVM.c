//SVM-SMO
// http://cs229.stanford.edu/materials/smo.pdf
// Ciocirlan Stefan-Dan 14.05.2019 ver 1.0

#include <stdlib.h>
#include "util.h"
#include "../common/common.h"

#define MAX_ITERATIONS 15
#define MAX_GLOBAL_ITERATIONS 100


real_t g_alphas[TRAINING_DATA_LENGTH];
real_t g_b = 0;

/*
* Initialize alpha with zeros
* Because of the the way svm works in classifing it is better not to have zero as a class
*/
void initialize_svm(real_t alphas[], element_t training_data_Y[],
                    size_t training_data_length, real_t *b) {
    size_t index;
    for(index = 0; index < training_data_length; index++) {
        alphas[index] = 0;
        training_data_Y[index] = training_data_Y[index] + 1;
    }
    *b = 0;
}

/*
* Calculate the inner product between two vectors X and Y with length given
*/
element_t inner_product (element_t X[], element_t Y[], size_t length) {
    size_t index;
    element_t sum = 0;
    for(index = 0; index < length; index++) {
        sum = sum + X[index] * Y[index];
    }
    return sum;
}

/*
* Caclulate the kernel function of two vector X and Y of a given length
* This function is chosen by the programmer
*/
element_t kernel_function (element_t X[], element_t Y[], size_t length) {
    return inner_product (X, Y, length);
}

/*
* calcualte de svm function in a point X
* f(x) = sum for i in [0, training_data_length] of (alpha(i)*y(i)*Kern(x(i), x)) + b
*/
real_t svm_function (element_t training_data_X[][INPUT_LENGTH], element_t training_data_Y[],
                    size_t training_data_length, size_t input_length,
                    element_t X[], real_t alphas[], real_t b) {
    size_t index;
    real_t sum = 0;
    for( index = 0; index < training_data_length; index++) {
        sum = sum + alphas[index] * training_data_Y[index]
            * kernel_function(training_data_X[index], X, input_length) + b;
    }
    return sum;
}

/*
* A random function
*/
size_t my_random (size_t i, size_t j, size_t max) {
    size_t a;
    a = (j + 1) % max;
    if (a == i) {
        return my_random(i, a, max);
    }
    return a;
}

/*
* Create de SVM
*/
void train_svm(element_t training_data_X[][INPUT_LENGTH], element_t training_data_Y[],
               size_t training_data_length, size_t input_length,
               real_t alphas[], real_t *b,
               real_t C, real_t tol, real_t max_iterations) {
    real_t E_i;
    real_t E_j;
    real_t old_alpha_i;
    real_t old_alpha_j;
    real_t L, H;
    real_t miu;
    real_t b1, b2;
    size_t index, jindex;
    size_t current_iterations = 0;
    size_t num_changed_alphas = 0;
    size_t global_iterations = 0;

    /*
    * If the alpha vector has not change for a given number of itereations than
    * the algorithm finish
    */
    while (current_iterations < max_iterations &&
           global_iterations < MAX_GLOBAL_ITERATIONS) {
        num_changed_alphas = 0;
        // going trough training data
        for (index = 0; index < training_data_length; index++) {
            // calcualte E(i) = f(x(i)) - y(i) 
            E_i = svm_function(training_data_X, training_data_Y,
                               training_data_length, input_length,
                               training_data_X[index], alphas, *b)
                  - training_data_Y[index];
            // if ((y(i)E(i) <−tol && alpha(i) <C)||(y(i)E(i) >tol && alpha(i) >0))
            if (
                ((training_data_Y[index] * E_i < -tol) && (alphas[index] < C)) ||
                ((training_data_Y[index] * E_i > tol) && (alphas[index] > 0))
            ) {
                // select j random j!=i
                jindex = my_random(index, jindex, training_data_length);
                // Ej = f(x(j)) − y(j)
                E_j = svm_function(training_data_X, training_data_Y,
                               training_data_length, input_length,
                               training_data_X[jindex], alphas, *b)
                     - training_data_Y[jindex];
                // save old alphas
                old_alpha_i = alphas[index];
                old_alpha_j = alphas[jindex];
                /* Compute L and H
                * If y(i)=y(j), L=max(0,alpha(i)+alpha(j)−C),H=min(C,alpha(i)+alpha(j))
                * If y(i) ̸=y(j), L=max(0,alpha(j)−alpha(i)),H=min(C,C+alpha(j)−alpha(i))
                */
                if(training_data_Y[index] == training_data_Y[jindex]) {
                    L = ((alphas[jindex] + alphas[index] - C) > 0) ?
                        (alphas[jindex] + alphas[index] - C) :
                        0;
                    H = (C < (alphas[jindex] + alphas[index])) ?
                        C :
                        (alphas[jindex] + alphas[index]);
                } else {
                    L = ((alphas[jindex] - alphas[index]) > 0) ?
                        (alphas[jindex] - alphas[index]) :
                        0;
                    H = (C < (C + alphas[jindex] - alphas[index])) ?
                        C :
                        (C + alphas[jindex] - alphas[index]);
                }
                // if (L == H) continue to next i.
                if (L == H) {
                    continue;
                }
                /* Compute miu
                * miu = 2 Kern(x(i), x(j)) − Kern(x(i), x(i)) − Kern(x(j), x(j)).
                */
                miu = 2 * kernel_function(training_data_X[index], training_data_X[jindex], input_length)
                      - kernel_function(training_data_X[index], training_data_X[index], input_length)
                      - kernel_function(training_data_X[jindex], training_data_X[jindex], input_length);
                // if (miu>=0) continue to next i.
                if (miu >= 0) {
                    continue;
                }
                /* Compute and clip new value for alpha(j)
                * alpha(j) = alpja(j) − y(j)(E(i) − E(j)) / miu
                * if alpha(j) > H alpha(j) = H
                * if alpha(j) < H and alpha(j) > L alpha(j) = alpha(j)
                * if alpha(j) < L alpha(j) = L
                */
                alphas[jindex] = alphas[jindex] -
                                (training_data_Y[index] * (E_i - E_j)) / miu;
                alphas[jindex] = (alphas[jindex] > H) ?
                            H :
                            ((alphas[jindex] < L) ?
                                L :
                                alphas[jindex]
                            );
                // if ( abs(alpha(j) − alpha_old(j)) < 1e−5) continue to next i.
                if (
                    ((alphas[jindex] - old_alpha_j) < 0.00001) &&
                    ((alphas[jindex] - old_alpha_j) > -0.00001)
                ) {
                    continue;
                }
                /* Determine value for alpha(i)
                * alpha(i) = alpha(i) + y(i)y(j)(alpha_old(j) − alpha(j) )
                */
                alphas[index] = alphas[index] + training_data_Y[index] *
                                training_data_Y[jindex] * (old_alpha_j - alphas[jindex]);
                /* Compute b1 and b2
                * b1 = b − E(i) − y(i)(alpha(i) − alpha_old(i))*Kern(x(i), x(i))
                *      − y(j)(alpha(j) − alpha_old(j)) * Kern(x(i), x(j))
                * b1 = b − E(j) − y(i)(alpha(i) − alpha_old(i))*Kern(x(i), x(j))
                *      − y(j)(alpha(j) − alpha_old(j)) * Kern(x(j), x(j))
                */
                b1 = *b - E_i - training_data_Y[index] * (alphas[index] - old_alpha_i)
                    * kernel_function(training_data_X[index], training_data_X[index], input_length)
                    - training_data_Y[jindex] * (alphas[jindex] - old_alpha_j)
                    * kernel_function(g_training_data_X[index], training_data_X[jindex], input_length);
                b2 = *b - E_j - training_data_Y[index] * (alphas[index] - old_alpha_i)
                    * kernel_function(training_data_X[index], training_data_X[jindex], input_length)
                    - training_data_Y[jindex] * (alphas[jindex] - old_alpha_j)
                    * kernel_function(training_data_X[jindex], training_data_X[jindex], input_length);
                /* Compute b
                * if 0 < alpha(i) < C b = b1
                * else if 0 < alpha(j) < C b = b2
                * else b = (b1+b2)/2
                */
                if (
                    (alphas[index] > 0) &&
                    (alphas[index] < C)
                ) {
                    *b = b1;
                } else {
                    if (
                        (alphas[jindex] > 0) &&
                        (alphas[jindex] < C)
                    ) {
                        *b = b2;
                    } else {
                        *b = (b1+b2)/2;
                    }
                }
                //alpha has changed
                num_changed_alphas = num_changed_alphas + 1;
            }
        }

        global_iterations = global_iterations + 1;
        // if alpha has not change increased the itereations
        if (num_changed_alphas == 0) {
            current_iterations = current_iterations + 1;
        } else {
            current_iterations = 0;
        }
    }
}


int main(void)
{
	real_t C = 100;
	real_t tol = 1;
    
    initialize_svm(g_alphas, g_training_data_Y, TRAINING_DATA_LENGTH, &g_b);
    train_svm(g_training_data_X, g_training_data_Y,
              TRAINING_DATA_LENGTH, INPUT_LENGTH,
              g_alphas, &g_b,
              C, tol, MAX_ITERATIONS);
    real_t answer = svm_function(g_training_data_X, g_training_data_Y, TRAINING_DATA_LENGTH,
                                 INPUT_LENGTH, g_test_data, g_alphas, g_b);

        #ifdef DEBUG
        printf("Result: %f\n", answer);
    #endif
    return 0;
}
