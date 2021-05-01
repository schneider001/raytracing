#include "Parallelepiped.h"
#include "iostream"


Parallelepiped Parallelepiped::operator = (const Parallelepiped& that) {
	coords0_ = that.coords0_;
	i_ = that.i_;
	j_ = that.j_;
	k_ = that.k_;
	l1_ = that.l1_;
	l2_ = that.l2_;
	l3_ = that.l3_;
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


vector Parallelepiped::normal(const vector& coords) {
	return closest_rectan.normal_;
}


vector Parallelepiped::intersect_ray(const vector& coords, vector& dir) {
	vector new_coords_center_shell = 0.5 * ((coords2_ + (-1) * coords0_) + (coords1_ + (-1) * coords0_) + (coords0_ + l3_ * k_ + (-1) * coords0_));
	double rad_shell = new_coords_center_shell.length();
	//std::cout << rad_shell << "\n";

	Sphere shell(coords0_ + new_coords_center_shell, rad_shell, Color(0, 0, 0), 0, 0, 0, 0, 1);
	vector ray_crosses_shell = shell.intersect_ray(coords, dir);

	if (ray_crosses_shell == vector(DBL_MAX, DBL_MAX))
		return  ray_crosses_shell;

	double tmin = 1;
	double closest_t = DBL_MAX;

	for (int i = 0; i < 6; i++) {
		vector res = facets_[i].intersect_ray(coords, dir);

		if (res.x_ > tmin && res.x_ < closest_t) {
			closest_t = res.x_;
			closest_rectan = facets_[i];
		}
	}

	return vector(closest_t, 0, 0);
}