#pragma once

class Star {
protected:
	float rotationSpd; // ���� �ӵ�
	float orbitalSpd;  // ���� �ӵ�

public:
	virtual void Orbital(float deltaT) {};	// ����
	virtual void Rotation(float deltaT) {};	// ����
};