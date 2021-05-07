#include "Camera.h"


Matrix Camera::turn_x(double angle) {
	return Matrix(vector(1, 0, 0), vector(0, cos(angle), -sin(angle)), vector(0, sin(angle), cos(angle)));
}


Matrix Camera::turn_y(double angle) {
	return Matrix(vector(cos(angle), 0, sin(angle)), vector(0, 1, 0), vector(-sin(angle), 0, cos(angle)));
}


Matrix Camera::turn_z(double angle) {
	return Matrix(vector(cos(angle), -sin(angle), 0), vector(sin(angle), cos(angle), 0), vector(0, 0, 1));
}


Matrix Camera::identity() {
	return Matrix(vector(1, 0, 0), vector(0, 1, 0), vector(0, 0, 1));
}

Matrix Camera::rotation(double angle) {

	static Matrix temp = identity();

	if (GetAsyncKeyState(VK_LEFT)) {
		temp =  matrix_mul_matrix(temp,turn_y(-angle));
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		temp = matrix_mul_matrix(temp, turn_y(angle));
	}
	if (GetAsyncKeyState(VK_UP)) {
		temp = matrix_mul_matrix(temp, turn_x(-angle));
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		temp = matrix_mul_matrix(temp, turn_x(angle));
	}
	if (GetAsyncKeyState('O')) {
		temp = matrix_mul_matrix(temp, turn_z(-angle));
	}
	if (GetAsyncKeyState('P')) {
		temp = matrix_mul_matrix(temp, turn_z(angle));
	}
	return temp;
}

void Camera::shift(double step) {
	if (GetAsyncKeyState(VK_SPACE)) {
		coords_.y_ += step;
	}
	if (GetAsyncKeyState(VK_SHIFT)) {
		coords_.y_ -= step;
	}
	if (GetAsyncKeyState('W')) {
		coords_.z_ += step;
	}
	if (GetAsyncKeyState('S')) {
		coords_.z_ -= step;
	}
	if (GetAsyncKeyState('D')) {
		coords_.x_ += step;
	}
	if (GetAsyncKeyState('A')) {
		coords_.x_ -= step;
	}
}

/*void Camera::shift(char but, double step) {
	if (but == 's') {
		coords_.z_ += step;
	}
	if (but == 'b') {
		coords_.z_ -= step;
	}
	if (but == 'r') {
		coords_.x_ += step;
	}
	if (but == 'l') {
		coords_.x_ -= step;
	}
	if (but == 'u') {
		coords_.y_ += step;
	}
	if (but == 'd') {
		coords_.y_ -= step;
	}
}
*/




