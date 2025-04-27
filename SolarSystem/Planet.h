#pragma once
#include "Sphere.h"
#include "Star.h"

class Planet : public Sphere, public Star {
public:
	Planet() : Sphere() {};
	Planet(float x, float y, float z, int slices = 20, int stacks = 20, float radius = 1.f) : Sphere(x, y, z, slices, stacks, radius) {
	}

	/// <summary>
	/// 매 프레임마다 공전과 자전을 진행하고, 공전의 진행 각도를 더한다
	/// </summary>
	/// <param name="deltaT"></param>
	void Update(float deltaT) override {
		Rotation(deltaT);
		Orbital(deltaT);
		theta += deltaT * orbitalSpd;
	}

	/// <summary>
	/// 자전
	/// </summary>
	/// <param name="deltaT">프레임 시간</param>
	void Rotation(float deltaT) override {
		Rotate(0, -rotationSpd * deltaT * D3DX_PI*2, 0);
	}

	/// <summary>
	/// 공전
	/// </summary>
	/// <param name="deltaT">프레임 시간</param>
	void Orbital(float deltaT) override {
		pos.x = cosf(theta) * orbitalRadius + originStar->GetPos().x;
		pos.z = sinf(theta) * orbitalRadius + originStar->GetPos().z;
	}

	/// <summary>
	/// 중심별 설정
	/// </summary>
	/// <param name="originStar"></param>
	void SetOriginalStar(Sphere* originStar) {
		this->originStar = originStar;
	};

	/// <summary>
	/// 공전 주기와 자전 주기, 공전 거리 설정
	/// </summary>
	/// <param name="rotate">자전 주기</param>
	/// <param name="orbit">공전 주기</param>
	/// <param name="distance">공전 거리</param>
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
	float theta;		// 공전 각도
	float orbitalRadius;	// 공전 거리

	Sphere* originStar; // 중심별
};