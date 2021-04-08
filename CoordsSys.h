#pragma once
#include "Vector.h"
#include "C:\TXlib\TX\TXLib.h"

class CoordsSys {
public:
	Vector<double> coords0_, coords1_;
	double scaleX_, scaleY_;


	Vector<double> to_pixels(const Vector<double>& coords);

	void draw_pixel(const Vector<double>& coords, const Vector<double>& color);

	void draw_circle(const Vector<double>& coords, double r, const Vector<double>& color);

	void draw_axis();

	void create_window();

	void draw_window();

	void set_color_back(const Vector<double>& background);

	CoordsSys(Vector<double> coords0, Vector<double> coords1, double scaleX, double scaleY) :
		coords0_(coords0),
		coords1_(coords1),
		scaleX_(scaleX),
		scaleY_(scaleY)
	{}
};
