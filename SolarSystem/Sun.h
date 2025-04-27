#pragma once
#include "Sphere.h"
#include "Star.h"

class Sun : public Sphere, public Star{
public:
	Sun() : Sphere() {};
	Sun(float x, float y, float z, int slices = 20, int stacks = 20, float radius = 1.f) : Sphere(x, y, z, slices, stacks, radius) {
        rotationSpd = 0.5f;
        
	}

    void Render(LPDIRECT3DDEVICE9 device) override {
        Sphere::Render(device);
        SetLight(device);
    }

    void SetLight(LPDIRECT3DDEVICE9 device) {
        D3DLIGHT9 light;
        ZeroMemory(&light, sizeof(D3DLIGHT9));

        light.Type = D3DLIGHT_POINT; 
        light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); 
        light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); 
        light.Ambient = D3DXCOLOR(.2f, .2f, .2f, 1.0f);

        light.Position = pos;
        light.Range = 1000000.0f; // ���� ����
        light.Attenuation0 = 0.0f; // �Ÿ��� ���� ���� X
        light.Attenuation1 = 0.0f; 
        light.Attenuation2 = 0.0f; 

        // ����Ʈ�� ��ġ�� ����
        device->SetLight(0, &light); // ù ��° ����Ʈ ���� (�ε��� 0)
        device->LightEnable(0, TRUE); // ����Ʈ�� Ȱ��ȭ

    }

    void SetMaterial() override {

        ZeroMemory(&material, sizeof(D3DMATERIAL9));
        material.Diffuse = D3DXCOLOR(1, 1, 1, 1);  
        material.Ambient = D3DXCOLOR(1,1,1,1);  
        material.Emissive = D3DXCOLOR(1,1,1,1); 
        material.Power = 0.f; 
    }

    void Update(float deltaT) override {
        Rotation(deltaT);
    }

    void Rotation(float deltaT) override {
        Rotate(0, -rotationSpd * deltaT, 0);
    }

private:
	_D3DLIGHT9 light;
};