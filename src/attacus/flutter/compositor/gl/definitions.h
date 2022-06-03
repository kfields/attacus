#pragma once

#include <cstdint>

namespace attacus {

struct ColorRgb {
    ColorRgb() : r(1.0f), g(1.0f), b(1.0f) {}
    ColorRgb(float _r, float _g, float _b)
        : r(_r), g(_g), b(_b) {}
    ColorRgb(const float* arr)
        : ColorRgb(arr[0], arr[1], arr[2]) {}

    float r;
    float g;
    float b;
};

struct TexCoord {
    TexCoord(float _u, float _v) : u(_u), v(_v) {}
    float u;
    float v;
};

struct Pos2RgbTexCoord0Vertex
{
	float x;
	float y;
    ColorRgb rgb;
	float u;
	float v;
};

} // namespace attacus