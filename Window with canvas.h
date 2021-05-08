#pragma once

#include "Vector.h"
#include "Sphere.h"
#include "Plane.h"
#include "Rectan.h"
#include "Parallelepiped.h"

#include <iostream>
#include <limits>
#include <string>
#include "C:\TXlib\TX\TXLib.h"


using Color = Vector<double>;
using vector = Vector<double>;
typedef void (*func_t) (void);


enum { sphere = 1, plane = 2, rectan = 3, parallelepiped = 4, end = 0 };
enum { ambient = 1, point = 2, directional = 3, null = 0 };


class Window {
	vector coords0_menu;
	vector coords1_menu;
	double width_of_page_;
	double high_of_page_;
public:
	vector coords0_window_, coords1_window_, coords0_sys_, coords1_sys_;
	double scaleX_, scaleY_;
	RGBQUAD* Video_memory;


	vector to_pixels(const vector& coords) {
		vector start_of_coord(coords1_sys_.x_ / 2, coords1_sys_.y_ / 2);
		vector rec_coord(coords.x_ * scaleX_ + start_of_coord.x_, start_of_coord.y_ - coords.y_ * scaleY_);
		return rec_coord;
	}


	Color extract_rgb(const vector& coords) {
		vector rec_coord = to_pixels(coords);
		Color color(txExtractColor(txGetPixel(rec_coord.x_, rec_coord.y_), TX_RED),
					txExtractColor(txGetPixel(rec_coord.x_, rec_coord.y_), TX_GREEN),
					txExtractColor(txGetPixel(rec_coord.x_, rec_coord.y_), TX_BLUE));
		return color;
	}


	void draw_pixel(const vector& coords, const Color& color) {
		vector rec_coord = to_pixels(coords);
		RGBQUAD* pixel = &Video_memory[(int)rec_coord.x_ + ((int)(coords1_window_.y_ - coords0_window_.y_) - (int)rec_coord.y_) * (int)(coords1_window_.x_ - coords0_window_.x_)];
		pixel->rgbRed = (BYTE)color.x_;
		pixel->rgbGreen = (BYTE)color.y_;
		pixel->rgbBlue = (BYTE)color.z_;
	}


	void draw_circle(const vector& coords, double r, const Color& color) {
		vector rec_coord = to_pixels(coords);
		txSetFillColor(RGB(color.x_, color.y_, color.z_));
		txCircle(rec_coord.x_, rec_coord.y_, r);
	}


	void create_window() {
		txCreateWindow(coords1_window_.x_, coords1_window_.y_);
		Video_memory = txVideoMemory();
	}


	void draw_window() {
		txRectangle(coords0_sys_.x_, coords0_sys_.y_, coords1_sys_.x_, coords1_sys_.y_);
	}


	void set_color_back(const Color& background) {
		txSetFillColor(RGB(background.x_, background.y_, background.z_));
	}


	void begin() {
		txBegin();
	}


	void end() {
		txEnd();
	}


	void draw_page(HDC* page) {
		txBitBlt(txDC(), coords1_sys_.x_ + 15, coords0_sys_.y_ + 60, width_of_page_, high_of_page_, *page);
	}


	Window(vector coords0_window, vector coords1_window, vector coords0_sys, vector coords1_sys, double scaleX, double scaleY, double width_of_page, double high_of_page) :
		Video_memory(nullptr),
		coords0_window_(coords0_window),
		coords1_window_(coords1_window),
		coords0_sys_(coords0_sys),
		coords1_sys_(coords1_sys),
		coords0_menu(coords1_sys.x_, 0),
		coords1_menu(coords1_window),
		width_of_page_(width_of_page),
		high_of_page_(high_of_page),
		scaleX_(scaleX),
		scaleY_(scaleY)
	{}
};



class Canvas {
public:
	vector coords0_, coords1_, coords0_menu_, coords1_menu_;
	Color frame_, background_;
	HDC* dc_;


	void draw_page() {
		txBitBlt(txDC(), coords1_menu_.x_ + 15, coords0_menu_.y_ + 60, txGetExtentX(*dc_), txGetExtentY(*dc_), *dc_);
	}

	void draw_canvas() {
		txSetColor(RGB(frame_.x_, frame_.y_, frame_.z_), 1, *dc_);
		txSetFillColor(RGB(background_.x_, background_.y_, background_.z_), *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);
		draw_page();
	}


	Canvas(vector coords0, vector coords1, vector coords0_menu, vector coords1_menu, Color frame, Color background, HDC* dc = &txDC()) :
		coords0_(coords0),
		coords1_(coords1),
		coords0_menu_(coords0_menu),
		coords1_menu_(coords1_menu),
		frame_(frame),
		background_(background),
		dc_(dc)
	{}
};



class BasicButton : public Canvas {
public:

	bool is_visible_;


	virtual void draw_button() {
		txSetColor(RGB(frame_.x_, frame_.y_, frame_.z_), 1, *dc_);
		txSetFillColor(RGB(background_.x_, background_.y_, background_.z_), *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);
		draw_page();
	}


	virtual bool if_button_pressed() {
		if (txMouseButtons() == 1 &&
			txMouseX() >= coords0_.x_ + coords0_menu_.x_ &&
			txMouseX() <= coords1_.x_ + coords0_menu_.x_ &&
			txMouseY() >= coords0_.y_ &&
			txMouseY() <= coords1_.y_) { 

			txSleep(120);

			return true;
		}
		else
			return false;
	}

	virtual void action() {}

	BasicButton(vector coords0, vector coords1, vector coords0_menu, vector coords1_menu, Color frame, Color background, bool is_visible, HDC* dc) :
		Canvas(coords0, coords1, coords0_menu, coords1_menu, frame, background, dc),
		is_visible_(is_visible)
	{}
};



class ParameterChangingButton : public BasicButton {
public:
	const char* name_;
	double* parametr_;
	int font_size_;


	void draw_clear_button() {
		txSetColor(RGB(background_.x_, background_.y_, background_.z_), 1, *dc_);
		txSetFillColor(RGB(background_.x_, background_.y_, background_.z_), *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);
		txSetColor(TX_BLACK, 1, *dc_);
		txSelectFont("Calibri", font_size_, -1, FW_DONTCARE, false, false, false, 0, *dc_);
		txDrawText(coords0_.x_, coords0_.y_, coords1_.x_, 2 * coords0_.y_ - coords1_.y_, name_, DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
	}

	void redraw_with_new_text(std::string parametr) {

		draw_clear_button();

		std::vector<char> writable(parametr.begin(), parametr.end());
		writable.push_back('\0');

		txDrawText(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, &writable[0], DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
	}

	virtual void draw_button() override {
		txBegin();
		double parametr = *parametr_;
		std:string str_parametr = std::to_string(parametr);
		redraw_with_new_text(str_parametr);
		draw_page();
		txEnd();
	}


	/*virtual void action() override {
		while (!GetAsyncKeyState(VK_RETURN) && !GetAsyncKeyState(VK_ESCAPE)) {
			double parametr = *parametr_;

			std:string str_parametr = std::to_string(parametr);
			std::vector<char> writable(str_parametr.begin(), str_parametr.end());
			writable.push_back('\0');

			txSetColor(TX_BLACK);
			txDrawText(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, &writable[0]);

			while (_kbhit()) _getch();
			if (!(std::cin >> parametr)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

			if (parametr >= 0 && parametr <= 255)
				*parametr_ = parametr;
		}
	}
	*/


	void redraw_with_frame() {
		txSetColor(RGB(frame_.x_, frame_.y_, frame_.z_), 4, *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);
		draw_button();
	}

	void redraw_without_frame() {
		txSetColor(RGB(179, 175, 191), 4, *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);
		draw_button();
	}



	virtual void action() override {
		redraw_with_frame();


		static std::string parametr;

		while (!txGetAsyncKeyState(VK_RETURN) && !txGetAsyncKeyState(VK_ESCAPE)) {

			int keys[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };
			const int key_sleep = 200;

			for (int i = 0; i < 10; i++) {
				if (txGetAsyncKeyState(keys[i])) {
					parametr += keys[i];
					redraw_with_new_text(parametr);
					txSleep(key_sleep);
				}
			}

			if (txGetAsyncKeyState(VK_OEM_PERIOD)) {
				parametr += '.';
				redraw_with_new_text(parametr);
				txSleep(key_sleep);

			}

			if (txGetAsyncKeyState(VK_BACK) && parametr.length() > 0) {
				parametr.pop_back();
				redraw_with_new_text(parametr);
				txSleep(key_sleep);
			}
		}

		double new_value = *parametr_;

		try {
			new_value = std::stod(parametr);
		}
		catch (const std::invalid_argument& ia) {}

		*parametr_ = new_value;
		parametr.clear();


		redraw_without_frame();
	}



	ParameterChangingButton(vector coords0, vector coords1, vector coords0_menu, vector coords1_menu, const char* name, double* parametr, Color frame, Color background, int font_size, bool is_visible, HDC *dc) :
		BasicButton(coords0, coords1, coords0_menu, coords1_menu, frame, background, is_visible, dc),
		name_(name),
		parametr_(parametr),
		font_size_(font_size)
	{}
};



class FunctionalButton : public BasicButton {
public:
	const char* name_;
	func_t function_;


	virtual void draw_button() override {
		txSetColor(RGB(frame_.x_, frame_.y_, frame_.z_), 1, *dc_);
		txSetFillColor(RGB(background_.x_, background_.y_, background_.z_), *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);
		txDrawText(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, name_, DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		draw_page();
	}


	virtual void action() override {
		function_();
	}


	FunctionalButton(vector coords0, vector coords1, vector coords0_menu, vector coords1_menu, const char* name, func_t function, Color frame, Color background, bool is_visible, HDC* dc) :
		BasicButton(coords0, coords1, coords0_menu, coords1_menu, frame, background, is_visible, dc),
		name_(name),
		function_(function)
	{}
};



class ManagerButton {
public:

	static const int number_of_buttons = 50;
	BasicButton* buttons_[number_of_buttons];


	void add(BasicButton* button) {
		int i;
		for (i = 0; buttons_[i] != NULL; ++i) {}
		buttons_[i] = button;
	}


	void draw_buttons() {
		for (int i = 0; buttons_[i] != NULL; i++)
			if (buttons_[i]->is_visible_ == true) {
				buttons_[i]->draw_button();
			}
	}


	void run() {
		for (int i = 0; buttons_[i] != NULL; i++) {
			if (buttons_[i]->is_visible_ == true) {
				buttons_[i]->draw_button();
				if (buttons_[i]->if_button_pressed()) {
					buttons_[i]->action();
				}
			}
		}
	}


	ManagerButton() {
		for (int i = 0; i < number_of_buttons; i++)
			buttons_[i] = NULL;
	}


	~ManagerButton() {
		for (int i = 0; buttons_[i] != NULL; ++i)
			delete buttons_[i];
	}
};



class SpherePole : public BasicButton {

	Sphere* sphere_;
	ManagerButton manager_of_parametrs;


	vector coords0_type_pole;
	vector coords1_type_pole;
	vector coords0_center;
	vector coords1_center;
	vector coords0_radius;
	vector coords1_radius;
	vector coords0_color;
	vector coords1_color;
	vector coords0_specular;
	vector coords1_specular;
	vector coords0_reflective;
	vector coords1_reflective;
	vector coords0_refractive;
	vector coords1_refractive;
	vector coords0_n;
	vector coords1_n;


public:

	virtual void draw_button() {
		txBegin();

		txSetColor(RGB(background_.x_, background_.y_, background_.z_), 1, *dc_);
		txSetFillColor(RGB(background_.x_, background_.y_, background_.z_), *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);

		txSetColor(TX_BLACK, 1, *dc_);
		txSelectFont("Calibri", 17, -1, FW_DONTCARE, false, false, false, 0, *dc_);
		txDrawText(coords0_type_pole.x_, coords0_type_pole.y_, coords1_type_pole.x_, coords1_type_pole.y_, "  Type:\n Sphere", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_center.x_, coords0_center.y_, coords1_center.x_, coords1_center.y_, "Center:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_radius.x_, coords0_radius.y_, coords1_radius.x_, coords1_radius.y_, "Radius:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_color.x_, coords0_color.y_, coords1_color.x_, coords1_color.y_, "Color:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_specular.x_, coords0_specular.y_, coords1_specular.x_, coords1_specular.y_, "Specular:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_reflective.x_, coords0_reflective.y_, coords1_reflective.x_, coords1_reflective.y_, "Reflective:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_refractive.x_, coords0_refractive.y_, coords1_refractive.x_, coords1_refractive.y_, "Refractive:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_n.x_, coords0_n.y_, coords1_n.x_, coords1_n.y_, "n:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		draw_page();

		txEnd();
	}


	void redraw_with_frame() {
		txSetColor(RGB(frame_.x_, frame_.y_, frame_.z_), 8, *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);
		draw_button();
	}

	void redraw_without_frame() {
		txSetColor(RGB(84, 94, 140), 8, *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);
		draw_button();
	}


	virtual void action() override {
		redraw_with_frame();
		while (txMouseX() >= coords0_.x_ + coords0_menu_.x_ &&
			   txMouseX() <= coords1_.x_ + coords0_menu_.x_ &&
			   txMouseY() >= coords0_.y_ &&
			   txMouseY() <= coords1_.y_) {
			manager_of_parametrs.run();
		}
		redraw_without_frame();
	}


	SpherePole(vector coords0, vector coords1, vector coords0_menu, vector coords1_menu, Sphere* sphere, Color frame, Color background, bool is_visible, HDC* dc) :
		BasicButton(coords0, coords1, coords0_menu, coords1_menu, frame, background, is_visible, dc),
		sphere_(sphere),

		coords0_type_pole(coords0_ + vector(-10, 5)),
		coords1_type_pole(coords0_type_pole + vector(70, 40)),
		coords0_center(coords1_type_pole.x_ + 5, coords0_type_pole.y_ - 17),
		coords1_center(coords0_center + vector(50, 50)),
		coords0_radius(coords1_center.x_ + 10, coords0_center.y_),
		coords1_radius(coords0_radius + vector(50, 50)),
		coords0_color(coords1_radius.x_ + 10, coords0_radius.y_),
		coords1_color(coords0_color + vector(50, 50)),
		coords0_specular(coords0_type_pole.x_ + 13, coords1_type_pole.y_ - 20),
		coords1_specular(coords0_specular + vector(50, 50)),
		coords0_reflective(coords1_specular.x_ + 10, coords0_specular.y_),
		coords1_reflective(coords0_reflective + vector(50, 50)),
		coords0_refractive(coords1_reflective.x_ + 10, coords0_reflective.y_),
		coords1_refractive(coords0_refractive + vector(60, 50)),
		coords0_n(coords1_refractive.x_ + 10, coords0_refractive.y_),
		coords1_n(coords0_n + vector(50, 50))
	{

		manager_of_parametrs.add( new ParameterChangingButton(vector(coords0_center.x_ - 5, coords1_center.y_ - 15),
			vector(coords0_center.x_ + 15, coords1_center.y_), coords0_menu, coords1_menu, "", &sphere_->center_.x_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add( new ParameterChangingButton(vector(coords0_center.x_ + 17, coords1_center.y_ - 15),
			vector(coords0_center.x_ + 37, coords1_center.y_), coords0_menu, coords1_menu, "", &sphere_->center_.y_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add( new ParameterChangingButton(vector(coords0_center.x_ + 39, coords1_center.y_ - 15),
			vector(coords0_center.x_ + 59, coords1_center.y_), coords0_menu, coords1_menu, "", &sphere_->center_.z_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add( new ParameterChangingButton(vector(coords0_radius.x_ + 17, coords1_radius.y_ - 15),
			vector(coords0_radius.x_ + 37, coords1_radius.y_), coords0_menu, coords1_menu, "", &sphere_->radius_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_color.x_ - 5, coords1_color.y_ - 15),
			vector(coords0_color.x_ + 15, coords1_color.y_), coords0_menu, coords1_menu, "", &sphere_->color_.x_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_color.x_ + 17, coords1_color.y_ - 15),
			vector(coords0_color.x_ + 37, coords1_color.y_), coords0_menu, coords1_menu, "", &sphere_->color_.y_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_color.x_ + 39, coords1_color.y_ - 15),
			vector(coords0_color.x_ + 59, coords1_color.y_), coords0_menu, coords1_menu, "", &sphere_->color_.z_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_specular.x_ + 17, coords1_specular.y_ - 15),
			vector(coords0_specular.x_ + 37, coords1_specular.y_), coords0_menu, coords1_menu, "", &sphere_->specular_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_reflective.x_ + 17, coords1_reflective.y_ - 15),
			vector(coords0_reflective.x_ + 37, coords1_reflective.y_), coords0_menu, coords1_menu, "", &sphere_->reflective_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_refractive.x_ + 17, coords1_refractive.y_ - 15),
			vector(coords0_refractive.x_ + 37, coords1_refractive.y_), coords0_menu, coords1_menu, "", &sphere_->refractive_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_n.x_ + 17, coords1_n.y_ - 15),
			vector(coords0_n.x_ + 37, coords1_n.y_), coords0_menu, coords1_menu, "", &sphere_->n_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
	}
};



class PlanePole : public BasicButton {

	Plane* plane_;
	ManagerButton manager_of_parametrs;


	vector coords0_type_pole;
	vector coords1_type_pole;
	vector coords0_normal;
	vector coords1_normal;
	vector coords0_D;
	vector coords1_D;
	vector coords0_color;
	vector coords1_color;
	vector coords0_specular;
	vector coords1_specular;
	vector coords0_reflective;
	vector coords1_reflective;
	vector coords0_refractive;
	vector coords1_refractive;
	vector coords0_n;
	vector coords1_n;


public:

	virtual void draw_button() {
		txBegin();

		txSetColor(RGB(background_.x_, background_.y_, background_.z_), 1, *dc_);
		txSetFillColor(RGB(background_.x_, background_.y_, background_.z_), * dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);

		txSetColor(TX_BLACK, 1, *dc_);
		txSelectFont("Calibri", 17, -1, FW_DONTCARE, false, false, false, 0, *dc_);
		txDrawText(coords0_type_pole.x_, coords0_type_pole.y_, coords1_type_pole.x_, coords1_type_pole.y_, "  Type:\n Plane", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_normal.x_, coords0_normal.y_, coords1_normal.x_, coords1_normal.y_, "Normal:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_D.x_, coords0_D.y_, coords1_D.x_, coords1_D.y_, "D:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_color.x_, coords0_color.y_, coords1_color.x_, coords1_color.y_, "Color:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_specular.x_, coords0_specular.y_, coords1_specular.x_, coords1_specular.y_, "Specular:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_reflective.x_, coords0_reflective.y_, coords1_reflective.x_, coords1_reflective.y_, "Reflective:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_refractive.x_, coords0_refractive.y_, coords1_refractive.x_, coords1_refractive.y_, "Refractive:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_n.x_, coords0_n.y_, coords1_n.x_, coords1_n.y_, "n:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		draw_page();

		txEnd();
	}


	void redraw_with_frame() {
		txSetColor(RGB(frame_.x_, frame_.y_, frame_.z_), 8, *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);
		draw_button();
	}

	void redraw_without_frame() {
		txSetColor(RGB(84, 94, 140), 8, *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);
		draw_button();
	}


	virtual void action() override {
		redraw_with_frame();
		while (txMouseX() >= coords0_.x_ + coords0_menu_.x_ &&
			   txMouseX() <= coords1_.x_ + coords0_menu_.x_ &&
			   txMouseY() >= coords0_.y_ &&
			   txMouseY() <= coords1_.y_) {
			manager_of_parametrs.run();
		}
		redraw_without_frame();
	}


	PlanePole(vector coords0, vector coords1, vector coords0_menu, vector coords1_menu, Plane* plane, Color frame, Color background, bool is_visible, HDC* dc) :
		BasicButton(coords0, coords1, coords0_menu, coords1_menu, frame, background, is_visible, dc),
		plane_(plane),

		coords0_type_pole(coords0_ + vector(-10, 5)),
		coords1_type_pole(coords0_type_pole + vector(70, 40)),
		coords0_normal(coords1_type_pole.x_ + 5, coords0_type_pole.y_ - 17),
		coords1_normal(coords0_normal + vector(50, 50)),
		coords0_D(coords1_normal.x_ + 10, coords0_normal.y_),
		coords1_D(coords0_D + vector(50, 50)),
		coords0_color(coords1_D.x_ + 10, coords0_D.y_),
		coords1_color(coords0_color + vector(50, 50)),
		coords0_specular(coords0_type_pole.x_ + 13, coords1_type_pole.y_ - 20),
		coords1_specular(coords0_specular + vector(50, 50)),
		coords0_reflective(coords1_specular.x_ + 10, coords0_specular.y_),
		coords1_reflective(coords0_reflective + vector(50, 50)),
		coords0_refractive(coords1_reflective.x_ + 10, coords0_reflective.y_),
		coords1_refractive(coords0_refractive + vector(60, 50)),
		coords0_n(coords1_refractive.x_ + 10, coords0_refractive.y_),
		coords1_n(coords0_n + vector(50, 50))
	{

		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_normal.x_ - 5, coords1_normal.y_ - 15),
			vector(coords0_normal.x_ + 15, coords1_normal.y_), coords0_menu, coords1_menu, "", &plane_->normal_.x_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_normal.x_ + 17, coords1_normal.y_ - 15),
			vector(coords0_normal.x_ + 37, coords1_normal.y_), coords0_menu, coords1_menu, "", &plane_->normal_.y_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_normal.x_ + 39, coords1_normal.y_ - 15),
			vector(coords0_normal.x_ + 59, coords1_normal.y_), coords0_menu, coords1_menu, "", &plane_->normal_.z_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_D.x_ + 17, coords1_D.y_ - 15),
			vector(coords0_D.x_ + 37, coords1_D.y_), coords0_menu, coords1_menu, "", &plane_->D_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_color.x_ - 5, coords1_color.y_ - 15),
			vector(coords0_color.x_ + 15, coords1_color.y_), coords0_menu, coords1_menu, "", &plane_->color_.x_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_color.x_ + 17, coords1_color.y_ - 15),
			vector(coords0_color.x_ + 37, coords1_color.y_), coords0_menu, coords1_menu, "", &plane_->color_.y_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_color.x_ + 39, coords1_color.y_ - 15),
			vector(coords0_color.x_ + 59, coords1_color.y_), coords0_menu, coords1_menu, "", &plane_->color_.z_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_specular.x_ + 17, coords1_specular.y_ - 15),
			vector(coords0_specular.x_ + 37, coords1_specular.y_), coords0_menu, coords1_menu, "", &plane_->specular_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_reflective.x_ + 17, coords1_reflective.y_ - 15),
			vector(coords0_reflective.x_ + 37, coords1_reflective.y_), coords0_menu, coords1_menu, "", &plane_->reflective_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_refractive.x_ + 17, coords1_refractive.y_ - 15),
			vector(coords0_refractive.x_ + 37, coords1_refractive.y_), coords0_menu, coords1_menu, "", &plane_->refractive_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_n.x_ + 17, coords1_n.y_ - 15),
			vector(coords0_n.x_ + 37, coords1_n.y_), coords0_menu, coords1_menu, "", &plane_->n_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
	}
};



class RectanPole : public BasicButton {

	Rectan* rectan_;
	ManagerButton manager_of_parametrs;


	vector coords0_type_pole;
	vector coords1_type_pole;
	vector coords0_start;
	vector coords1_start;
	vector coords0_i;
	vector coords1_i;
	vector coords0_j;
	vector coords1_j;
	vector coords0_length1;
	vector coords1_length1;
	vector coords0_length2;
	vector coords1_length2;
	vector coords0_color;
	vector coords1_color;
	vector coords0_specular;
	vector coords1_specular;
	vector coords0_reflective;
	vector coords1_reflective;
	vector coords0_refractive;
	vector coords1_refractive;
	vector coords0_n;
	vector coords1_n;


public:

	virtual void draw_button() {
		txBegin();

		txSetColor(RGB(background_.x_, background_.y_, background_.z_), 1, *dc_);
		txSetFillColor(RGB(background_.x_, background_.y_, background_.z_), *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);

		txSetColor(TX_BLACK, 1, *dc_);
		txSelectFont("Calibri", 17, -1, FW_DONTCARE, false, false, false, 0, *dc_);
		txDrawText(coords0_type_pole.x_, coords0_type_pole.y_, coords1_type_pole.x_, coords1_type_pole.y_, "  Type:\n Rectangle", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_start.x_, coords0_start.y_, coords1_start.x_, coords1_start.y_, "Start:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_i.x_, coords0_i.y_, coords1_i.x_, coords1_i.y_, "i:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_j.x_, coords0_j.y_, coords1_j.x_, coords1_j.y_, "j:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_length1.x_, coords0_length1.y_, coords1_length1.x_, coords1_length1.y_, "Length1:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_length2.x_, coords0_length2.y_, coords1_length2.x_, coords1_length2.y_, "Length2:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_color.x_, coords0_color.y_, coords1_color.x_, coords1_color.y_, "Color:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_specular.x_, coords0_specular.y_, coords1_specular.x_, coords1_specular.y_, "Specular:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_reflective.x_, coords0_reflective.y_, coords1_reflective.x_, coords1_reflective.y_, "Reflective:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_refractive.x_, coords0_refractive.y_, coords1_refractive.x_, coords1_refractive.y_, "Refractive:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_n.x_, coords0_n.y_, coords1_n.x_, coords1_n.y_, "n:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		draw_page();

		txEnd();
	}


	void redraw_with_frame() {
		txSetColor(RGB(frame_.x_, frame_.y_, frame_.z_), 8, *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);
		draw_button();
	}

	void redraw_without_frame() {
		txSetColor(RGB(84, 94, 140), 8, *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);
		draw_button();
	}


	virtual void action() override {
		redraw_with_frame();
		while (txMouseX() >= coords0_.x_ + coords0_menu_.x_ &&
			   txMouseX() <= coords1_.x_ + coords0_menu_.x_ &&
			   txMouseY() >= coords0_.y_ &&
			   txMouseY() <= coords1_.y_) {
			manager_of_parametrs.run();
		}
		redraw_without_frame();
	}


	RectanPole(vector coords0, vector coords1, vector coords0_menu, vector coords1_menu, Rectan* rectan, Color frame, Color background, bool is_visible, HDC* dc) :
		BasicButton(coords0, coords1, coords0_menu, coords1_menu, frame, background, is_visible, dc),
		rectan_(rectan),

		coords0_type_pole(coords0_ + vector(-10, 5)),
		coords1_type_pole(coords0_type_pole + vector(70, 40)),
		coords0_start(coords1_type_pole.x_ + 5, coords0_type_pole.y_ - 17),
		coords1_start(coords0_start + vector(50, 50)),
		coords0_i(coords1_start.x_ + 10, coords0_start.y_),
		coords1_i(coords0_i + vector(50, 50)),
		coords0_j(coords1_i.x_ + 12, coords0_i.y_),
		coords1_j(coords0_j + vector(50, 50)),
		coords0_length1(coords0_type_pole.x_ + 30, coords1_type_pole.y_ - 15 ),
		coords1_length1(coords0_length1 + vector(60, 50)),
		coords0_length2(coords1_length1.x_ + 20, coords0_length1.y_),
		coords1_length2(coords0_length2 + vector(60, 50)),
		coords0_color(coords1_length2.x_ + 20, coords0_length2.y_),
		coords1_color(coords0_color + vector(60, 50)),
		coords0_specular(coords0_type_pole.x_ + 13, coords1_length1.y_ - 15),
		coords1_specular(coords0_specular + vector(50, 50)),
		coords0_reflective(coords1_specular.x_ + 10, coords0_specular.y_),
		coords1_reflective(coords0_reflective + vector(50, 50)),
		coords0_refractive(coords1_reflective.x_ + 10, coords0_reflective.y_),
		coords1_refractive(coords0_refractive + vector(60, 50)),
		coords0_n(coords1_refractive.x_ + 10, coords0_refractive.y_),
		coords1_n(coords0_n + vector(50, 50))
	{
		
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_start.x_ -7, coords1_start.y_ - 15),
			vector(coords0_start.x_ + 11, coords1_start.y_), coords0_menu, coords1_menu, "", &rectan_->coords0_.x_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_start.x_ + 12, coords1_start.y_ - 15),
			vector(coords0_start.x_ + 30, coords1_start.y_), coords0_menu, coords1_menu, "", &rectan_->coords0_.y_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_start.x_ + 31, coords1_start.y_ - 15),
			vector(coords0_start.x_ + 49, coords1_start.y_), coords0_menu, coords1_menu, "", &rectan_->coords0_.z_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_i.x_ - 5, coords1_i.y_ - 15),
			vector(coords0_i.x_ + 13, coords1_i.y_), coords0_menu, coords1_menu, "", &rectan_->i_.x_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_i.x_ + 14, coords1_i.y_ - 15),
			vector(coords0_i.x_ + 32, coords1_i.y_), coords0_menu, coords1_menu, "", &rectan_->i_.y_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_i.x_ + 33, coords1_i.y_ - 15),
			vector(coords0_i.x_ + 51, coords1_i.y_), coords0_menu, coords1_menu, "", &rectan_->i_.z_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_j.x_ - 5, coords1_j.y_ - 15),
			vector(coords0_j.x_ + 13, coords1_j.y_), coords0_menu, coords1_menu, "", &rectan_->j_.x_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_j.x_ + 14, coords1_j.y_ - 15),
			vector(coords0_j.x_ + 32, coords1_j.y_), coords0_menu, coords1_menu, "", &rectan_->j_.y_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_j.x_ + 33, coords1_j.y_ - 15),
			vector(coords0_j.x_ + 51, coords1_j.y_), coords0_menu, coords1_menu, "", &rectan_->j_.z_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_length1.x_ + 17, coords1_length1.y_ - 15),
			vector(coords0_length1.x_ + 37, coords1_length1.y_), coords0_menu, coords1_menu, "", &rectan_->l1_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_length2.x_ + 17, coords1_length2.y_ - 15),
			vector(coords0_length2.x_ + 37, coords1_length2.y_), coords0_menu, coords1_menu, "", &rectan_->l2_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_color.x_ + 2, coords1_color.y_ - 15),
			vector(coords0_color.x_ + 20, coords1_color.y_), coords0_menu, coords1_menu, "", &rectan_->color_.x_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_color.x_ + 21, coords1_color.y_ - 15),
			vector(coords0_color.x_ + 39, coords1_color.y_), coords0_menu, coords1_menu, "", &rectan_->color_.y_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_color.x_ + 40, coords1_color.y_ - 15),
			vector(coords0_color.x_ + 58, coords1_color.y_), coords0_menu, coords1_menu, "", &rectan_->color_.z_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_specular.x_ + 17, coords1_specular.y_ - 15),
			vector(coords0_specular.x_ + 37, coords1_specular.y_), coords0_menu, coords1_menu, "", &rectan_->specular_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_reflective.x_ + 17, coords1_reflective.y_ - 15),
			vector(coords0_reflective.x_ + 37, coords1_reflective.y_), coords0_menu, coords1_menu, "", &rectan_->reflective_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_refractive.x_ + 17, coords1_refractive.y_ - 15),
			vector(coords0_refractive.x_ + 37, coords1_refractive.y_), coords0_menu, coords1_menu, "", &rectan_->refractive_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_n.x_ + 17, coords1_n.y_ - 15),
			vector(coords0_n.x_ + 37, coords1_n.y_), coords0_menu, coords1_menu, "", &rectan_->n_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
	}
};



class ParallelepipedPole : public BasicButton {

	Parallelepiped* parallelepiped_;
	ManagerButton manager_of_parametrs;


	vector coords0_type_pole;
	vector coords1_type_pole;
	vector coords0_start;
	vector coords1_start;
	vector coords0_i;
	vector coords1_i;
	vector coords0_j;
	vector coords1_j;
	vector coords0_k;
	vector coords1_k;
	vector coords0_length1;
	vector coords1_length1;
	vector coords0_length2;
	vector coords1_length2;
	vector coords0_length3;
	vector coords1_length3;
	vector coords0_color;
	vector coords1_color;
	vector coords0_specular;
	vector coords1_specular;
	vector coords0_reflective;
	vector coords1_reflective;
	vector coords0_refractive;
	vector coords1_refractive;
	vector coords0_n;
	vector coords1_n;


public:



	virtual void draw_button() {
		txBegin();

		txSetColor(RGB(background_.x_, background_.y_, background_.z_), 1, *dc_);
		txSetFillColor(RGB(background_.x_, background_.y_, background_.z_), *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);

		txSetColor(TX_BLACK, 1, *dc_);
		txSelectFont("Calibri", 17, -1, FW_DONTCARE, false, false, false, 0, *dc_);
		txDrawText(coords0_type_pole.x_, coords0_type_pole.y_, coords1_type_pole.x_, coords1_type_pole.y_, "  Type:\n Parallelepiped", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_start.x_, coords0_start.y_, coords1_start.x_, coords1_start.y_, "Start:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_i.x_, coords0_i.y_, coords1_i.x_, coords1_i.y_, "i:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_j.x_, coords0_j.y_, coords1_j.x_, coords1_j.y_, "j:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_k.x_, coords0_k.y_, coords1_k.x_, coords1_k.y_, "k:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_length1.x_, coords0_length1.y_, coords1_length1.x_, coords1_length1.y_, "Length1:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_length2.x_, coords0_length2.y_, coords1_length2.x_, coords1_length2.y_, "Length2:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_length3.x_, coords0_length3.y_, coords1_length3.x_, coords1_length3.y_, "Length3:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_color.x_, coords0_color.y_, coords1_color.x_, coords1_color.y_, "Color:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_specular.x_, coords0_specular.y_, coords1_specular.x_, coords1_specular.y_, "Specular:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_reflective.x_, coords0_reflective.y_, coords1_reflective.x_, coords1_reflective.y_, "Reflective:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_refractive.x_, coords0_refractive.y_, coords1_refractive.x_, coords1_refractive.y_, "Refractive:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);
		txDrawText(coords0_n.x_, coords0_n.y_, coords1_n.x_, coords1_n.y_, "n:", DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS, *dc_);

		draw_page();

		txEnd();
	}


	void redraw_with_frame() {
		txSetColor(RGB(frame_.x_, frame_.y_, frame_.z_), 8, *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);
		draw_button();
	}

	void redraw_without_frame() {
		txSetColor(RGB(84, 94, 140), 8, *dc_);
		txRectangle(coords0_.x_, coords0_.y_, coords1_.x_, coords1_.y_, *dc_);
		draw_button();
	}


	virtual void action() override {
		redraw_with_frame();
		while (txMouseX() >= coords0_.x_ + coords0_menu_.x_ &&
			   txMouseX() <= coords1_.x_ + coords0_menu_.x_ &&
			   txMouseY() >= coords0_.y_ &&
			   txMouseY() <= coords1_.y_) {
			manager_of_parametrs.run();
		}
		redraw_without_frame();
	}


	ParallelepipedPole(vector coords0, vector coords1, vector coords0_menu, vector coords1_menu, Parallelepiped* parallelepiped, Color frame, Color background, bool is_visible, HDC* dc) :
		BasicButton(coords0, coords1, coords0_menu, coords1_menu, frame, background, is_visible, dc),
		parallelepiped_(parallelepiped),

		coords0_type_pole(coords0_ + vector(-10, 5)),
		coords1_type_pole(coords0_type_pole + vector(93, 40)),
		coords0_start(coords1_type_pole.x_ + 25, coords0_type_pole.y_ - 17),
		coords1_start(coords0_start + vector(50, 50)),
		coords0_color(coords1_start.x_ + 25, coords0_start.y_),
		coords1_color(coords0_color + vector(50, 50)),
		coords0_i(coords0_type_pole.x_ + 30, coords1_type_pole.y_ - 15),
		coords1_i(coords0_i + vector(60, 50)),
		coords0_j(coords1_i.x_ + 20, coords0_i.y_),
		coords1_j(coords0_j + vector(60, 50)),
		coords0_k(coords1_j.x_ + 20, coords0_j.y_),
		coords1_k(coords0_k + vector(60, 50)),
		coords0_length1(coords0_type_pole.x_ + 30, coords1_i.y_ - 15),
		coords1_length1(coords0_length1 + vector(60, 50)),
		coords0_length2(coords1_length1.x_ + 20, coords0_length1.y_),
		coords1_length2(coords0_length2 + vector(60, 50)),
		coords0_length3(coords1_length2.x_ + 20, coords0_length2.y_),
		coords1_length3(coords0_length3 + vector(60, 50)),
		coords0_specular(coords0_type_pole.x_ + 13, coords1_length1.y_ - 15),
		coords1_specular(coords0_specular + vector(50, 50)),
		coords0_reflective(coords1_specular.x_ + 10, coords0_specular.y_),
		coords1_reflective(coords0_reflective + vector(50, 50)),
		coords0_refractive(coords1_reflective.x_ + 10, coords0_reflective.y_),
		coords1_refractive(coords0_refractive + vector(60, 50)),
		coords0_n(coords1_refractive.x_ + 10, coords0_refractive.y_),
		coords1_n(coords0_n + vector(50, 50))
	{

		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_start.x_ - 5, coords1_start.y_ - 15),
			vector(coords0_start.x_ + 15, coords1_start.y_), coords0_menu, coords1_menu, "", &parallelepiped_->coords0_.x_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_start.x_ + 17, coords1_start.y_ - 15),
			vector(coords0_start.x_ + 37, coords1_start.y_), coords0_menu, coords1_menu, "", &parallelepiped_->coords0_.y_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_start.x_ + 39, coords1_start.y_ - 15),
			vector(coords0_start.x_ + 59, coords1_start.y_), coords0_menu, coords1_menu, "", &parallelepiped_->coords0_.z_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_color.x_ - 5, coords1_color.y_ - 15),
			vector(coords0_color.x_ + 15, coords1_color.y_), coords0_menu, coords1_menu, "", &parallelepiped_->color_.x_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_color.x_ + 17, coords1_color.y_ - 15),
			vector(coords0_color.x_ + 37, coords1_color.y_), coords0_menu, coords1_menu, "", &parallelepiped_->color_.y_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_color.x_ + 39, coords1_color.y_ - 15),
			vector(coords0_color.x_ + 59, coords1_color.y_), coords0_menu, coords1_menu, "", &parallelepiped_->color_.z_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_i.x_ - 5, coords1_i.y_ - 15),
			vector(coords0_i.x_ + 15, coords1_i.y_), coords0_menu, coords1_menu, "", &parallelepiped_->i_.x_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_i.x_ + 17, coords1_i.y_ - 15),
			vector(coords0_i.x_ + 37, coords1_i.y_), coords0_menu, coords1_menu, "", &parallelepiped_->i_.y_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_i.x_ + 39, coords1_i.y_ - 15),
			vector(coords0_i.x_ + 59, coords1_i.y_), coords0_menu, coords1_menu, "", &parallelepiped_->i_.z_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_j.x_ - 5, coords1_j.y_ - 15),
			vector(coords0_j.x_ + 15, coords1_j.y_), coords0_menu, coords1_menu, "", &parallelepiped_->j_.x_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_j.x_ + 17, coords1_j.y_ - 15),
			vector(coords0_j.x_ + 37, coords1_j.y_), coords0_menu, coords1_menu, "", &parallelepiped_->j_.y_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_j.x_ + 39, coords1_j.y_ - 15),
			vector(coords0_j.x_ + 59, coords1_j.y_), coords0_menu, coords1_menu, "", &parallelepiped_->j_.z_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_k.x_ - 5, coords1_k.y_ - 15),
			vector(coords0_k.x_ + 15, coords1_k.y_), coords0_menu, coords1_menu, "", &parallelepiped_->k_.x_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_k.x_ + 17, coords1_k.y_ - 15),
			vector(coords0_k.x_ + 37, coords1_k.y_), coords0_menu, coords1_menu, "", &parallelepiped_->k_.y_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_k.x_ + 39, coords1_k.y_ - 15),
			vector(coords0_k.x_ + 59, coords1_k.y_), coords0_menu, coords1_menu, "", &parallelepiped_->k_.z_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_length1.x_ + 17, coords1_length1.y_ - 15),
			vector(coords0_length1.x_ + 37, coords1_length1.y_), coords0_menu, coords1_menu, "", &parallelepiped_->l1_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_length2.x_ + 17, coords1_length2.y_ - 15),
			vector(coords0_length2.x_ + 37, coords1_length2.y_), coords0_menu, coords1_menu, "", &parallelepiped_->l2_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_length3.x_ + 17, coords1_length3.y_ - 15),
			vector(coords0_length3.x_ + 37, coords1_length3.y_), coords0_menu, coords1_menu, "", &parallelepiped_->l3_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_specular.x_ + 17, coords1_specular.y_ - 15),
			vector(coords0_specular.x_ + 37, coords1_specular.y_), coords0_menu, coords1_menu, "", &parallelepiped_->specular_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_reflective.x_ + 17, coords1_reflective.y_ - 15),
			vector(coords0_reflective.x_ + 37, coords1_reflective.y_), coords0_menu, coords1_menu, "", &parallelepiped_->reflective_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_refractive.x_ + 17, coords1_refractive.y_ - 15),
			vector(coords0_refractive.x_ + 37, coords1_refractive.y_), coords0_menu, coords1_menu, "", &parallelepiped_->refractive_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
		manager_of_parametrs.add(new ParameterChangingButton(vector(coords0_n.x_ + 17, coords1_n.y_ - 15),
			vector(coords0_n.x_ + 37, coords1_n.y_), coords0_menu, coords1_menu, "", &parallelepiped_->n_,
			Color(233, 236, 246), Color(255, 255, 255), 13, true, dc_));
	}
};



void add_poles(HDC* pages, Object* arr_of_obj[], ManagerButton* manager_of_buttons, vector coords0_menu, vector coords1_menu, double width_of_page, double high_of_page) {

	int number_dc = 0;
	pages[0] = txCreateCompatibleDC(width_of_page, high_of_page);

	vector coords0(10, 10);
	vector coords1(10, 10);

	for (int i = 0; arr_of_obj[i]->type_ != end; i++) {
		if (arr_of_obj[i]->type_ == sphere || arr_of_obj[i]->type_ == plane)
			coords1 = coords0 + vector(250, 80);
		if (arr_of_obj[i]->type_ == rectan)
			coords1 = coords0 + vector(250, 120);
		if (arr_of_obj[i]->type_ == parallelepiped)
			coords1 = coords0 + vector(250, 155);

		if (coords1.y_ < high_of_page) {
			if (arr_of_obj[i]->type_ == sphere)
				manager_of_buttons->add(new SpherePole(coords0, coords1, coords0_menu, coords1_menu, dynamic_cast<Sphere*>(arr_of_obj[i]), Color(233, 236, 246), Color(179, 175, 191), true, &pages[number_dc]));
			if (arr_of_obj[i]->type_ == plane)
				manager_of_buttons->add(new PlanePole(coords0, coords1, coords0_menu, coords1_menu, dynamic_cast<Plane*>(arr_of_obj[i]), Color(233, 236, 246), Color(179, 175, 191), true, &pages[number_dc]));
			if (arr_of_obj[i]->type_ == rectan)
				manager_of_buttons->add(new RectanPole(coords0, coords1, coords0_menu, coords1_menu, dynamic_cast<Rectan*>(arr_of_obj[i]), Color(233, 236, 246), Color(179, 175, 191), true, &pages[number_dc]));
			if (arr_of_obj[i]->type_ == parallelepiped)
				manager_of_buttons->add(new ParallelepipedPole(coords0, coords1, coords0_menu, coords1_menu, dynamic_cast<Parallelepiped*>(arr_of_obj[i]), Color(233, 236, 246), Color(179, 175, 191), true, &pages[number_dc]));
			coords0 = coords1 + vector(-250, 10);
		}
		else {
			number_dc++;
			pages[number_dc] = txCreateCompatibleDC(width_of_page, high_of_page);
			coords0 = vector(10, 10);
			coords1 = vector(10, 10);
			i--;
		}	
	}
}