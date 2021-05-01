#pragma once


#include "Vector.h"
#include "Object.h"
#include <cfloat>
#include <iostream>

using Color = Vector<double>;
using vector = Vector<double>;


class Rectan : public Object {
public:
	vector coords0_;
	vector i_;
	vector j_;
	double l1_;
	double l2_;

	vector coords1_;
	vector coords2_;
	vector normal_;
	double D_;

	Rectan operator = (const Rectan& that);
	

	vector normal_calculation();


	double D_calculation();


	virtual vector normal(const vector& coords) override;


	virtual vector intersect_ray(const vector& coords, vector& dir) override;
	

	Rectan(vector coords0, vector i, vector j, double l1, double l2,
		Color color, int specular, double reflective, double refractive) :
		Object(color, specular, reflective, refractive, 1, 3),
		coords0_(coords0),
		i_(i.normalization()),
		j_(j.normalization()),
		l1_(l1),
		l2_(l2),
		coords1_(coords0_ + l1_ * i_),
		coords2_(coords0_ + l2_ * j_),
		normal_(normal_calculation().normalization()),
		D_(D_calculation())
	{}										 
};

