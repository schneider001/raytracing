#pragma once


#include "Vector.h"

using Color = Vector<double>;
using vector = Vector<double>;


class Object {
public:

	Color color_;
	int specular_;
	double reflective_;
	int type_;


	virtual vector intersect_ray(const vector& coords, vector& dir) = 0;


	Object(Color color, int specular, double reflective, int type) :
		color_(color),
		specular_(specular),
		reflective_(reflective),
		type_(type)
	{}
};


class End : public Object {
public:

	virtual vector intersect_ray(const vector& coords, vector& dir) {
		return vector(0, 0, 0);
	}

	End():
		Object(Color(0, 0, 0), 0, 0, 0)
	{}
};