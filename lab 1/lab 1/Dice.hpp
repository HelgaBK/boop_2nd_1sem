#pragma once

#include <ostream>
#include <vector>

class Dice {
public:
	//Creates a dice with given surfaces count and random probabilities
	Dice(int surfacesCount);

	//Creates a dice with given probabilities (surfaces count becomes equal to vector size)
	Dice(const std::vector<double> &probabilities);

	~Dice();

	double calculateMean();

	//Get probability of getting given value when rolling the dice
	double getProbability(int value);

	int getSurfacesCount();

	void print(std::ostream &stream);

	void printCompact(std::ostream &stream);

	//Does the same as printCompact
	friend std::ostream& operator<<(std::ostream&, Dice&);

protected:
	double *probabilities;
	int surfacesCount;
};
