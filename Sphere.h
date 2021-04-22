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

	double rad_quad;

	Sphere& operator = (const Sphere& that);

	bool operator == (const Sphere& that);


	Sphere(const Sphere& that):
		center_(that.center_),
		radius_(that.radius_),
		color_(that.color_),
		specular_(that.specular_),
		reflective_(that.reflective_),
		rad_quad(that.rad_quad)
	{}

	Sphere(vector center, double radius, Color color, int specular, double reflective):
		center_(center),
		radius_(radius),
		color_(color),
		specular_(specular),
		reflective_(reflective),
		rad_quad(radius_ * radius_)
	{}
};

