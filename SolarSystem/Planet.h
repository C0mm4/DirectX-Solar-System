#pragma once
#include "Sphere.h"
#include "Star.h"

class Planet : public Sphere, public Star {
public:
	Planet() : Sphere() {};
	Planet(float x, float y, float z, int slices = 20, int stacks = 20, float radius = 1.f) : Sphere(x, y, z, slices, stacks, radius) {
	}

	/// <summary>
	/// �� �����Ӹ��� ������ ������ �����ϰ�, ������ ���� ������ ���Ѵ�
	/// </summary>
	/// <param name="deltaT"></param>
	void Update(float deltaT) override {
		Rotation(deltaT);
		Orbital(deltaT);
		theta += deltaT * orbitalSpd;
	}

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="deltaT">������ �ð�</param>
	void Rotation(float deltaT) override {
		Rotate(0, -rotationSpd * deltaT * D3DX_PI*2, 0);
	}

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="deltaT">������ �ð�</param>
	void Orbital(float deltaT) override {
		pos.x = cosf(theta) * orbitalRadius + originStar->GetPos().x;
		pos.z = sinf(theta) * orbitalRadius + originStar->GetPos().z;
	}

	/// <summary>
	/// �߽ɺ� ����
	/// </summary>
	/// <param name="originStar"></param>
	void SetOriginalStar(Sphere* originStar) {
		this->originStar = originStar;
	};

	/// <summary>
	/// ���� �ֱ�� ���� �ֱ�, ���� �Ÿ� ����
	/// </summary>
	/// <param name="rotate">���� �ֱ�</param>
	/// <param name="orbit">���� �ֱ�</param>
	/// <param name="distance">���� �Ÿ�</param>
	void SetValue(float rotate, float orbit, float distance) {
		rotationSpd = rotate;
		orbitalSpd = orbit;
		orbitalRadius = distance;
	}

	void Render(LPDIRECT3DDEVICE9 device) override {
		Sphere::Render(device);


		LineVertex lines[101]; 

		float lineLength = 5.0f;

		D3DXVECTOR3 originStarPos = originStar->GetPos();

		for (int i = 0; i <= 100; i++) {
			lines[i] = { {originStarPos.x + cosf(2 * D3DX_PI / 100 * i) * orbitalRadius, originStarPos.y,
				originStarPos.z + sinf(2 * D3DX_PI / 100 * i) * orbitalRadius},
			D3DCOLOR_ARGB(255, 255, 255, 255)};
		}

		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);
		device->SetTransform(D3DTS_WORLD, &identity);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		device->SetRenderState(D3DRS_LIGHTING, FALSE);
		// Set FVF and Draw Primitive
		device->SetFVF(CUSTOMFVF_LINEVERTEX);
		device->DrawPrimitiveUP(D3DPT_LINESTRIP, 100, lines, sizeof(LineVertex));
		device->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

private:
	float theta;		// ���� ����
	float orbitalRadius;	// ���� �Ÿ�

	Sphere* originStar; // �߽ɺ�
};