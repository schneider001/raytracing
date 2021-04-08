#include "Vector.h"
#include "CoordsSys.h"
#include "C:\TXlib\TX\TXLib.h"
#include <iostream>
#include <cfloat>

double inf = DBL_MAX;
using Color = Vector<double>;
using vector = Vector<double>;

struct Sphere {
	vector center_;
	double radius_;
	Color color_;

	Sphere& operator = (const Sphere& that) {
		center_ = that.center_;
		radius_ = that.radius_;
		color_  = that.color_;
		return *this;
	}

	bool operator == (const Sphere& that) {
		if (this->radius_ == that.radius_ && this->center_ == that.center_ && this->color_ == that.color_)
			return true;
		else
			return false;
	}


	Sphere(const Sphere& that) {
		center_ = that.center_;
		radius_ = that.radius_;
		color_ = that.color_;
	}

	Sphere(vector center, double radius, Color color) {
		center_ = center;
		radius_ = radius;
		color_  = color;
	}
};


vector intersect_ray_sphere(const vector& O, const vector& D, const Sphere& S);
Color TraceRay(vector O, vector D, Sphere* arr_of_sph);


int main() {

	vector coords0(0, 0);
	vector coords1(900, 550);
	CoordsSys canvas(coords0, coords1, 1, 1);
	canvas.create_window();
	txBegin();
	vector O(0, 0, 0);

	int d = 50;
	Sphere arr_of_sph[] = { Sphere(vector(-200, 100, 20), 50, Color(0, 255, 0))  };
	/*
	vector D(0, 10, d);
	Color color = TraceRay(O, D, arr_of_sph);
	printf("%f %f %f", color.x_, color.y_, color.z_);*/
	
	for (int x = -450; x < 450; x++) {
		for (int y = -275; y < 275; y++) {
			vector D(x, y, d);
			Color color = TraceRay(O, D, arr_of_sph);
			canvas.draw_pixel(vector(x, y), color);
		}
	}
	txSleep(1000);
}


vector intersect_ray_sphere(const vector& O, const vector& D, const Sphere& S) {
	vector C = S.center_;
	double r = S.radius_;
	vector OC = O + (-1) * C;

	double k1 = D * D;
	double k2 = 2 * OC * D;
	double k3 = OC * OC - r * r;

	double discriminant = k2 * k2 - 4 * k1 * k3;
	if (discriminant < 0) {
		return vector(inf, inf);
	}

	double t1 = (-k2 + sqrt(discriminant)) / (2 * k1);
	double t2 = (-k2 - sqrt(discriminant)) / (2 * k1);
	return vector(t1, t2);
}


Color TraceRay(vector O, vector D, Sphere* arr_of_sph) {
	double closest_t = inf;
	Sphere null_sphere(vector(0,0,0), 0, Color(0, 0, 0));
	Sphere closest_sphere(null_sphere);

	for (int i = 0; i < 1; i++) {

		vector res = intersect_ray_sphere(O, D, arr_of_sph[i]);

		if (res.x_ > 1 && res.x_ < inf && res.x_ < closest_t) {
			closest_t = res.x_;
			closest_sphere = arr_of_sph[i];
		}
		if (res.y_ > 1 && res.y_ < inf && res.y_ < closest_t) {
			closest_t = res.y_;
			closest_sphere = arr_of_sph[i];
		}
	}

	if (closest_sphere == null_sphere) {
		return Color(255, 255, 255);
	}

	return closest_sphere.color_;
}