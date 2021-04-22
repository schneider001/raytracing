#pragma once
#include "Vector.h"


using vector = Vector<double>;


class Matrix {
public:

	Vector<vector> table;

	friend vector vector_mul_matrix(const vector& vec, const Matrix& mat);

	friend Matrix matrix_mul_matrix(Matrix m1, Matrix m2);

	Matrix& operator = (const Matrix& that);

	Matrix(vector line_1, vector line_2, vector line_3):
		table(line_1, line_2, line_3)
	{}

	Matrix(const Matrix& that) :
		table(that.table.x_, that.table.y_, that.table.z_)
	{}
};

