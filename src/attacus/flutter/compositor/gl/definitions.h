#pragma once

#include <cstdint>

struct ColorRgba {
    ColorRgba() : r(255), g(255), b(255), a(255) {}
    ColorRgba(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
        : r(_r), g(_g), b(_b), a(_a) {}
    ColorRgba(uint32_t _c) : c(_c) {}
    ColorRgba(float _r, float _g, float _b, float _a)
        : r(int(_r * 255.0f)), g(int(_g * 255.0f)), b(int(_b * 255.0f)), a(int(_a * 255.0f)) {}
    ColorRgba(const float* arr)
        : ColorRgba(arr[0], arr[1], arr[2], arr[3]) {}

    union {
        struct {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };
        uint32_t c;
    };
};

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

struct PosRgbaTexCoord0Vertex
{
	float x;
	float y;
	float z;
    ColorRgba rgba;
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
