#include <windows.h>

class GameTimer {
private:
    LARGE_INTEGER frequency;  // �ʴ� ī��Ʈ Ƚ��
    LARGE_INTEGER lastTime;   // ���� ������ �ð�

public:
    GameTimer() {
        // Ÿ�̸� ���ļ� ��������
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&lastTime); // �ʱ� �ð� ����
    }

    float getDeltaTime() {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime); // ���� �ð� ��������

        // ��� �ð� ��� (�� ����)
        float deltaTime = static_cast<float>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;

        // ���� �ð��� ���� �ð����� ������Ʈ
        lastTime = currentTime;

        return deltaTime;
    }
};
#pragma once
