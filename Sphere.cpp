#include "Sphere.h"


Sphere  Sphere:: operator = (const Sphere& that) {
		center_ = that.center_;
		radius_ = that.radius_;
		color_ = that.color_;
		specular_ = that.specular_;
		reflective_ = that.reflective_;
		refractive_ = that.refractive_;
		n_ = that.n_;
		return *this;
}


bool Sphere::operator == (const Sphere& that) {
		if (this->radius_ == that.radius_ && 
			this->center_ == that.center_ && 
			this->color_ == that.color_ && 
			this->specular_ == that.specular_ &&
			this->reflective_ == that.reflective_)
			return true;
		else
			return false;
}


vector Sphere::intersect_ray(const vector& coords, vector& dir) {

	vector OC = coords + (-1) * center_;
	double a = dir * dir;
	double b = 2 * OC * dir;
	double c = OC * OC - rad_quad;

	double discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return vector(DBL_MAX, DBL_MAX);
	}

	double root_disc = sqrt(discriminant);
	double a_twice = 2 * a;

	double t1 = (-b + root_disc) / a_twice;
	double t2 = (-b - root_disc) / a_twice;
	return vector(t1, t2);
}


vector Sphere::normal(const vector& coords) {
	vector perpendicular = coords + (-1) * center_;
	perpendicular.normalization();
	return perpendicular;
}