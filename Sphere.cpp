#include "Sphere.h"


Sphere&  Sphere:: operator = (const Sphere& that) {
		center_ = that.center_;
		radius_ = that.radius_;
		color_ = that.color_;
		specular_ = that.specular_;
		reflective_ = that.reflective_;
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


Sphere::Sphere(const Sphere& that) {
		center_ = that.center_;
		radius_ = that.radius_;
		color_ = that.color_;
		specular_ = that.specular_;
		reflective_ = that.reflective_;
		rad_quad = that.rad_quad;
	}

Sphere::Sphere(vector center, double radius, Color color, int specular, double reflective) {
		center_ = center;
		radius_ = radius;
		color_ = color;
		specular_ = specular;
		reflective_ = reflective;
		rad_quad = radius_ * radius_;
	}

