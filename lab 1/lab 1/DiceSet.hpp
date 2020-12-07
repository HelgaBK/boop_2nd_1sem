#pragma once

#include <ostream>
#include <vector>
#include "Dice.hpp"

class DiceSet {
public:
	//Creates an empty set
	DiceSet() {}

	//Creates a set with given dice. Dice must not be added to more than one set
	DiceSet(const std::vector<Dice*> &dice);

	//Deletes all the dice in the set
	~DiceSet();

	//Adds a single dice to the set
	void addDice(Dice* dice);

	int calculateMaxSum();

	double calculateMean();

	//Returns vector of size (max sum + 1) with elements equal to probability of getting element index when rolling all the dice in set
	std::vector<double> *calculateProbabilities();

	void print(std::ostream &stream);

	void printCompact(std::ostream &stream);

	//Does the same as printCompact
	friend std::ostream& operator<<(std::ostream &stream, DiceSet &diceSet);

private:
	std::vector<Dice*> dice;
};
