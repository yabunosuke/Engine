#include "Time.h"
#include <thread>

Time::Time():

	frame_count_(0),             // 経過フレーム初期化
	time_(0),                    // 経過時間初期化

	fixed_delta_time_(0.02),     // 0.02秒ごとにFixedUpdate
	fixed_update_time_(0.0),

	frame_rate_(1.0f / 60.0f)
{

}

void Time::InstrumentationStart()
{
    // 周波数取得
    QueryPerformanceFrequency(&frequency_);
    // 計測開始
    QueryPerformanceCounter(&time_count_start_);
}

double Time::InstrumentationEnd()
{
    // 現在時間を取得
    QueryPerformanceCounter(&time_count_end_);
    // 経過時間計測
    delta_time_ = static_cast<double>(time_count_end_.QuadPart - time_count_start_.QuadPart) / static_cast<double>(frequency_.QuadPart);

    // 経過秒数加算
    time_ += delta_time_;
    // 経過フレーム数増加
    ++frame_count_;



    return delta_time_;
}
