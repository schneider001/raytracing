#include "Object.h"


Object Object::operator = (const Object& that) {
	color_ = that.color_;
	specular_ = that.specular_;
	reflective_ = that.reflective_;
	refractive_ = that.refractive_;
	n_ = that.n_;
	type_ = that.type_;
	return *this;
}
