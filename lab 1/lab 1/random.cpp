#include "random.hpp"

#include <cstdlib>

double getRandomDouble() {
	return rand() / ((double)RAND_MAX);
}

unsigned int getRandomInt(unsigned int upperBound) {
	return ((unsigned int)(getRandomDouble() * upperBound));
}
