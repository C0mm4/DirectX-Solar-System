#pragma once

class Star {
protected:
	float rotationSpd; // 자전 속도
	float orbitalSpd;  // 공전 속도

public:
	virtual void Orbital(float deltaT) {};	// 공전
	virtual void Rotation(float deltaT) {};	// 자전
};