#pragma once


#include "Object.h"


class Sphere : public Object {
public:
	vector center_;
	double radius_;



	Sphere operator = (const Sphere& that);

	bool operator == (const Sphere& that);

	virtual vector intersect_ray(const vector& coords, vector& dir) override;

	virtual vector normal(const vector& coords) override;


	Sphere(const Sphere& that) :
		Object(that.color_, that.specular_, that.reflective_, that.refractive_, that.n_, that.type_),
		center_(that.center_),
		radius_(that.radius_)
	{}


	Sphere(vector center, double radius, Color color, int specular, double reflective, double refractive, double n, int type=1) :
		Object(color, specular, reflective, refractive, n, type),
		center_(center),
		radius_(radius)
	{}
};

