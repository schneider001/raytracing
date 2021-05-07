#pragma once
#include "Vector.h"


using vector = Vector<double>;


class Light {
public:

	int type_;
	double intensity_;
	vector position_;

	Light(int type, double intensity, vector position) :
		type_(type),
		intensity_(intensity),
		position_(position)
	{}

	Light(int type, double intensity) :
		type_(type),
		intensity_(intensity),
		position_(vector(0, 0, 0))
	{}
};

