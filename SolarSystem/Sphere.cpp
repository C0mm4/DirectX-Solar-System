#include <Windows.h>
#include <iostream>
#include "Sphere.h"


HRESULT Sphere::Init(LPDIRECT3DDEVICE9 device) {
    // ��ü ���ؽ� ����

    vertexCount = (slices + 1) * (stacks + 1);
    int indexCount = slices * stacks * 6; // �� ���� �ﰢ�� = 6�� �ε���
    polygonCount = slices * stacks * 2;

    // Vertex Buffer ����
    device->CreateVertexBuffer(vertexCount * sizeof(CUSTOMVERTEX), 0, 0, D3DPOOL_MANAGED, &_vertexes, nullptr);

    // Index Buffer ����
    device->CreateIndexBuffer(indexCount * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &_indicies, nullptr);

    // Vertex Buffer ä���
    CUSTOMVERTEX* vertices = nullptr;
    _vertexes->Lock(0, 0, (void**)&vertices, 0);


    int v = 0;
    for (int stack = 0; stack <= stacks; ++stack) {
        float phi = D3DX_PI * stack / stacks; // 0 ~ pi

        for (int slice = 0; slice <= slices; ++slice) {
            float theta = 2.0f * D3DX_PI * (float)slice / (float)slices; // 0 ~ 2pi

            D3DXVECTOR3 pos;
            pos.x = radius * sinf(phi) * cosf(theta);  // X��
            pos.y = radius * cosf(phi);                // Y��
            pos.z = radius * sinf(phi) * sinf(theta);  // Z��
            D3DXVec3Normalize(&vertices[v].normal, &pos);  // ��� ���� ���

            vertices[v].pos = pos;

            float u = (float)slice / slices;  // �ؽ�ó ��ǥ u (0 ~ 1)
            float vC = (float)stack / stacks;  // �ؽ�ó ��ǥ v (0 ~ 1)

            vertices[v].uv.x = u;
            vertices[v].uv.y = vC;
            v++;
        }
    }
    _vertexes->Unlock();

    // Index Buffer ä���
    WORD* indices = nullptr;
    _indicies->Lock(0, 0, (void**)&indices, 0);

    int i = 0;
    for (int stack = 0; stack < stacks; ++stack) {
        for (int slice = 0; slice < slices; ++slice) {
            WORD first = (WORD)(stack * (slices + 1) + slice);
            WORD second = (WORD)(first + slices + 1);

            // �ﰢ�� 1
            indices[i++] = first;
            indices[i++] = second;
            indices[i++] = first + 1;

            // �ﰢ�� 2
            indices[i++] = second;
            indices[i++] = second + 1;
            indices[i++] = first + 1;
        }
    }
    _indicies->Unlock();

    SetMaterial();

    return S_OK;
}