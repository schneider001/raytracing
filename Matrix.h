#pragma once
#include "Vector.h"


using vector = Vector<double>;


class Matrix {
public:

	Vector<Vector<double>> table;

	friend vector vector_mul_matrix(const vector& vec, const Matrix& mat);

	friend Matrix matrix_mul_matrix(Matrix m1, Matrix m2);

	Matrix& operator = (const Matrix& that);

	Matrix(vector line_1, vector line_2, vector line_3) {
		table.x_ = line_1;
		table.y_ = line_2;
		table.z_ = line_3;
	}

	Matrix(const Matrix& that) {
		table.x_ = that.table.x_;
		table.y_ = that.table.y_;
		table.z_ = that.table.z_;
	}
};

