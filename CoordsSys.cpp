#include "CoordsSys.h"

Vector<double> CoordsSys::to_pixels(const Vector<double>& coords) {
	Vector<double> start_of_coord(coords1_.x_ / 2, coords1_.y_ / 2);
	Vector<double> rec_coord(coords.x_ * scaleX_ + start_of_coord.x_, start_of_coord.y_ - coords.y_ * scaleY_);
	return rec_coord;
}


void CoordsSys::draw_pixel(const Vector<double>& coords, const Vector<double>& color_of_point) {
	Vector<double> rec_coord = to_pixels(coords);
	txSetPixel(rec_coord.x_, rec_coord.y_, RGB(color_of_point.x_, color_of_point.y_, color_of_point.z_));
}


void CoordsSys::create_window() {
	txCreateWindow(coords1_.x_, coords1_.y_);
}


void CoordsSys::draw_window() {
	txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_);
}


void CoordsSys::set_color_back(const Vector<double>& background) {
	txSetFillColor(RGB(background.x_, background.y_, background.z_));
}

