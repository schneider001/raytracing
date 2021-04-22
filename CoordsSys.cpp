#include "CoordsSys.h"


Vector<double> CoordsSys::to_pixels(const Vector<double>& coords) {
	Vector<double> start_of_coord(coords1_.x_ / 2, coords1_.y_ / 2);
	Vector<double> rec_coord(coords.x_ * scaleX_ + start_of_coord.x_, start_of_coord.y_ - coords.y_ * scaleY_);
	return rec_coord;
}


Vector<double> CoordsSys::extract_rgb(const Vector<double>& coords) {
	Vector<double> rec_coord = to_pixels(coords);
	Vector<double> color(txExtractColor(txGetPixel(rec_coord.x_, rec_coord.y_), TX_RED),
						 txExtractColor(txGetPixel(rec_coord.x_, rec_coord.y_), TX_GREEN),
						 txExtractColor(txGetPixel(rec_coord.x_, rec_coord.y_), TX_BLUE));
	return color;
}


void CoordsSys::draw_pixel(const Vector<double>& coords, const Vector<double>& color) {
	Vector<double> rec_coord = to_pixels(coords);
	RGBQUAD* pixel = &Video_memory[(int)rec_coord.x_ + ((int)(coords1_.y_ - coords0_.y_) - (int)rec_coord.y_) * (int)(coords1_.x_ - coords0_.x_)];
	pixel->rgbRed   = (BYTE)color.x_;
	pixel->rgbGreen = (BYTE)color.y_;
	pixel->rgbBlue  = (BYTE)color.z_;
}


void CoordsSys::draw_circle(const Vector<double>& coords, double r, const Vector<double>& color) {
	Vector<double> rec_coord = to_pixels(coords);
	txSetFillColor(RGB(color.x_, color.y_, color.z_));
	txCircle(rec_coord.x_, rec_coord.y_, r);
}


void CoordsSys::create_window() {
	txCreateWindow(coords1_.x_, coords1_.y_);
	Video_memory = txVideoMemory();
}


void CoordsSys::draw_window() {
	txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_);
}


void CoordsSys::set_color_back(const Vector<double>& background) {
	txSetFillColor(RGB(background.x_, background.y_, background.z_));
}

void CoordsSys::begin() {
	txBegin();
}

void CoordsSys::end() {
	txEnd();
}