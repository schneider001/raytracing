#pragma once


#include "Vector.h"
#include "Object.h"
#include <cfloat>


using Color = Vector<double>;
using vector = Vector<double>;


class Plane : public Object {
public:

	vector normal_;
	double D_;


	Plane operator = (const Plane& that);


	virtual vector intersect_ray(const vector& coords, vector& dir) override;

	virtual vector normal(const vector& coords) override;


	Plane(const Plane& that):
		Object(that.color_, that.specular_, that.reflective_, that.refractive_, that.n_, that.type_),
		normal_(that.normal_),
		D_(that.D_)
	{
		normal_.normalization();
	}


	Plane(vector normal, double D, Color color, int specular, double reflective, double refractive) :
		Object(color, specular, reflective, refractive, 1, 2),
		normal_(normal),
		D_(D)
	{
		normal_.normalization();
	}
};

