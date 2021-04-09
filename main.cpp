#include "Vector.h"
#include "Sphere.h"
#include "CoordsSys.h"
#include "C:\TXlib\TX\TXLib.h"
#include <iostream>
#include <cfloat>


double inf = DBL_MAX;
using Color = Vector<double>;
using vector = Vector<double>;


struct Light {
	const char* type_;
	double intensity_;
	vector position_;

	Light(const char* type, double intensity, vector position) {
		type_ = type;
		intensity_ = intensity;
		position_ = position;
	}

	Light(const char* type, double intensity) {
		type_ = type;
		intensity_ = intensity;
		position_ = vector(0, 0, 0);
	}
};


vector intersect_ray_sphere(const vector& coords_of_camera, vector  dir, const Sphere& sphere);
Color trace_ray(const vector& coords_of_camera, vector dir, Sphere* arr_of_sph, Light* arr_of_lights);
double compute_lighting(vector intersection, vector normal, Light* arr_of_lights);


int main() {
	vector coords0(0, 0);
	vector coords1(900, 550);
	CoordsSys canvas(coords0, coords1, 1, 1);
	canvas.create_window();
	canvas.draw_window();
	vector coords_of_camera(0, 0, 0);

	int z_of_view_port = 1100;

	Sphere arr_of_sph[] = { Sphere(vector(200, 100, 1200), 50,  Color(255, 0, 0)),
							Sphere(vector(-200, 60,  1300), 75,  Color(0, 0, 255)),
							Sphere(vector(0, 200, 1400), 100, Color(0, 255, 255)),
							Sphere(vector(0, -1300, 1200), 1200, Color(255, 255, 0)) };

	Light arr_of_lights[] = { Light("point", 0.6, vector(0, 600, 1300)), Light("point", 0.3, vector(0, -600, 1300)), Light("ambient", 0.1) };

	for (int x = -450; x < 450; x++) {
		for (int y = -275; y < 275; y++) {
			vector dir(x, y, z_of_view_port);
			Color color = trace_ray(coords_of_camera, dir, arr_of_sph, arr_of_lights);
			canvas.draw_pixel(vector(x, y), color);
		}
	}
}


vector intersect_ray_sphere(const vector& coords_of_camera, vector dir, const Sphere& sphere) {

	vector OC = coords_of_camera + (-1) * sphere.center_;
	dir.normalization();
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


Color trace_ray(const vector& coords_of_camera, vector dir, Sphere* arr_of_sph, Light* arr_of_lights) {
	double closest_t = inf;
	Sphere null_sphere(vector(0, 0, 0), 0, Color(0, 0, 0));
	Sphere closest_sphere(null_sphere);

	dir.normalization();

	for (int i = 0; i < 4; i++) {

		vector res = intersect_ray_sphere(coords_of_camera, dir, arr_of_sph[i]);

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
		return Color(119, 97, 127);
	}

	vector intersection = coords_of_camera + closest_t * dir;
	vector perpendicular = intersection + (-1) * closest_sphere.center_;
	perpendicular.normalization();

	return compute_lighting(intersection, perpendicular, arr_of_lights) * closest_sphere.color_;
}


double compute_lighting(vector intersection, vector normal, Light* arr_of_lights) {

	double intensity = 0;
	vector l_dir;

	for (int i = 0; i < 3; i++) {
		if (arr_of_lights[i].type_ == "ambient") {
			intensity += arr_of_lights[i].intensity_;
		}
		else {
			l_dir = arr_of_lights[i].position_ + (-1) * intersection;
		}

		double N_dot_L = normal * l_dir;

		if (N_dot_L > 0) {
			intensity += arr_of_lights[i].intensity_ * N_dot_L / (normal.length() * l_dir.length());
		}
	}
	return intensity;
}


/*
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
*/