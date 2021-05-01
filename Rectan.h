#pragma once


#include "Object.h"
#include "Sphere.h"



class Rectan : public Object {

	vector normal_calculation();

	double D_calculation();

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


	virtual vector normal(const vector& coords) override;


	virtual vector intersect_ray(const vector& coords, vector& dir) override;


	Rectan(const Rectan& that) :
		Object(that.color_, that.specular_, that.reflective_, that.refractive_, that.n_, that.type_),
		coords0_(that.coords0_),
		i_(that.i_),
		j_(that.j_),
		l1_(that.l1_),
		l2_(that.l2_),
		coords1_(that.coords1_),
		coords2_(that.coords2_),
		normal_(that.normal_),
		D_(that.D_)
	{}


	Rectan() :
		Object(Color(0, 0, 0), 0, 0, 0.1, 1, 3),
		coords0_(vector(100, 100, 100)),
		i_(vector(0, 1, 0)),
		j_(vector(1, 0, 0)),
		l1_(1),
		l2_(1),
		coords1_(coords0_ + l1_ * i_),
		coords2_(coords0_ + l2_ * j_),
		normal_(normal_calculation().normalization()),
		D_(D_calculation())
	{}
	

	Rectan(vector coords0, vector i, vector j, double l1, double l2,
		Color color, int specular, double reflective, double refractive, double n, int type = 3) :
		Object(color, specular, reflective, refractive, n, type),
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

