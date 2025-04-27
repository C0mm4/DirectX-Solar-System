#include "CCamera.h"
#include "math.h"

HRESULT CCamera::Init(LPDIRECT3DDEVICE9 device) {
    // ���� ���� ���� �ʱ�ȭ
	lookAt = { 0, 0, 1 };
	upVec = { 0, 1, 0 };
    right = { 1, 0, 0 };
	return S_OK;
}

void CCamera::SetViewTransform(LPDIRECT3DDEVICE9 device) {
    // ���� ��ġ�� lookAt ����, upVec�� ������� View Matrix ��� �� ����
	D3DXMATRIX viewTM;
	D3DXVECTOR3 lookPos = lookAt + pos;
	D3DXMatrixLookAtLH(&viewTM, &pos, &lookPos, &upVec);
	device->SetTransform(D3DTS_VIEW, &viewTM);
}

void CCamera::Rotate(float x, float y, float z)
{
    // right ���� ��� �� �� ���⺤�� ����ȭ
    D3DXVec3Cross(&right, &upVec, &lookAt);
    D3DXVec3Normalize(&right, &right);
    D3DXVec3Normalize(&upVec, &upVec);
    D3DXVec3Normalize(&lookAt, &lookAt);

    if (x != 0.0f)
    {
        // X�� ȸ��
        D3DXMATRIX rotX;
        D3DXMatrixRotationAxis(&rotX, &right, x);
        D3DXVec3TransformNormal(&upVec, &upVec, &rotX);
        D3DXVec3TransformNormal(&lookAt, &lookAt, &rotX);
    }

    if (y != 0.0f)
    {
        // Y�� ȸ��
        D3DXMATRIX rotY;
        D3DXMatrixRotationAxis(&rotY, &upVec, y);
        D3DXVec3TransformNormal(&lookAt, &lookAt, &rotY);
    }

    if (z != 0.0f)
    {
        // Z�� ȸ��
        D3DXMATRIX rotZ;
        D3DXMatrixRotationAxis(&rotZ, &lookAt, z);
        D3DXVec3TransformNormal(&upVec, &upVec, &rotZ);
    }

    // ���� ���� ����ȭ �� right ���� ����
    D3DXVec3Normalize(&lookAt, &lookAt);
    D3DXVec3Normalize(&upVec, &upVec);
    D3DXVec3Cross(&right, &upVec, &lookAt);
}

void CCamera::SetTarget(Object& target) {
    CameraCenterTarget = &target;

    // ��ǥ ��ġ ����
    targetPos = CalculateNewPosAtSetTarget(target);

    // ��ġ ���� ����
    PositionLerp(0);

    // ��ǥ ���� ���� ����
    CalculateNewTargetVector();

    // ���� ���� ���� ����
    RotateLerp(0);
}

D3DXVECTOR3 CCamera::CalculateNewPosAtSetTarget(Object& target) {
    D3DXVECTOR3 ret;

    // option�� Ÿ�ٰ��� Y �Ÿ��� ������� ��ü�� XZ ��� �� Y �Ÿ���ŭ ������ XZ����� ���� ������ ���
    float plainCircleRadiusSquare = sqrt(pow(option.TargetDistance, 2) + pow(option.TargetDistanceY, 2));

    // ���� �������� ��ü�� ���� ���͸� ������. (ī�޶� �ش� ��ġ���� �ٶ� �� �ֵ���)
    float x = -plainCircleRadiusSquare * target.GetLookAtVector().x;
    float z = -plainCircleRadiusSquare * target.GetLookAtVector().z;

    // ���� ���� ��ǥ�� ��ü�� ��ġ�� ���� ��ü �ֺ��� ������ ���
    auto targetPos = target.GetPos();
    ret.x = targetPos.x + x;
    ret.z = targetPos.z + z;
    ret.y = targetPos.y + option.TargetDistanceY;

    return ret;
}

void CCamera::CalculateNewTargetVector() {
    // ��ǥ lookAt ���
    targetLookAt = CameraCenterTarget->GetPos() - targetPos;
    D3DXVec3Normalize(&targetLookAt, &targetLookAt);

    // �ӽ� upVec (World Coordinate)
    targetUpVec = { 0,1,0 };

    // lookAt, �ӽ� upVec���� right ���
    D3DXVec3Cross(&right, &targetUpVec, &targetLookAt);

    // lookAt, right���� upVec ���
    D3DXVec3Cross(&targetUpVec, &targetLookAt, &right);
}

void CCamera::PositionLerp(float range){
    // pos �� lerp ���
    D3DXVec3Lerp(&pos, &pos, &targetPos, range * 2);
    if (lerpValue >= 0.5f) {
        isLerp = false;
    }
    else isLerp = true;
}

void CCamera::RotateLerp(float range) {
    // lookAt �� lerp ���
    D3DXVec3Lerp(&lookAt, &lookAt, &targetLookAt, range);
    D3DXVec3Normalize(&lookAt, &lookAt);

    // �ӽ� upVec (World Coordionate)
    targetUpVec = { 0,1,0 };
    
    // lookAt, �ӽ� upVec���� right ���
    D3DXVec3Cross(&right, &upVec, &lookAt);
    D3DXVec3Normalize(&right, &right);

    // lookAt, right���� upVec ���
    D3DXVec3Cross(&targetUpVec, &targetLookAt, &right);
}

void CCamera::Update(float deltaT) {
    // ī�޶� Ÿ�� ������ ��ǥ �������� �̵�
    if (isLerp) {
        lerpValue += deltaT;
        if (CameraCenterTarget != NULL) {
            TargetPosLerp(lerpValue);
            TargetLookAtLerp(lerpValue);
        }
    }
    // ī�޶� Ÿ�� ���� ���� Ÿ�� �̵��� ���� ī�޶� ��ġ, ���� ���� ����
    else if (CameraCenterTarget != NULL) {
        // ��ǥ �������� �Ÿ�, ���� ���� ��Ȯ�� ���
        D3DXVECTOR3 vectorToTarget = CameraCenterTarget->GetPos() - pos;
        float distance = D3DXVec3Length(&vectorToTarget);

        auto targetLookVec = CameraCenterTarget->GetLookAtVector();
        float dot = D3DXVec3Dot(&lookAt, &targetLookVec);

        // ���� ������ �� ��ġ�� ī�޶� ���� ����
        if (isTargetTraceLerp) {
            TargetPosLerp(deltaT);
            TargetLookAtLerp(deltaT);
        }
        // ���� ���� �� �ε巴�� ����
        else if(isTargetTraceLerpEnd){

            // ���� ���� �� ���� ó��
            option.stopLerpTimer += deltaT;
            float fade = 1.0f - (option.stopLerpTimer / option.StopLerpDuration);
            fade = max(fade, 0.0f); // ���� ����


            TargetPosLerp(deltaT * fade);  // �ε巴�� ����
            TargetLookAtLerp(deltaT * fade);

            if (fade <= 0.01f) {
                isTargetTraceLerpEnd = false; // ������ ����
            }
        }

    }
    else {
        if (targetPos != NULL) {
            PositionLerp(deltaT);
        }
        if (targetLookAt != NULL) {
            RotateLerp(deltaT);
        }
    }
}

void CCamera::TargetPosLerp(float deltaT) {
    // ��ǥ ��ġ ���
    targetPos = CalculateNewPosAtSetTarget(*CameraCenterTarget);

    // �ε巴�� �̵�
    PositionLerp(deltaT * option.LerpSpeed);

    // Ÿ�� ��ü�� ī�޶� ���� �Ÿ� ���
    D3DXVECTOR3 toRealTarget = CameraCenterTarget->GetPos() - pos;
    float realTargetDistance = D3DXVec3Length(&toRealTarget);


}

void CCamera::TargetLookAtLerp(float range) {
    // ��ǥ ���� ���� ���
    D3DXVECTOR3 targetLookPos = CameraCenterTarget->GetPos() + CameraCenterTarget->GetLookAtVector() * option.TargetLookDistance;
    D3DXVECTOR3 desiredLook = targetLookPos - pos;
    D3DXVec3Normalize(&desiredLook, &desiredLook);
    targetLookAt = desiredLook;

    // ���� �ٶ󺸴� ����� ��ǥ ������ ���� ���̸� ���
    float dot = D3DXVec3Dot(&lookAt, &targetLookAt);
    dot = max(-1.0f, min(1.0f, dot)); // �����ϰ� Ŭ����

    float angle = acosf(dot); // ���� ������ ���� ����

    // ������ ȸ���ؾ� �� ��츦 ����ؼ� ȸ�� �ӵ� ������ ����
    float rotationSpeedScale = 1.0f + (angle / D3DX_PI) * option.additionalRotationSpeed;
    float adjustedDelta = range * option.baseRotationSpeed * rotationSpeedScale;

    // ���� ������ Lerp
    RotateLerp(adjustedDelta);
}

// Ű �Է�
void CCamera::KeyInput(float deltaT) {
    // WASD ��, ��, ��, ��
    if (GetAsyncKeyState('W')) {
        Move(0, 0, 1 * deltaT);
    }
    if (GetAsyncKeyState('S')) {
        Move(0, 0, -1 * deltaT);
    }
    if (GetAsyncKeyState('A')) {
        Move(-1 * deltaT, 0, 0);
    }
    if (GetAsyncKeyState('D')) {
        Move(1 * deltaT, 0, 0);
    }
    // QE ��, ��
    if (GetAsyncKeyState('Q')) {
        Move(0, -1 * deltaT, 0);
    }
    if (GetAsyncKeyState('E')) {
        Move(0, 1 * deltaT, 0);
    }

    // ����Ű ȸ��
    if (GetAsyncKeyState(VK_RIGHT)) {
        Rotate(0, 0.2 * deltaT, 0);
    }
    if (GetAsyncKeyState(VK_LEFT)) {
        Rotate(0, -0.2 * deltaT, 0);
    }
    if (GetAsyncKeyState(VK_UP)) {
        Rotate(-0.2 * deltaT, 0, 0);
    }
    if (GetAsyncKeyState(VK_DOWN)) {
        Rotate(0.2 * deltaT, 0, 0);
    }
}