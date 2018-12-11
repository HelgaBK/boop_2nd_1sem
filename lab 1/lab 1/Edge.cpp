#include "Edge.hpp"

#include <vector>

template<typename T>
Edge<T>::Edge(Vertex<T> *vertex, unsigned int length) {
	this->vertex = vertex;
	this->length = length;
}
