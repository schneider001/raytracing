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
Color trace_ray(vector O, vector D, Sphere* arr_of_sph);

void twoD() {
	vector coords0(0, 0);
	vector coords1(900, 550);
	CoordsSys canvas(coords0, coords1, 1, 1);
	canvas.create_window();
	canvas.draw_window();
	canvas.draw_circle(vector(70, 100, 0), 30, Color(255, 0, 0));
	canvas.draw_circle(vector(40, 60, 0), 7, Color(0, 0, 255));
	canvas.draw_circle(vector(200, -200, 0), 100, Color(0, 255, 255));
	canvas.draw_axis();
	vector coords_of_camera(0, 0, 0);

	int x_of_view_port = 20;

	Sphere arr_of_sph[] = { Sphere(vector(70, 100, 0), 30, Color(255, 0, 0)), 
							Sphere(vector(40, 60, 0), 7, Color(0, 0, 255)), 
							Sphere(vector(200, -200, 0), 100, Color(0, 255, 255)) };

	for (int y = -275; y <= 275; y++) {
		vector dir(x_of_view_port, y, 0);
		Color color = trace_ray(coords_of_camera, dir, arr_of_sph);
		canvas.draw_pixel(vector(x_of_view_port, y), color);
	}

}

void threeD() {
	vector coords0(0, 0);
	vector coords1(900, 550);
	CoordsSys canvas(coords0, coords1, 1, 1);
	canvas.create_window();
	canvas.draw_window();
	vector coords_of_camera(0, 0, 0);

	int z_of_view_port = 20;

	Sphere arr_of_sph[] = { Sphere(vector(0, 100, 70), 30, Color(255, 0, 0)),
							Sphere(vector(0, 60, 40), 7, Color(0, 0, 255)),
							Sphere(vector(0, -200, 200), 100, Color(0, 255, 255)) };

	for (int x = -450; x < 450; x++) {
		for (int y = -275; y < 275; y++) {
			vector dir(x, y, z_of_view_port);
			Color color = trace_ray(coords_of_camera, dir, arr_of_sph);
			canvas.draw_pixel(vector(x, y), color);
		}
	}
}


int main() {
	threeD();	
}


vector intersect_ray_sphere(const vector& coords_of_camera, const vector& dir, const Sphere& sphere) {

	vector OC = coords_of_camera + (-1) * sphere.center_;

	double a = dir * dir;
	double b = 2 * OC * dir;
	double c = OC * OC - sphere.radius_ * sphere.radius_;

	double discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return vector(inf, inf);
	}

	double t1 = (-b + sqrt(discriminant)) / (2 * a);
	double t2 = (-b - sqrt(discriminant)) / (2 * a);
	return vector(t1, t2);
}


Color trace_ray(vector O, vector D, Sphere* arr_of_sph) {
	double closest_t = inf;
	Sphere null_sphere(vector(0, 0, 0), 0, Color(0, 0, 0));
	Sphere closest_sphere(null_sphere);

	for (int i = 0; i < 3; i++) {

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