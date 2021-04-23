#include "Plane.h"


Plane Plane::operator = (const Plane& that) {
	normal_ = that.normal_;
	D_ = that.D_;
	color_ = that.color_;
	specular_ = that.specular_;
	reflective_ = that.reflective_;
}


vector Plane::intersect_ray(const vector& coords, vector& dir) {

	double normal_dot_dir = normal_ * dir;

	if (normal_dot_dir == 0) {
		return DBL_MAX;
	}

	double t = (-1) * (normal_ * coords + D_) / normal_dot_dir;
	return vector(t, 0, 0);
}