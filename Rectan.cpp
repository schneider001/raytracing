#include "Rectan.h"



Rectan Rectan::operator = (const Rectan& that) {
	coords0_ = that.coords0_;
	i_ = that.i_;
	j_ = that.j_;
	l1_ = that.l1_;
	l2_ = that.l2_;
	coords1_ = that.coords1_;
	coords2_ = that.coords2_;
	normal_ = that.normal_;
	D_ = that.D_;
	color_ = that.color_;
	specular_ = that.specular_;
	reflective_ = that.reflective_;
	refractive_ = that.refractive_;
	n_ = that.n_;
	return *this;
}


double Rectan::D_calculation() {
	return (-1) * normal_ * coords0_;
}


vector Rectan::normal_calculation() {
	return vector((coords1_.y_ - coords0_.y_) * (coords2_.z_ - coords0_.z_) - (coords2_.y_ - coords0_.y_) * (coords1_.z_ - coords0_.z_),
		(coords2_.x_ - coords0_.x_) * (coords1_.z_ - coords0_.z_) - (coords1_.x_ - coords0_.x_) * (coords2_.z_ - coords0_.z_),
		(coords1_.x_ - coords0_.x_) * (coords2_.y_ - coords0_.y_) - (coords2_.x_ - coords0_.x_) * (coords1_.y_ - coords0_.y_));
}


vector Rectan::normal(const vector& coords) {
	return normal_;
}


vector Rectan::intersect_ray(const vector& coords, vector& dir) {
	double normal_dot_dir = normal_ * dir;

	if (normal_dot_dir == 0) {
		return DBL_MAX;
	}

	double t = (-1) * (normal_ * coords + D_) / normal_dot_dir;
	vector intersection = coords + t * dir;
	vector new_coords = intersection + (-1) * coords0_;
	double pr_i = new_coords * i_ / i_.length();
	double pr_j = new_coords * j_ / j_.length();
	if (pr_i > 0 && pr_i < l1_ && pr_j > 0 && pr_j < l2_)
		return t;
	else
		return DBL_MAX;
}