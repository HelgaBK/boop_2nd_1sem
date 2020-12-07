#include "Dice.hpp"

#include <ostream>
#include <vector>
#include "random.hpp"

Dice::Dice(int surfacesCount) {
	double probabilityLeft = 1;
	int surfacesLeft = surfacesCount;

	this->surfacesCount = surfacesCount;
	this->probabilities = new double[this->surfacesCount];
	for (int i = 0; i < surfacesCount - 1; i++) {
		this->probabilities[i] = (2 * getRandomDouble() * (probabilityLeft / surfacesLeft));
		probabilityLeft -= this->probabilities[i];
		surfacesLeft--;
	}
	this->probabilities[surfacesCount - 1] = probabilityLeft;
}

Dice::Dice(const std::vector<double> &probabilities) {
	this->surfacesCount = probabilities.size();
	this->probabilities = new double[this->surfacesCount];

	for (int i = 0; i < this->surfacesCount; i++) {
		this->probabilities[i] = probabilities[i];
	}
}

Dice::~Dice() {
	delete[] this->probabilities;
}

double Dice::calculateMean() {
	double result = 0;

	for (int i = 0; i < this->surfacesCount; i++) {
		result += (this->probabilities[i] * (i + 1));
	}

	return result;
}

double Dice::getProbability(int value) {
	return this->probabilities[value - 1];
}

int Dice::getSurfacesCount() {
	return this->surfacesCount;
}

void Dice::print(std::ostream &stream) {
	stream << "Surfaces count: " << this->surfacesCount << "\n";
	stream << "Probabilities:";
	for (int i = 0; i < this->surfacesCount; i++) {
		stream << " " << this->probabilities[i];
	}
	stream << "\n";
	stream << "Mean: " << this->calculateMean() << "\n";
}

void Dice::printCompact(std::ostream &stream) {
	stream << "(" << this->surfacesCount << ";" << this->calculateMean() << ")";
}

std::ostream& operator<<(std::ostream &stream, Dice &dice) {
	dice.printCompact(stream);

	return stream;
}
