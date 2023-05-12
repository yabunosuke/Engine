#pragma once
#include <Windows.h>
#include "Common/Property.h"
#include "Common/Singleton.h"

class Time final
	: public Singleton<Time>
{
public:
	friend class Singleton<Time>; // インスタンス作成を許可

	void InstrumentationStart();
	double InstrumentationEnd();
	bool CheckFixedUpdate()
	{
		return fixed_update_time_ >= fixed_delta_time_;
	}
	void AddFixedTimer()
	{
		// 固定長更新用タイマーに加算
		fixed_update_time_ += delta_time_;
	}
	void SubFixedTimer()
	{
		fixed_update_time_ -= fixed_delta_time_;
	}
	void ClearFixedTimer()
	{
		fixed_update_time_ = 0.0f;
	}
	// 最後のフレームから現在のフレームまでの経過秒数
	yEngine::ReadOnlyProperty<double> time
	{
		&delta_time_
	};

	yEngine::ReadOnlyProperty<double> deltaTime
	{
		&fixed_update_time_
	};
	// 固定フレームレートの更新を実行するインターバル
	yEngine::Property<double> fixedDeltaTime
	{
		&fixed_delta_time_
	};

	// 最初のシーンが起動してからの経過フレーム数（ReadOnly）
	yEngine::ReadOnlyProperty<int> frameCount
	{
		&frame_count_
	};
	// 最初のシーンが起動してからの経過秒数（float型）（ReadOnly）
	yEngine::ReadOnlyProperty<double> timeAsDouble
	{
		&time_
	};

private:

	Time();

	int frame_count_;					// 最初のシーンが起動してからの経過フレーム
	double time_;						// 最初のシーンが起動してからの経過時間（秒）

	double frame_rate_;					// 1フレームにかける時間
	double fixed_delta_time_;			// 固定長更新にかける時間
	LARGE_INTEGER frequency_;			// 周波数
	LARGE_INTEGER time_count_start_;	// 計測開始時間
	LARGE_INTEGER time_count_end_;		// 計測終了時間
	double delta_time_{0.0};					// 1フレームでの経過時間
	double fixed_update_time_;			// 固定長更新用タイマー
};

