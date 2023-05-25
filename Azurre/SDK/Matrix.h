#pragma once

struct Vector;

struct Matrix4x4 {
    float* operator[ ](int index) {
        return matrix[index];
    }
    float matrix[4][4];
};

class Matrix3x4 {
    float mat[3][4];
public:
    constexpr auto operator[](int i) const noexcept { return mat[i]; }
    auto operator[](int i) noexcept { return mat[i]; }
    constexpr auto origin() const noexcept;
};

#include "Vector.h"

constexpr auto Matrix3x4::origin() const noexcept
{
    return Vector{ mat[0][3], mat[1][3], mat[2][3] };
}