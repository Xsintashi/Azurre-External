#pragma once
struct Matrix4x4 {
	float* operator[ ](int index) {
		return matrix[index];
	}
	float matrix[4][4];
};