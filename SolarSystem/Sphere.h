#pragma once
#include "Object.h"

class Sphere : public Object {
public:
	Sphere() : Object() {};
	Sphere(float x, float y, float z, int slices = 20, int stacks = 20, float radius = 1.f) : Object(x, y, z) {
		this->slices = slices;
		this->stacks = stacks;
		this->radius = radius;
	}

	HRESULT Init(LPDIRECT3DDEVICE9 device);

private:
	int slices;
	int stacks;
	float radius;
};