#pragma once

#include <cstring>

//Returns a pointer to a new object that is an exact clone (copied byte by byte) of the one referenced by the passed pointer
template<typename T>
T* cloneObject(T* object) {
	T* result = new T;

	memcpy(result, object, sizeof(T));

	return result;
}
