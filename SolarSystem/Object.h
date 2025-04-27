#pragma once
#include <d3d9.h>
#include <d3dx9math.h>



// ���̷�Ʈ3 ������ ���� ���� ����ü
struct CUSTOMVERTEX
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
};

// ���̷�Ʈ3 ���� ���� ���� ����ü
struct LineVertex {
	D3DXVECTOR3 pos;
	DWORD color;
};


// ������ ���� FVF ����
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

// ���� ���� FVF ����
#define CUSTOMFVF_LINEVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)


class Object {
protected:
	LPDIRECT3DVERTEXBUFFER9 _vertexes;	// ���� ����
	LPDIRECT3DINDEXBUFFER9 _indicies;	// �ε��� ����
	D3DXMATRIX worldMatrix;				// ��ü ���� ���� ��Ʈ����
	D3DXMATRIX rotateMatrix;			// ��ü ���� ȸ�� ��Ʈ����
	D3DXVECTOR3 pos;					// ��ü ��ġ ����
	LPDIRECT3DTEXTURE9 _texture;		// �ؽ���
	D3DMATERIAL9 material;				// ��Ƽ����
	int vertexCount;					// ���� ����
	int polygonCount;					// ������ ����


	D3DXVECTOR3 upVec, lookAt, right;	// ��ü�� up, front, right ����


public:
	Object();

	Object(float x, float y, float z);

	virtual ~Object() {
		// ��ü delete �� ���� ����, �ε��� ���� Release
		if (_vertexes != NULL) _vertexes->Release();
		if (_indicies != NULL) _indicies->Release();
	}

	/// <summary>
	/// ��ü�� �� �����Ӻ� ���� main�� �޽��� �������� Update ȣ��
	/// </summary>
	/// <param name="deltaT">���� �����Ӱ��� �ð�</param>
	virtual void Update(float deltaT) {}; 

	/// <summary>
	/// ��ü�� �ʱ�ȭ ����. ���� ���� �� �ʿ��� �۾� ��ü �� ����
	/// </summary>
	/// <param name="device">Direct3 ����̽� ��ü ������</param>
	/// <returns>�ʱ�ȭ ���� ����</returns>
	virtual HRESULT Init(LPDIRECT3DDEVICE9 device)=0;	

	/// <summary>
	/// ��ü�� ������ ����. 
	/// </summary>
	/// <param name="device">DirectX ����̽� ��ü ������</param>
	virtual void Render(LPDIRECT3DDEVICE9 device);

	/// <summary>
	/// ��ü�� ��ġ�� �Ű�������ŭ �̵� �̵��� ��ü�� local coordinate�� ����
	/// </summary>
	/// <param name="x">x value</param>
	/// <param name="y">y value</param>
	/// <param name="z">z value</param>
	virtual void Move(float x, float y, float z);

	/// <summary>
	/// ��ü�� ��ġ�� �Ű�������ŭ �̵� �̵��� world coordinate�� ���� 
	/// </summary>
	/// <param name="x">x value</param>
	/// <param name="y">y value</param>
	/// <param name="z">z value</param>
	virtual void MoveInWorld(float x, float y, float z);

	/// <summary>
	/// ��ü�� �Ű�������ŭ ȸ�� ȸ���� ��ü�� local matrix�� ����
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	virtual void Rotate(float x, float y, float z);

	/// <summary>
	/// ��ü�� ��ġ�� ��ȯ
	/// </summary>
	/// <returns>��ġ Vector</returns>
	D3DXVECTOR3 GetPos() { return pos; }

	/// <summary>
	/// ��ü������ ��ġ�� ���
	/// </summary>
	/// <param name="obj">Ÿ�� ������Ʈ</param>
	/// <returns>��ü���� �Ÿ� Vector</returns>
	D3DXVECTOR3 ComparePos(const Object& obj) { return obj.pos - pos; }

	/// <summary>
	/// ��ü�� �ٶ󺸴� ������ ��ȯ
	/// </summary>
	/// <returns>��ü�� LookAt Vector</returns>
	D3DXVECTOR3 GetLookAtVector() { return lookAt; }

	/// <summary>
	/// ��ü�� Ű �Է��� ����
	/// </summary>
	/// <param name="deltaT">Frame Time</param>
	virtual void KeyInput(float deltaT) {};

	/// <summary>
	/// ��Ƽ������ �����ϴ� �Լ�
	/// </summary>
	virtual void SetMaterial();

	/// <summary>
	/// �ؽ��ĸ� �ε��ϴ� �Լ�
	/// </summary>
	/// <param name="device">Direct ����̽� ��ü ������</param>
	/// <param name="texturePath">�ؽ��� ���</param>
	virtual void LoadTexture(LPDIRECT3DDEVICE9 device, LPCWSTR texturePath);
};