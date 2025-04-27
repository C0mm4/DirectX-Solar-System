#pragma once
#include "Object.h"

class CCamera : public Object {
public:
	CCamera() : Object() {
		CameraCenterTarget = NULL;
		lerpValue = 0;

		// �ɼ� �� ����
		option.LerpSpeed = 3;
		option.TargetDistance = 5;
		option.TargetLookDistance = 3;
		option.TargetDistanceY = 2;
		option.stopLerpTimer = 0;
		option.StopLerpDuration = 0.5f;
		option.baseRotationSpeed = 1.f;
		option.additionalRotationSpeed = 2.f;

		// Ʈ���� �ʱ�ȭ
		isLerp = false;
		isTargetTraceLerp = false;
		isTargetTraceLerpEnd = false; 
		option.stopLerpTimer = 0;
		option.StopLerpDuration = 0.5f;
	};

	CCamera(float x, float y, float z) : Object(x, y, z) {
		CameraCenterTarget = NULL;
		lerpValue = 0;

		// �ɼ� �� ����
		option.LerpSpeed = 3;
		option.TargetDistance = 5;
		option.TargetLookDistance = 3;
		option.TargetDistanceY = 2;
		option.stopLerpTimer = 0;
		option.StopLerpDuration = 0.5f;
		option.baseRotationSpeed = 1.f;
		option.additionalRotationSpeed = 2.f;

		// Ʈ���� �ʱ�ȭ
		isLerp = false;
		isTargetTraceLerp = false;
		isTargetTraceLerpEnd = false;
	}

	/// <summary>
	/// ��ü�� �ʱ�ȭ ����. ���� ���� �� �ʿ��� �۾� ��ü �� ����
	/// </summary>
	/// <param name="device">Direct3 ����̽� ��ü ������</param>
	/// <returns>�ʱ�ȭ ���� ����</returns>
	HRESULT Init(LPDIRECT3DDEVICE9 device) override;


	void Render(LPDIRECT3DDEVICE9 deivce) override 	{
		// ī�޶� ����Ʈ�� UI �� �߰� ����? 
	};

	void SetTargetPos(D3DXVECTOR3 pos) {
		targetPos = pos;
	};

	void SetTargetVector(D3DXVECTOR3 dir) {
		targetLookAt = dir;
	}

	/// <summary>
	/// ��ü�� �� �����Ӻ� ���� main�� �޽��� �������� Update ȣ��
	/// </summary>
	/// <param name="deltaT">���� �����Ӱ��� �ð�</param>
	void Update(float deltaT) override;

	/// <summary>
	/// ī�޶��� ��ġ, ���� ���͸� ������� View Transform ����
	/// </summary>
	/// <param name="device">D3D device</param>
	void SetViewTransform(LPDIRECT3DDEVICE9 device);

	/// <summary>
	/// ��ü�� �Ű�������ŭ ȸ�� ȸ���� ��ü�� local matrix�� ����
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void Rotate(float x, float y, float z) override;

	/// <summary>
	/// ī�޶� �߽� ��ü ����
	/// </summary>
	/// <param name="target">�߽� ��ü</param>
	void SetTarget(Object& target);

	/// <summary>
	/// ��ü�� ������ �� ī�޶��� ��ǥ ���� ���
	/// </summary>
	/// <param name="target">��ǥ ��ü</param>
	/// <returns>��ǥ ��ġ</returns>
	D3DXVECTOR3 CalculateNewPosAtSetTarget(Object& target);

	/// <summary>
	/// ��ü�� ������ �� ��ǥ ���� ���� ��� 
	/// CalculateNewPosAtSetTarget �Լ� ȣ�� �� ���
	/// </summary>
	void CalculateNewTargetVector();

	/// <summary>
	/// pos�� targetPos ���� range������ Lerp ��� �ǽ�
	/// </summary>
	/// <param name="range">Lerp range</param>
	void PositionLerp(float range);

	/// <summary>
	/// lookAt Vector�� targetLookAt ���� range ������ Lerp ��� �ǽ� ���� upVec, right Vector�� ���� ���
	/// </summary>
	/// <param name="range">Lerp range</param>
	void RotateLerp(float range);

	/// <summary>
	/// ī�޶� ���� ��ü�� ��ġ�� ����
	/// </summary>
	/// <param name="range">Lerp value</param>
	void TargetPosLerp(float range);

	/// <summary>
	/// ���� ��ü�� ���� ���͸� ���� �� �� ��ü�� lookAt ���͸� �����Ѵ�
	/// </summary>
	/// <param name="range">lerp Value</param>
	void TargetLookAtLerp(float range);

	void KeyInput(float deltaT) override;

private:
	D3DXVECTOR3 targetLookAt, targetUpVec; // using for Lerp
	struct CameraOption {
		float TargetDistance;			// ������ ��ǥ �Ÿ�
		float TargetLookDistance;		// ī�޶� ��ǥ ���� (����� Look At vector�� ũ�⸦ ����)
		float LerpSpeed ;				// ���� �ӵ�
		float TargetDistanceY;			// ������ Y �Ÿ�
		float stopLerpTimer;			// ���� ����� �ε巴�� ���ߴ� �ð� Ÿ�̸� ����
		float StopLerpDuration;			// ���� ���� �� ���� ���� �ð�
		float baseRotationSpeed;			// ī�޶� ȸ�� �ӵ�
		float additionalRotationSpeed;	// ��ü�� ȸ�� �ӵ��� ���� �߰� ȸ�� �ӵ� ���
	};
	CameraOption option;

	float lerpValue;					// ī�޶� ���� ��ü ���� �� �ش� ��ü�� ī�޶� ��ǥ�������� �̵��� �� ����ϴ� lerp ��
	bool isLerp;						// ī�޶� ���� ��ü ���� �� �ش� ��ü�� lerp ���� ����
	D3DXVECTOR3 targetPos;				// ī�޶� ��ü ���� �� ī�޶��� ��ǥ ����
	Object* CameraCenterTarget;			// ī�޶� �����ϴ� ���
	bool isTargetTraceLerp;				// ���� ����� �����ӿ� ���� ī�޶��� ���� ����
	bool isTargetTraceLerpEnd;			// ���� ����� ���� ������ ����ʿ� ���� ������ �������� �� �ε巴�� ���߱� ���� Ʈ����

};