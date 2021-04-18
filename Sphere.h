#pragma once

#include "Vector.h"
using Color = Vector<double>;
using vector = Vector<double>;

class Sphere {
public:
	vector center_;
	double radius_;
	Color color_;
	int specular_;
	double reflective_;

	Sphere& operator = (const Sphere& that);

	bool operator == (const Sphere& that);


	Sphere(const Sphere& that);

	Sphere(vector center, double radius, Color color, int specular, double reflective);
};

