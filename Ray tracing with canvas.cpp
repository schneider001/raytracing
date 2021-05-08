#define _CRT_SECURE_NO_WARNINGS

#include "Vector.h"
#include "Window.h"
#include "Matrix.h"
#include "Camera.h"
#include "Light.h"

#include "Object.h"
#include "Sphere.h"
#include "Plane.h"
#include "Rectan.h"
#include "Parallelepiped.h"

#include <omp.h>
#include <cfloat>


using Color = Vector<double>;
using vector = Vector<double>;



void draw_scene(Object* arr_of_obj[], Light* arr_of_lights, Window window, const vector& coords0_scene, const vector& coords1_scene);
Color trace_ray(const vector& coords, vector dir, Object* arr_of_obj[], Light* arr_of_lights, const int& recursion_depth, const int& is_light_in_obj, const double& tmin, const double& tmax);
bool existence_of_shadow(const vector& intersection, vector& dir, Object* arr_of_obj[], const double& tmin, const double& tmax);
vector reflect_ray(const vector& ray, const vector& normal);
vector refract_ray(const vector& ray, const vector& normal, double n1, double n2);
double compute_lighting(const vector& intersection, vector& normal, vector& overview, Object* arr_of_obj[], Light* arr_of_lights, const int& specular);



int main() {
	vector coords0_scene(0, 0);
	vector coords1_scene(700, 450);
	vector coords0_menu(coords1_scene.x_, 0);
	vector coords1_menu(1000, 450);

	double width_of_page = coords1_menu.x_ - coords0_menu.x_ - 30;
	double high_of_page = coords1_menu.y_ - coords0_menu.y_ - 70;

	vector coords0_window = coords0_scene;
	vector coords1_window = coords1_menu;

	Window window(coords0_window, coords1_window, coords0_scene, coords1_scene, 1, 1, width_of_page, high_of_page);
	window.create_window();
	window.draw_window();
	 

	Object* arr_of_obj[] = { new Sphere(vector( 200,    0, 900),   75,  Color(255,   0,   0), 500, 0.3, 0.3, 1,   sphere),
							 new Sphere(vector(-200,    0, 900),   75,  Color(  0,   0, 255), 500, 0.3, 0.3, 1,   sphere),
							 new Sphere(vector(   0, -100, 900),  100,  Color(  0, 255, 255), 500, 0.5, 0.3, 1,   sphere),
							 new Sphere(vector(	  0,  -50, 400),   80,  Color(255, 255, 255), 500, 0.1, 0.9, 1.4, sphere),
							 new Sphere(vector(   0, -180, 800),   20,  Color(255,   0, 255), 500, 0.3, 0.3, 1,   sphere),
							 new Plane (vector(   0,    1,   0),  200,  Color(255, 255,   0), 500, 0.3, 0,   1,    plane),
							 new Rectan(vector( -50, -200, 730), vector(0, 1, 1), vector(1, 0, 0), 100, 100,
										Color ( 255,  255, 255), 300, 0.1, 0.5, 1, rectan),
							 new Parallelepiped(vector(-12.5, -75, 700), vector(0, 1, 1), vector(1, 0, 0), vector(0, 1, -1),
												25, 25, 25, Color(255,  165,   0), 0, 0.1, 0.5, 1, parallelepiped),
							 new End() };


	Light arr_of_lights[] = { Light(ambient, 0.2),
							  Light(point, 0.3, vector( 200, 200, 900)),
							  Light(point, 0.3, vector(-200, 200, 900)),
							  Light(directional, 0.1, vector( 1, 1,  1)),
							  Light(directional, 0.1, vector( 0, 0, -1)),
							  Light(directional, 0.1, vector(-1, 1,  1)),
							  Light(null, 0) };


	Canvas canvas(coords0_menu, coords1_menu, coords0_menu, coords1_menu, Color(84, 94, 140), Color(84, 94, 140));
	canvas.draw_canvas();


	ManagerButton manager_of_buttons;
	

	const int number_of_pages = 50;
	HDC pages[number_of_pages] = {};
	add_poles(pages, arr_of_obj, &manager_of_buttons, coords0_menu, coords1_menu, width_of_page, high_of_page);



	while (!GetAsyncKeyState(VK_ESCAPE)) {
		draw_scene(arr_of_obj, arr_of_lights, window, coords0_scene, coords1_scene);
		manager_of_buttons.run();
		window.draw_page(&pages[0]);
	}

	for (int i = 0; i < number_of_pages; i++)
		txDeleteDC(pages[i]);
}



void draw_scene(Object* arr_of_obj[], Light* arr_of_lights, Window window, const vector& coords0_scene, const vector& coords1_scene) {
	const int num_threads = omp_get_max_threads();

#pragma omp parallel num_threads(num_threads) 
	{

		int thread_num = omp_get_thread_num();

		double start_x = ((coords1_scene.x_ - coords0_scene.x_) / num_threads) * thread_num - (coords1_scene.x_ - coords0_scene.x_) / 2;
		double end_x = ((coords1_scene.x_ - coords0_scene.x_) / num_threads) * (1. + thread_num) - (coords1_scene.x_ - coords0_scene.x_) / 2;
		double start_y = -coords1_scene.y_ / 2;
		double end_y = coords1_scene.y_ / 2;

		static Camera camera(vector(0, 0, -500));

		double z_of_view_port = 1100;

		Matrix turn = camera.rotation(0.0007);
		camera.shift(0.5);

		for (double x = start_x; x < end_x; x ++) {
			for (double y = start_y; y < end_y; y ++) {

				vector dir(x, y, z_of_view_port);
				vector changed_dir = vector_mul_matrix(dir, turn);

				double tmin = 1;
				double tmax = DBL_MAX;
				int reflection_depth = 2;
				int is_light_in_obj = 0;

				Color color = trace_ray(camera.coords_, changed_dir, arr_of_obj, arr_of_lights, reflection_depth, is_light_in_obj, tmin, tmax);

				window.draw_pixel(vector(x, y), color);
				//window.draw_pixel(vector(x, y + 1), color);
				//window.draw_pixel(vector(x + 1, y), color);
				//window.draw_pixel(vector(x + 1, y + 1), color);
			}
		}
	}
}



Color trace_ray(const vector& coords, vector dir, Object* arr_of_obj[], Light* arr_of_lights, const int& recursion_depth, const int& is_light_in_obj, const double& tmin, const double& tmax) {

	Object null_obj(Color(0, 0, 0), 0, 0, 0, 0, 0);
	vector null_vector(0, 0, 0);

	double closest_t = DBL_MAX;

	Object* closest_obj = &null_obj;


	vector overview = (-1) * dir;
	vector direction = dir;
	dir.normalization();

	int i = 0;
	for (i; arr_of_obj[i]->type_ != plane; i++) {

		vector res = arr_of_obj[i]->intersect_ray(coords, dir);

		if (res.x_ > tmin && res.x_ < tmax && res.x_ < closest_t) {
			closest_t = res.x_;
			closest_obj = arr_of_obj[i];
		}
		if (res.y_ > tmin && res.y_ < tmax && res.y_ < closest_t) {
			closest_t = res.y_;
			closest_obj = arr_of_obj[i];
		}
	}

	int is_light_in_obj_ = is_light_in_obj;

	if (closest_t < DBL_MAX)
		is_light_in_obj_ += 1;


	for (i; arr_of_obj[i]->type_ != end; i++) {

		vector res = arr_of_obj[i]->intersect_ray(coords, dir);

		if (res.x_ > tmin && res.x_ < tmax && res.x_ < closest_t) {
			closest_t = res.x_;
			closest_obj = arr_of_obj[i];
		}
	}

	if (closest_t == DBL_MAX) 
		return Color(119, 97, 127);


	vector intersection = coords + closest_t * dir;
	vector perpendicular = closest_obj->normal(intersection);
	Color color = closest_obj->color_;
	double specular = closest_obj->specular_;
	double reflective = closest_obj->reflective_;
	double refractive = closest_obj->refractive_;
	Color reflected_color(0, 0, 0);
	Color refracted_color(0, 0, 0);

	double n = closest_obj->n_;


	Color local_color =  compute_lighting(intersection, perpendicular, overview, arr_of_obj, arr_of_lights, specular) * color;
	if (recursion_depth <= 0 || (reflective <= 0 && refractive <= 0))
		return local_color;


	if (reflective > 0) {
		vector reflection = reflect_ray(overview, perpendicular);
		reflected_color = trace_ray(intersection, reflection, arr_of_obj, arr_of_lights, recursion_depth - 1, is_light_in_obj, 0.0001, DBL_MAX);
	}

	if (refractive > 0)  {
		vector new_dir = dir;
		if (is_light_in_obj % 2 == 0) {
			new_dir = refract_ray(dir, perpendicular, 1, n);
		}
		else {
			new_dir = refract_ray(dir, (-1) * perpendicular, n, 1);
		}

		refracted_color = trace_ray(intersection, new_dir, arr_of_obj, arr_of_lights, recursion_depth, is_light_in_obj_, 0.0001, DBL_MAX);

	}

	return (1. - reflective - refractive) * local_color + reflective * reflected_color + refractive * refracted_color;
}



bool existence_of_shadow(const vector& intersection, vector& dir, Object* arr_of_obj[], const double& tmin, const double& tmax) {

	dir.normalization();

	int i = 0;
	for (i; arr_of_obj[i]->type_ != plane; i++) {

		vector res = arr_of_obj[i]->intersect_ray(intersection, dir);
		
		if (res.x_ > tmin && res.x_ < tmax)
			return true;

		if (res.y_ > tmin && res.y_ < tmax) 
			return true;
	}

	for (i; arr_of_obj[i]->type_ != end; i++) {

		vector res = arr_of_obj[i]->intersect_ray(intersection, dir);

		if (res.x_ > tmin && res.x_ < tmax)
			return true;
	}

	return false;
}



vector reflect_ray(const vector& ray, const vector& normal) {
	return 2 * ray * normal * normal + (-1) * ray;
}



vector refract_ray(const vector& ray, const vector& normal, double n1, double n2) {

	double cosa1 = (-1) * ray * normal;
	double sina1 = 1;

	if (cosa1 * cosa1 < 1)
		sina1 = sqrt(1 - cosa1 * cosa1);

	double sina2 = (n1 / n2) * sina1;

	vector refracted = (sina2 / sina1) * (normal + ray) + (-1) * normal;

	return refracted;
}



double compute_lighting(const vector& intersection, vector& normal, vector& overview, Object* arr_of_obj[], Light* arr_of_lights, const int& specular) {

	double intensity = 0;
	vector l_dir;


	for (int i = 0; arr_of_lights[i].type_ != null; i++) {

		if (arr_of_lights[i].type_ == ambient) {
			intensity += arr_of_lights[i].intensity_;
		}
		else if (arr_of_lights[i].type_ == point){
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
