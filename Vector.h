#pragma once
#include <cmath>

template<typename T>
class Vector {
public:
	T x_;
	T y_;
	T z_;
	
	double length();
	Vector& normalization();

	Vector& operator = (const Vector& that);
	bool operator == (const Vector& that);

	friend Vector operator + (const Vector& a, const Vector& b);
	friend Vector operator * (double k, const Vector& a);
	friend double operator * (const Vector& a, const Vector& b);

	Vector(T x = 0, T y = 0, T z = 0):
		x_(x),
		y_(y),
		z_(z)
	{}

	Vector(const Vector& that):
		x_(that.x_),
		y_(that.y_),
		z_(that.z_)
	{}
};

