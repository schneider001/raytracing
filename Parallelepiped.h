#pragma once


#include "Rectan.h"
#include "iostream"


class Parallelepiped : public Rectan {

	Rectan facets_[6];

public:
	vector k_;
	double l3_;


	Parallelepiped operator = (const Parallelepiped& that);


	virtual vector normal(const vector& coords) override;


	virtual vector intersect_ray(const vector& coords, vector& dir) override;


	Parallelepiped(const Parallelepiped& that) :
		Rectan(that.coords0_, that.i_, that.j_, that.l1_, that.l2_,
			   that.color_, that.specular_, that.reflective_, that.refractive_, that.n_, that.type_),
		k_(that.k_),
		l3_(that.l3_)
	{
		for (int i = 0; i < 6; i++)
			facets_[i] = that.facets_[i];
	}


	Parallelepiped(vector coords0, vector i, vector j, vector k, double l1, double l2, double l3,
		Color color, int specular, double reflective, double refractive, double n, int type) :
		Rectan(coords0, i, j, l1, l2, color, specular, reflective, refractive, n, type),
		k_(k.normalization()),
		l3_(l3)
	{
		facets_[0] = Rectan(coords0,          i, j, l1, l2, color, specular, reflective, refractive, n);
		facets_[1] = Rectan(coords0 + l3 * k, i, j, l1, l2, color, specular, reflective, refractive, n);
		facets_[2] = Rectan(coords0,          i, k, l1, l3, color, specular, reflective, refractive, n);
		facets_[3] = Rectan(coords2_,         i, k, l1, l3, color, specular, reflective, refractive, n);
		facets_[4] = Rectan(coords0,          k, j, l3, l2, color, specular, reflective, refractive, n);
		facets_[5] = Rectan(coords1_,         k, j, l3, l2, color, specular, reflective, refractive, n);
	}
};

