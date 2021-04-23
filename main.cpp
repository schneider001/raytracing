#include "Vector.h"
#include "Sphere.h"
#include "CoordsSys.h"
#include "Matrix.h"
#include "Camera.h"
#include "Light.h"
#include "Plane.h"
#include "Object.h"

#include <iostream>
#include <omp.h>
#include <cfloat>


using Color = Vector<double>;
using vector = Vector<double>;


Color trace_ray(const vector& coords, vector& dir, Object* arr_of_obj[], Light* arr_of_lights, const int& recursion_depth, const double& tmin, const double& tmax);
bool existence_of_shadow(const vector& intersection, vector& dir, Object* arr_of_obj[], const double& tmin, const double& tmax);
vector reflect_ray(const vector& ray, const vector& normal);
double compute_lighting(const vector& intersection, vector& normal, vector& overview, Object* arr_of_obj[], Light* arr_of_lights, const int& specular);


enum { sphere = 1, plane = 2, end = 0 };


int main() {
	vector coords0(0, 0);
	vector coords1(800, 500);
	CoordsSys canvas(coords0, coords1, 1, 1);
	canvas.create_window();
	canvas.draw_window();

	Camera camera(vector(0, 0, 350));

	double z_of_view_port = 1100;
	 
	Object* arr_of_obj[] = { new Sphere(vector( 200,     0,  900),   75,  Color(255,   0,   0), 500, 0.2),
							 new Sphere(vector(-200,     0,  900),   75,  Color(  0,   0, 255), 500, 0.2),
							 new Sphere(vector(   0,  -100,  900),  100,  Color(  0, 255, 255), 500, 0.2),
							 new Plane (vector(0, 1, 0), 200, Color(255, 255, 0), 500, 0.5),
							 new End()};

	Light arr_of_lights[] = { 
							  Light("ambient", 0.2),
							  Light("point", 0.3, vector( 200, 200, 900)),
							  Light("point", 0.3, vector(-200, 200, 900)),
							  Light("directional", 0.1, vector(1, 1,  1)),
							  Light("directional", 0.1, vector(-1, 1, 1)),
							  Light("null", 0) };



	while (!GetAsyncKeyState(VK_ESCAPE)) {

		const int num_threads = omp_get_max_threads();

		#pragma omp parallel num_threads(num_threads) 
		{
			int thread_num = omp_get_thread_num();

			double start_x = (coords1.x_ / num_threads) * thread_num - coords1.x_ / 2;
			double end_x = (coords1.x_ / num_threads) * (1. + thread_num) - coords1.x_ / 2;
			double start_y = -coords1.y_ / 2;
			double end_y = coords1.y_ / 2;

			Matrix turn = camera.rotation(0.0003);
			char but = camera.shift_check();

			for (double x = start_x; x < end_x; x+=2) {
				for (double y = start_y; y < end_y; y+=2) {
					vector dir(x, y, z_of_view_port);
					vector changed_dir = vector_mul_matrix(dir, turn);
					camera.shift(but, 0.0001);
					Color color = trace_ray(camera.coords_, changed_dir, arr_of_obj, arr_of_lights, 2, 1, DBL_MAX);
					canvas.draw_pixel(vector(x, y), color);
					canvas.draw_pixel(vector(x, y + 1), color);
					canvas.draw_pixel(vector(x + 1, y), color);
					canvas.draw_pixel(vector(x + 1, y + 1), color);
				}
			}
		}
	}
}



Color trace_ray(const vector& coords, vector& dir, Object* arr_of_obj[], Light* arr_of_lights, const int& recursion_depth, const double& tmin, const double& tmax) {

	Sphere null_sphere(vector(0, 0, 0), 0, Color(0, 0, 0), 0, 0);
	Plane null_plane(vector(1, 1, 1), 0, Color(0, 0, 0), 0, 0);

	double closest_t = DBL_MAX;
	int type;
	
	double closest_t_sph = DBL_MAX;
	Sphere* closest_sphere = &null_sphere;


	double closest_t_pln = DBL_MAX;
	Plane* closest_plane = &null_plane;


	vector overview = (-1) * dir;
	dir.normalization();


	for (int i = 0; (*arr_of_obj[i]).type_ != plane; i++) {

		vector res = (*arr_of_obj[i]).intersect_ray(coords, dir);

		if (res.x_ > tmin && res.x_ < tmax && res.x_ < closest_t_sph) {
			closest_t_sph = res.x_;
			closest_sphere = dynamic_cast <Sphere*>(arr_of_obj[i]);
		}
		if (res.y_ > tmin && res.y_ < tmax && res.y_ < closest_t_sph) {
			closest_t_sph = res.y_;
			closest_sphere = dynamic_cast <Sphere*>(arr_of_obj[i]);
		}
	}


	for (int i = 0; (*arr_of_obj[i]).type_ != end; i++) {

		vector res = (*arr_of_obj[i]).intersect_ray(coords, dir);

		if (res.x_ > tmin && res.x_ < tmax && res.x_ < closest_t_pln) {
			closest_t_pln = res.x_;
			closest_plane = dynamic_cast <Plane*>(arr_of_obj[i]);
		}
	}


	if (closest_t_pln >= closest_t_sph) {
		closest_t = closest_t_sph;
		type = sphere;
	}
	else {
		closest_t = closest_t_pln;
		type = plane;
	}

	if (closest_t == DBL_MAX) {
		return Color(119, 97, 127);
	}

	vector intersection = coords + closest_t * dir;
	vector perpendicular;
	Color color;
	int specular;
	double reflective;

	if (type == sphere) {
		perpendicular = intersection + (-1) * (*closest_sphere).center_;
		perpendicular.normalization();

		specular = (*closest_sphere).specular_;
		color = (*closest_sphere).color_;
		reflective = (*closest_sphere).reflective_;
	}

	if(type == plane) {
		if (closest_plane != nullptr)
			perpendicular = (*closest_plane).normal_;
		else 
			return Color(119, 97, 127);

		specular = (*closest_plane).specular_;
		color = (*closest_plane).color_;
		reflective = (*closest_plane).reflective_;
	}

	Color local_color =  compute_lighting(intersection, perpendicular, overview, arr_of_obj, arr_of_lights, specular) * color;
	if (recursion_depth <= 0 || reflective <= 0)
		return local_color;


	vector reflection = reflect_ray(overview, perpendicular);
	Color reflected_color = trace_ray(intersection, reflection, arr_of_obj, arr_of_lights, recursion_depth - 1, 0.0001, DBL_MAX);
	return (1. - reflective) * local_color + reflective * reflected_color;
}


bool existence_of_shadow(const vector& intersection, vector& dir, Object* arr_of_obj[], const double& tmin, const double& tmax) {

	dir.normalization();

	for (int i = 0; (*arr_of_obj[i]).type_ != plane; i++) {

		vector res = (*arr_of_obj[i]).intersect_ray(intersection, dir);
		
		if (res.x_ > tmin && res.x_ < tmax)
			return true;

		if (res.y_ > tmin && res.y_ < tmax) 
			return true;
	}

	for (int i = 0; (*arr_of_obj[i]).type_ != end; i++) {

		vector res = (*arr_of_obj[i]).intersect_ray(intersection, dir);

		if (res.x_ > tmin && res.x_ < tmax)
			return true;
	}

	return false;
}


vector reflect_ray(const vector& ray, const vector& normal) {
	return 2 * ray * normal * normal + (-1) * ray;
}


double compute_lighting(const vector& intersection, vector& normal, vector& overview, Object* arr_of_obj[], Light* arr_of_lights, const int& specular) {

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
			if (existence_of_shadow(intersection, l_dir, arr_of_obj, 0.0001, DBL_MAX)) {
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
