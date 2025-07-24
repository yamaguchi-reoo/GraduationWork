#pragma once
#include <windows.h>

class FpsControl
{
public:
    void Initialize();
    void Update();
    void Draw(int x = 10, int y = 10) const;
    float GetFps() const;

private:
    float target_fps = 60.0f;           // �Œ肵����FPS
    float frame_duration = 1.0f / 60.0f; // 1�t���[���̕b���i��F60FPS -> ��0.01666�b�j

    LARGE_INTEGER frequency;
    LARGE_INTEGER prev_time;
    float fps_timer = 0.0f;
    int frame_count = 0;
    float current_fps = 0.0f;
};