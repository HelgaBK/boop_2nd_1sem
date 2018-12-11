#pragma once

#include <vector>
#include "Edge.hpp"

template<typename T>
class Vertex {
public:
	T *data;
	std::vector<Edge<T>*> incidentEdges;

	//Creates a vertex with given data
	Vertex(T *data);

	~Vertex();

	//Adds an edge to given vertex of given length
	void addEdge(Vertex<T>* edgeEnd, unsigned int length);

	//Removes an edge to given vertex
	void removeEdge(Vertex<T>*);
};
