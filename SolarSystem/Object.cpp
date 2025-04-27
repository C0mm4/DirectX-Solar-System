#include "Object.h"


Object::Object() {
    pos.x = pos.y = pos.z = 0;
	upVec = { 0, 1, 0 };
	lookAt = { 0, 0, 1 };
	right = { 1, 0, 0 };
	D3DXMatrixIdentity(&rotateMatrix);
}

Object::Object(float x, float y, float z) {
	pos.x = x; pos.y = y; pos.z = z;
	upVec = { 0, 1, 0 };
	lookAt = { 0, 0, 1 };
	right = { 1, 0, 0 };
	D3DXMatrixIdentity(&rotateMatrix);
};

void Object::Render(LPDIRECT3DDEVICE9 device) {
    device->SetMaterial(&material);
    device->SetFVF(CUSTOMFVF);  // FVF 설정
    // 정점 버퍼 설정
    device->SetStreamSource(0, _vertexes, 0, sizeof(CUSTOMVERTEX));

    // 인덱스 버퍼 설정
    device->SetIndices(_indicies);

    // 텍스처 설정 (필요한 경우)
    device->SetTexture(0, _texture);

    D3DXMATRIX trans;
    D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);

    D3DXMATRIX worldMatrix = rotateMatrix * trans;
    device->SetTransform(D3DTS_WORLD, &worldMatrix);

    device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, polygonCount);


}

void Object::Move(float x, float y, float z) {

	D3DXVECTOR3 movement = right * x + upVec * y + lookAt * z;

	pos += movement;
}

void Object::MoveInWorld(float x, float y, float z) {

    D3DXVECTOR3 movement = { x,y,z };

    pos += movement;
}

void Object::Rotate(float x, float y, float z) {

    // ReCalculate right vector, Renormalize direction vector
    D3DXVec3Cross(&right, &upVec, &lookAt);
    D3DXVec3Normalize(&right, &right);
    D3DXVec3Normalize(&upVec, &upVec);
    D3DXVec3Normalize(&lookAt, &lookAt);


    // Set Axis matrix Identity
    D3DXMATRIX rotX, rotY, rotZ;
    D3DXMatrixIdentity(&rotX);
    D3DXMatrixIdentity(&rotY);
    D3DXMatrixIdentity(&rotZ);

    // Local Z Axis Rotation
    if (z != 0.0f)
    {
        // calculate matrix, vector
        D3DXMatrixRotationAxis(&rotZ, &lookAt, z);
        D3DXVec3TransformNormal(&upVec, &upVec, &rotZ);
    }

    if (x != 0.0f)
    {
        // calculate matrix, vector
        D3DXMatrixRotationAxis(&rotX, &right, x);
        D3DXVec3TransformNormal(&lookAt, &lookAt, &rotX);
        D3DXVec3TransformNormal(&upVec, &upVec, &rotX);
    }

    if (y != 0.0f)
    {
        D3DXMatrixRotationAxis(&rotY, &upVec, y);
        D3DXVec3TransformNormal(&lookAt, &lookAt, &rotY);
        D3DXVec3TransformNormal(&upVec, &upVec, &rotY);
    }

    // calculate rotated matrix
    rotateMatrix *= rotZ * rotY * rotX;


    // Normalize Direction Vector
    D3DXVec3Normalize(&lookAt, &lookAt);
    D3DXVec3Normalize(&upVec, &upVec);
    D3DXVec3Cross(&right, &upVec, &lookAt);
}

void Object::SetMaterial() {
    ZeroMemory(&material, sizeof(D3DMATERIAL9));
    material.Diffuse = D3DXCOLOR(1, 1, 1, 1);  
    material.Ambient = D3DXCOLOR(0, 0, 0, 0);  
    material.Specular = D3DXCOLOR(0, 0, 0, 0); 
    material.Emissive = D3DXCOLOR(0, 0, 0, 0);
    material.Power = 0.f; 
}

void Object::LoadTexture(LPDIRECT3DDEVICE9 device, LPCWSTR texturePath) {

    HRESULT hr = D3DXCreateTextureFromFile(device, texturePath, &_texture);
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load texture", L"Error", MB_OK);
    }
}