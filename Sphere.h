#pragma once

#include "Vector.h"
#include "Object.h"
#include <cfloat>

#include <cfloat>


using Color = Vector<double>;
using vector = Vector<double>;

class Sphere : public Object {
public:
	vector center_;
	double radius_;

	double rad_quad;


	Sphere operator = (const Sphere& that);

	bool operator == (const Sphere& that);

	virtual vector intersect_ray(const vector& coords, vector& dir) override;


	Sphere(const Sphere& that) :
		Object(that.color_, that.specular_, that.reflective_, that.type_),
		center_(that.center_),
		radius_(that.radius_),
		rad_quad(that.rad_quad)
	{}


	Sphere(vector center, double radius, Color color, int specular, double reflective) :
		Object(color, specular, reflective, 1),
		center_(center),
		radius_(radius),
		rad_quad(radius_* radius_)
	{}
};

