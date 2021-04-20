#pragma once
#include "Vector.h"
#include "Matrix.h"
#include <cmath>
#include "C:\TXlib\TX\TXLib.h"

using vector = Vector<double>;


class Camera {	
public:

	vector coords_;

	Matrix turn_x(double angle);

	Matrix turn_y(double angle);

	Matrix turn_z(double angle);

	Matrix identity();

	Matrix rotation(double angle);

	char shift_check();

	void shift(char but, double step);

	Camera(vector coords) {
		coords_ = coords;
	}
};

