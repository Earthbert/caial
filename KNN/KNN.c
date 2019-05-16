//KNN
// https://www.geeksforgeeks.org/k-nearest-neighbours/
// we use euclidian_distance_square instead of euclidian_distance
// Ciocirlan Stefan-Dan 14.05.2019

#include <stdlib.h>
#include "util.h"
#include "../common/common.h"

#define K_VALUE 7
element_t g_distances[K_VALUE];
element_t g_values[K_VALUE];
/* Calculate the euclidian distance between vectors X and Y with a length given
*/
element_t euclidian_distance_square (element_t X[], element_t Y[], size_t length) {
    size_t index;
    element_t sum = 0;
    for(index = 0; index < length; index++) {
        sum = sum + (X[index]-Y[index])*(X[index]-Y[index]);
    }
    return sum;
}

/* Classify a given point test_data_X with a KNN network with training_data_length
* points with position training_data_X and values training_data_Y with given k value
* for the bumber of neighbors. It also calculate de distances to this neighbors with
* their values
*/
element_t classifyAPoint(element_t training_data_X[][], element_t training_data_Y[],
												 size_t training_data_length, size_t input_length,
												 element_t test_data_X[], size_t k, element_t distances[],
												 element_t values[]) {
	size_t index, jidenx, kindex;
	element_t distance;
	size_t max_frequency;
	size_t return_value;
	size_t current_frequency;

	//initialise distances and values
	for(kindex = 0; kindex < k; kindex++) {
		distances[kindex] = 999999;
		values[kindex] = -1;
	}
	/*
	* Find the closest k points from training data points to our test data point
	* distance is a sort vector of points distance with smallest distance on position 0
	* values is the value of the points above
	*/
	for (index = 0; index < training_data_length; index++) {
		//find the current distance
		distance = euclidian_distance_square(training_data_X[index], test_data_X, input_length);
		//find if its position in the distance vector
		for(kindex = 0; kindex < k; kindex++) {
			if(distance < distances[kindex]) {
				//move all other points on position to the right
				for(jindex = k - 1; jindex > kindex; jidnex--) {
					distances[jidnex] = distances[jidnex-1];
					values[jidnex] = values[jidnex-1];
				}
				//put the point on its position
				distances[kindex] = distance;
				values[kindex] = training_data_Y[index];
				break;
			}
		}
	}

	//calculate the value with the higher frecuency in out k points
	max_frequency = 1;
	return_value = values[0];
	for(kindex = 0; kindex < k; kindex++) {
		current_frequency = 1;
		/*find other points with the same value
		going from kindex+1 because if the value was before the current
		point than was already counted
		*/
		for(jidnex = kindex + 1; jindex < k; jidnex++) {
			if(values[kindex] == values[jidnex]) {
				current_frequency = current_frequency + 1;
			}
		}
		//if it is the the new max frequency value
		if(current_frequency > max_frequency) {
			return_value = values[kindex];
			max_frequency = current_frequency;
		}
	}
	
	return return_value; 
} 

int main() 
{
	// Parameter to decide groupr of the testing point 
	size_t answer = classifyAPoint(g_training_data_X, g_training_data_Y,
												 TRAINING_DATA_LENGTH, INPUT_LENGTH,
												 g_test_data, K_VALUE, g_distances,
												 g_values); 

  #ifdef DEBUG
    printf("Result: %d\n", answer);
  #endif
	return 0; 
} 

