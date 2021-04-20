#include "Vector.h"
#include "Sphere.h"
#include "CoordsSys.h"
#include "C:\TXlib\TX\TXLib.h"
#include <iostream>
#include <cfloat>
#include <omp.h>
#include "Matrix.h"
#include "Camera.h"


double inf = DBL_MAX;
using Color = Vector<double>;
using vector = Vector<double>;

Sphere null_sphere(vector(0, 0, 0), 0, Color(0, 0, 0), 0, 0);

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


vector intersect_ray_sphere(const vector& coords, vector& dir, const Sphere& sphere, double dir_dot_dir);
Color trace_ray(const vector& coords, vector& dir, Sphere* arr_of_sph, Light* arr_of_lights, const int& recursion_depth, const double& tmin, const double& tmax);
bool existence_of_shadow(const vector& intersection, vector& dir, Sphere* arr_of_sph, const double& tmin, const double& tmax);
vector reflect_ray(const vector& ray, const vector& normal);
double compute_lighting(const vector& intersection, vector& normal, vector& overview, Sphere* arr_of_sph, Light* arr_of_lights, const int& specular);


int main() {
	vector coords0(0, 0);
	vector coords1(800, 500);
	CoordsSys canvas(coords0, coords1, 1, 1);
	canvas.create_window();
	canvas.draw_window();

	Camera camera(vector(0, 0, 350));

	int z_of_view_port = 1100;
	 
	Sphere arr_of_sph[] = { Sphere(vector( 200,     0,  900),   75,  Color(255,   0,   0), 500, 0.3),
							Sphere(vector(-200,     0,  900),   75,  Color(  0,   0, 255), 500, 0.3),
							Sphere(vector(   0,  -100,  900),  100,  Color(  0, 255, 255), 500, 0.3),
							Sphere(vector(   0, -5100, 1300), 5000,  Color(255, 255,   0),  50, 0.3),
							Sphere(vector(   0,     0,    0),    0,  Color(  0,   0,   0),   0, 0) };

	Light arr_of_lights[] = { 
							  Light("ambient", 0.2),
							  Light("point", 0.3, vector( 200, 200, 900)),
							  Light("point", 0.3, vector(-200, 200, 900)),
							  //Light("directional", 0.2, vector(1, 1,  1)),
							  //Light("directional", 0.2, vector(-1, 1, 1)),
							  Light("null", 0) };


	/*while (!GetAsyncKeyState(VK_ESCAPE)) {
		vector dir(20, 20, z_of_view_port);
		Matrix turn = camera.change_position(txPI);
		vector changed_dir = vector_mul_matrix(dir, turn);
		printf("%f  %f  %f\n", changed_dir.x_, changed_dir.y_, changed_dir.z_);
	}*/

	while (!GetAsyncKeyState(VK_ESCAPE)) {

		const int num_threads = omp_get_max_threads();

		#pragma omp parallel num_threads(num_threads) 
		{	
			int thread_num = omp_get_thread_num();

			double start_x = (coords1.x_ / num_threads) * thread_num - coords1.x_ / 2;
			double end_x = (coords1.x_ / num_threads) * (1 + thread_num) - coords1.x_ / 2;
			double start_y = -coords1.y_ / 2;
			double end_y = coords1.y_ / 2;

			Matrix turn = camera.rotation(0.001);
			char but = camera.shift_check();
			

			for (double x = start_x; x < end_x; x++) {
				for (double y = start_y; y < end_y; y++) {
					vector dir(x, y, z_of_view_port);
					vector changed_dir = vector_mul_matrix(dir, turn);
					camera.shift(but, 0.0001);
					Color color = trace_ray(camera.coords_, changed_dir, arr_of_sph, arr_of_lights, 3, 1, inf);
					canvas.draw_pixel(vector(x, y), color);
					//canvas.draw_pixel(vector(x, y + 1), color);
					//canvas.draw_pixel(vector(x + 1, y), color);
					//canvas.draw_pixel(vector(x + 1, y + 1), color);
				}
			}
		}

	}
}


vector intersect_ray_sphere(const vector& coords, vector& dir, const Sphere& sphere, double dir_dot_dir) {

	vector OC = coords + (-1) * sphere.center_;
	double a = dir_dot_dir;
	double b = 2 * OC * dir;
	double c = OC * OC - sphere.rad_quad;

	double discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return vector(inf, inf);
	}

	double root_disc = sqrt(discriminant);
	double a_twice = 2 * a;

	double t1 = (-b + root_disc) / a_twice;
	double t2 = (-b - root_disc) / a_twice;
	return vector(t1, t2);
}


Color trace_ray(const vector& coords, vector& dir, Sphere* arr_of_sph, Light* arr_of_lights, const int& recursion_depth, const double& tmin, const double& tmax) {
	double closest_t = inf;
	Sphere closest_sphere(null_sphere);

	vector overview = (-1) * dir;
	dir.normalization();

	double dir_dot_dir = dir * dir;


	for (int i = 0; arr_of_sph[i].radius_ != 0; i++) {

		vector res = intersect_ray_sphere(coords, dir, arr_of_sph[i], dir_dot_dir);

		if (res.x_ > tmin && res.x_ < tmax && res.x_ < closest_t) {
			closest_t = res.x_;
			closest_sphere = arr_of_sph[i];
		}
		if (res.y_ > tmin && res.y_ < tmax && res.y_ < closest_t) {
			closest_t = res.y_;
			closest_sphere = arr_of_sph[i];
		}
	}


	if (closest_sphere == null_sphere) {
		return Color(119, 97, 127);
	}


	vector intersection = coords + closest_t * dir;
	vector perpendicular = intersection + (-1) * closest_sphere.center_;
	perpendicular.normalization();
	Color local_color =  compute_lighting(intersection, perpendicular, overview, arr_of_sph, arr_of_lights, closest_sphere.specular_) * closest_sphere.color_;
	if (recursion_depth <= 0 || closest_sphere.reflective_ <= 0)
		return local_color;


	vector reflection = reflect_ray(overview, perpendicular);
	Color reflected_color = trace_ray(intersection, reflection, arr_of_sph, arr_of_lights, recursion_depth - 1, 0.0001, inf);
	return (1. - closest_sphere.reflective_) * local_color + closest_sphere.reflective_ * reflected_color;
}


bool existence_of_shadow(const vector& intersection, vector& dir, Sphere* arr_of_sph, const double& tmin, const double& tmax) {

	dir.normalization();

	double dir_dot_dir = dir * dir;

	for (int i = 0; arr_of_sph[i].radius_ != 0; i++) {

		vector res = intersect_ray_sphere(intersection, dir, arr_of_sph[i], dir_dot_dir);
		
		if (res.x_ > tmin && res.x_ < tmax && res.x_) {
			return true;
		}
		if (res.y_ > tmin && res.y_ < tmax && res.y_) {
			return true;
		}
	}
	return false;
}


vector reflect_ray(const vector& ray, const vector& normal) {
	return 2 * ray * normal * normal + (-1) * ray;
}


double compute_lighting(const vector& intersection, vector& normal, vector& overview, Sphere* arr_of_sph, Light* arr_of_lights, const int& specular) {

	double intensity = 0;
	vector l_dir;


	for (int i = 0; arr_of_lights[i].type_ != "null"; i++) {

		if (arr_of_lights[i].type_ == "ambient") {
			intensity += arr_of_lights[i].intensity_;
		}
		else if (arr_of_lights[i].type_ == "point"){
			l_dir = arr_of_lights[i].position_ + (-1) * intersection;
		}
		else {
			l_dir = arr_of_lights[i].position_;
		}


		if (l_dir.length() != 0) {
			if (existence_of_shadow(intersection, l_dir, arr_of_sph, 0.0001, inf)) {
				continue;
			}
		}


		double N_dot_L = normal * l_dir;

		if (N_dot_L > 0) {
			intensity += arr_of_lights[i].intensity_ * N_dot_L / (normal.length() * l_dir.length());
		}
		
		if (specular != -1) {
			vector reflection = reflect_ray(l_dir, normal);
			double R_dot_O = reflection * overview;
			if (R_dot_O > 0) {
				double cos_n = pow(R_dot_O / (reflection.length() * overview.length()), specular);
				intensity += arr_of_lights[i].intensity_ * cos_n;
			}
		}
	}


	if (intensity > 1) {
		intensity = 1;
	}

	return intensity;
}
