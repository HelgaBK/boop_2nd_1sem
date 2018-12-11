#include "DiceSet.hpp"

#include <ostream>
#include <vector>
#include "Dice.hpp"

DiceSet::DiceSet(const std::vector<Dice*> &dice) {
	this->dice = std::vector<Dice*>(dice);
}

DiceSet::~DiceSet() {
	for (Dice* currentDice : this->dice) {
		delete currentDice;
	}
}

void DiceSet::addDice(Dice* dice) {
	this->dice.push_back(dice);
}

int DiceSet::calculateMaxSum() {
	int result = 0;

	for (Dice* dice : this->dice) {
		result += dice->getSurfacesCount();
	}

	return result;
}

double DiceSet::calculateMean() {
	double result = 0;

	for (Dice* currentDice : this->dice) {
		result += currentDice->calculateMean();
	}

	return result;
}

std::vector<double> *DiceSet::calculateProbabilities() {
	int maxSum = this->calculateMaxSum();
	int previousSum;
	double* previousProbabitities = new double[maxSum + 1];
	double* currentProbabilities = new double[maxSum + 1];

	previousProbabitities[0] = 1;
	for (int i = 1; i <= maxSum; i++) {
		previousProbabitities[i] = 0;
	}

	for (Dice* currentDice : this->dice) {
		for (int i = 0; i <= maxSum; i++) {
			currentProbabilities[i] = 0;
		}
		for (int currentSum = 1; currentSum <= maxSum; currentSum++) {
			for (int currentDiceValue = 1; currentDiceValue <= currentDice->getSurfacesCount(); currentDiceValue++) {
				previousSum = currentSum - currentDiceValue;
				if (previousSum >= 0) {
					currentProbabilities[currentSum] += previousProbabitities[previousSum] * currentDice->getProbability(currentDiceValue);
				}
			}
		}
		std::swap(previousProbabitities, currentProbabilities);
	}

	std::vector<double> *result = new std::vector<double>(previousProbabitities, previousProbabitities + (maxSum + 1));
	delete[] currentProbabilities;
	delete[] previousProbabitities;

	return result;
}

void DiceSet::print(std::ostream &stream) {
	std::vector<double> *probabilities = this->calculateProbabilities();
	bool shouldPrintProbability = false;

	stream << "Dice:";
	for (Dice* currentDice : this->dice) {
		stream << " " << *currentDice;
	}
	stream << "\n";
	stream << "Mean: " << this->calculateMean() << "\n";
	stream << "Probabilities:\n";
	for (size_t i = 0; i < probabilities->size(); i++) {
		if ((*probabilities)[i] > 0) {
			shouldPrintProbability = true;
		}
		if (shouldPrintProbability) {
			stream << i << ": " << (*probabilities)[i] << "\n";
		}
	}

	delete probabilities;
}

void DiceSet::printCompact(std::ostream &stream) {
	stream << "[" << this->dice.size() << ";" << this->calculateMean() << "]";
}

std::ostream& operator<<(std::ostream &stream, DiceSet &diceSet) {
	diceSet.printCompact(stream);

	return stream;
}
