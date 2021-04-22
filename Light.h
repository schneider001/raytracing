#pragma once
#include "Vector.h"


using vector = Vector<double>;


class Light {
public:

	const char* type_;
	double intensity_;
	vector position_;

	Light(const char* type, double intensity, vector position) :
		type_(type),
		intensity_(intensity),
		position_(position)
	{}

	Light(const char* type, double intensity) :
		type_(type),
		intensity_(intensity),
		position_(vector(0, 0, 0))
	{}
};

