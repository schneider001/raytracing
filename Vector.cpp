#include "Vector.h"


Vector<double>& Vector<double>::operator = (const Vector<double>& that) {
	this->x_ = that.x_;
	this->y_ = that.y_;
	this->z_ = that.z_;
	return *this;
}

bool Vector<double>::operator == (const Vector<double>& that) {
	if (this->x_ == that.x_ && this->y_ == that.y_ && this->z_ == that.z_)
		return true;
	else
		return false;
}

double Vector<double>::length() {
	return sqrt(x_ * x_ + y_ * y_ + z_* z_);
}


Vector<double>& Vector<double>::normalization() {
	Vector temp_vector = 1 / (*this).length() * (*this);
	x_ = temp_vector.x_;
	y_ = temp_vector.y_;
	z_ = temp_vector.z_;
	return (*this);
}


Vector<double> operator + (const Vector<double>& a, const Vector<double>& b) {
	Vector<double> result(a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_);
	return result;
}


Vector<double> operator * (double k, const Vector<double>& a) {
	Vector<double> result(k * a.x_, k * a.y_, k * a.z_);
	return result;
}


double operator * (const Vector<double>& a, const Vector<double>& b) {
	return (a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_);
}

