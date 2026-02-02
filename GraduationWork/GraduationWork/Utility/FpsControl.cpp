#include "FpsControl.h"
#include "DxLib.h"

void FpsControl::Initialize()
{
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&prev_time);
    frame_duration = 1.0f / target_fps; 
}

void FpsControl::Update()
{
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    float delta = static_cast<float>(now.QuadPart - prev_time.QuadPart) / frequency.QuadPart;


    if (delta < frame_duration)
    {
        int wait_ms = static_cast<int>((frame_duration - delta) * 1000.0f);
        if (wait_ms > 0)
        {
            WaitTimer(wait_ms);
        }

        while (true)
        {
            QueryPerformanceCounter(&now);
            delta = static_cast<float>(now.QuadPart - prev_time.QuadPart) / frequency.QuadPart;
            if (delta >= frame_duration)
                break;
        }
    }

    prev_time = now;

    fps_timer += delta;
    frame_count++;

    if (fps_timer >= 1.0f)
    {
        current_fps = frame_count / fps_timer;
        fps_timer = 0.0f;
        frame_count = 0;
    }
}


void FpsControl::Draw(int x, int y) const
{
   //DrawFormatString(x, y, GetColor(255, 255, 255), "FPS: %.1f", current_fps);
}

float FpsControl::GetFps() const
{
    return current_fps;
}