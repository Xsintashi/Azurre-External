#include "Helpers.h"

Color3 Helpers::ConvertColors3(float in[3]) {
	Color3 out;
	out.r = in[0] * 255.f;
	out.g = in[1] * 255.f;
	out.b = in[2] * 255.f;

	return out;
}

Color4 Helpers::ConvertColors4(float in[4]) {
	Color4 out;
	out.r = in[0] * 255.f;
	out.g = in[1] * 255.f;
	out.b = in[2] * 255.f;
	out.a = in[3] * 255.f;

	return out;
}