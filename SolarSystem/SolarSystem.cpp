// SampleSolarSystem.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <Windows.h>
#include <d3d9.h>
#include "GameTimer.h"
#include "CCamera.h"
#include "Planet.h"
#include "Sun.h"

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3dDevice = NULL;

GameTimer* timer;
CCamera* camera;

Sun* sun;
Planet* earth;
Planet* moon;



// D3D 객체와 Device 객체 생성
HRESULT InitD3D(HWND hWnd)
{
    // D3D 객체 생성
    D3DPRESENT_PARAMETERS d3dpp;
    if (NULL == (d3d = Direct3DCreate9(D3D_SDK_VERSION)))
        return E_FAIL;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.BackBufferCount = 2;

    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;


    // D3D Device 객체 생성
    if (d3dDevice != NULL) d3dDevice->Release();
    if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, &d3dDevice))) {
        return E_FAIL;
    }




    // 카메라의 초기 목표 설정
    return S_OK;
}

VOID Cleanup()
{
    if (d3dDevice != NULL)
        d3dDevice->Release();
    if (d3d != NULL)
        d3d->Release();
}


LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static bool isActive;
    switch (msg)
    {
    case WM_DESTROY:
        // 윈도우 종료 시 D3D 객체 및 디바이스 해제
        Cleanup();
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        switch (wParam) {

        }
        break;
    case WM_SIZE:
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
        // WM_PAINT 발생 시 D3D Render Sequence 실행
        // validate window rect = didn't loop WM_PAINT
        ValidateRect(hWnd, NULL);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Render() {

    if (NULL == d3dDevice)
        return;
    // Clear Device
    d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 1);

    // Lock d3d Device
    if (SUCCEEDED(d3dDevice->BeginScene()))
    {

        D3DXMATRIX tempTM;
        D3DXMatrixIdentity(&tempTM);

        // Projection Matrix 설정
        D3DXMATRIX tempProjection;
        D3DXMatrixPerspectiveFovLH(&tempProjection, D3DXToRadian(60.f), 1.f, 0.01f, 100.f);
        d3dDevice->SetTransform(D3DTS_PROJECTION, &tempProjection);

        // View Matrix를 카메라의 설정에 맞게 설정
        camera->SetViewTransform(d3dDevice);

        // Z 버퍼 활성화
        d3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
        d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        d3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);


        // 광원을 사용하지 않음으로 vertex의 rgb 값만을 출력에 사용.
        d3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
        d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

        d3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
        d3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

        d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

        sun->Render(d3dDevice);
        earth->Render(d3dDevice);
        moon->Render(d3dDevice);
        // Unlock d3d Device
        d3dDevice->EndScene();

    }
    // Flip Device buffer
    d3dDevice->Present(NULL, NULL, NULL, NULL);
}


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
    // 윈도우 클래스 등록
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      L"Solar System", NULL };
    RegisterClassEx(&wc);
    // 윈도우 생성
    HWND hWnd = CreateWindow(L"Solar System", L"Solar System",
        WS_OVERLAPPEDWINDOW, 100, 100, 1000, 1000,
        GetDesktopWindow(), NULL, wc.hInstance, NULL);
    
    // Direct3D 초기화
    // D3D 초기화 성공 시 윈도우 출력 및 메시지 루프
    if (SUCCEEDED(InitD3D(hWnd)))
    {
        // 타이머, 카메라 생성
        timer = new GameTimer();
        camera = new CCamera(0, 15, -15);
        camera->SetTargetVector(-camera->GetPos());

        sun = new Sun(0, 0, 0, 20, 20, 1.f);
        sun->Init(d3dDevice);
        sun->LoadTexture(d3dDevice, L"sunmap.jpg");
        earth = new Planet(2, 0, 0, 20, 20, 0.1f);
        earth->Init(d3dDevice);
        earth->SetOriginalStar(sun);

        earth->LoadTexture(d3dDevice, L"earthmap1k.jpg");
        earth->SetValue(1, 0.00274, 7);

        moon = new Planet(3.5, 0, 0, 20, 20, 0.05f);
        moon->Init(d3dDevice);
        moon->SetOriginalStar(earth);
        moon->LoadTexture(d3dDevice, L"2k_moon.jpg");
        moon->SetValue(0.0366, 0.0366, 1.5f);

        // 윈도우 출력
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);
        // 메시지 루프
        MSG msg;
        float deltaT = 0;
        while (true) {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else {
                // 60프레임으로 작동
                deltaT += timer->getDeltaTime();
                if (deltaT >= 1.f / 60.f) {
                    camera->Update(deltaT);
                    camera->KeyInput(deltaT);
                    Render();
                    sun->Update(deltaT);
                    earth->Update(deltaT);
                    moon->Update(deltaT);
                    deltaT = 0;

                }
            }
        }

        delete timer;
        delete camera;
        delete sun;
        delete earth;
        delete moon;
    }
    // 메시지 루프 종료 = PostQuitMessage() or D3D 초기화 실패 시 등록된 WNDCLASSEX 소거 및 프로그램 종료
    // 등록된 클래스 소거
    UnregisterClass(L"Solar System", wc.hInstance);
    return 0;
}