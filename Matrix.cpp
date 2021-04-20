#include "Matrix.h"


using vector = Vector<double>;


Matrix& Matrix::operator = (const Matrix& that) {
	this->table.x_ = that.table.x_;
	this->table.y_ = that.table.y_;
	this->table.z_ = that.table.z_;
	return *this;
}


vector vector_mul_matrix(const vector& vec, const Matrix& mat) {
	return vector(vec.x_ * mat.table.x_.x_ + vec.y_ * mat.table.x_.y_ + vec.z_ * mat.table.x_.z_,
				  vec.x_ * mat.table.y_.x_ + vec.y_ * mat.table.y_.y_ + vec.z_ * mat.table.y_.z_,
				  vec.x_ * mat.table.z_.x_ + vec.y_ * mat.table.z_.y_ + vec.z_ * mat.table.z_.z_);
}


Matrix matrix_mul_matrix(Matrix m1, Matrix m2) {
	return Matrix(vector(m1.table.x_.x_ * m2.table.x_.x_ + m1.table.x_.y_ * m2.table.y_.x_ + m1.table.x_.z_ * m2.table.z_.x_,
						 m1.table.x_.x_ * m2.table.x_.y_ + m1.table.x_.y_ * m2.table.y_.y_ + m1.table.x_.z_ * m2.table.z_.y_,
						 m1.table.x_.x_ * m2.table.x_.z_ + m1.table.x_.y_ * m2.table.y_.z_ + m1.table.x_.z_ * m2.table.z_.z_),
				  vector(m1.table.y_.x_ * m2.table.x_.x_ + m1.table.y_.y_ * m2.table.y_.x_ + m1.table.y_.z_ * m2.table.z_.x_,
						 m1.table.y_.x_ * m2.table.x_.y_ + m1.table.y_.y_ * m2.table.y_.y_ + m1.table.y_.z_ * m2.table.z_.y_,
						 m1.table.y_.x_ * m2.table.x_.z_ + m1.table.y_.y_ * m2.table.y_.z_ + m1.table.y_.z_ * m2.table.z_.z_),
				  vector(m1.table.z_.x_ * m2.table.x_.x_ + m1.table.z_.y_ * m2.table.y_.x_ + m1.table.z_.z_ * m2.table.z_.x_,
						 m1.table.z_.x_ * m2.table.x_.y_ + m1.table.z_.y_ * m2.table.y_.y_ + m1.table.z_.z_ * m2.table.z_.y_,
						 m1.table.z_.x_ * m2.table.x_.z_ + m1.table.z_.y_ * m2.table.y_.z_ + m1.table.z_.z_ * m2.table.z_.z_));
}