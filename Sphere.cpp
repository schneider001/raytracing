#include "Sphere.h"


Sphere&  Sphere:: operator = (const Sphere& that) {
		center_ = that.center_;
		radius_ = that.radius_;
		color_ = that.color_;
		return *this;
	}

bool Sphere::operator == (const Sphere& that) {
		if (this->radius_ == that.radius_ && this->center_ == that.center_ && this->color_ == that.color_)
			return true;
		else
			return false;
	}


Sphere::Sphere(const Sphere& that) {
		center_ = that.center_;
		radius_ = that.radius_;
		color_ = that.color_;
	}

Sphere::Sphere(vector center, double radius, Color color) {
		center_ = center;
		radius_ = radius;
		color_ = color;
	}