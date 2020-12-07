#pragma once

template<typename T>
class Vertex;

template<typename T>
class Edge {
public:
	//Assigns given values to class members
	Edge(Vertex<T> *vertex, unsigned int length);

	Vertex<T> *vertex;
	unsigned int length;
};

template class Edge<int>;
