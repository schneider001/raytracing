#pragma once


#include "Vector.h"
#include <cfloat>

using Color = Vector<double>;
using vector = Vector<double>;


class Object {
public:

	Color color_;
	double specular_;
	double reflective_;
	double refractive_;
	double n_;
	int type_;


	Object operator = (const Object& that);


	virtual vector intersect_ray(const vector& coords, vector& dir) {
		return vector(0, 0, 0);
	}

	virtual vector normal(const vector& coords) {
		return vector(0, 0, 0);
	}


	Object(Color color, int specular, double reflective, double refractive, double n, int type) :
		color_(color),
		specular_(specular),
		reflective_(reflective),
		refractive_(refractive),
		n_(n),
		type_(type)
	{}
};


class End : public Object {
public:

	virtual vector intersect_ray(const vector& coords, vector& dir) {
		return vector(0, 0, 0);
	}

	End():
		Object(Color(0, 0, 0), 0, 0, 0, 0, 0)
	{}
};