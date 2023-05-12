#pragma once
#include <iostream>
#include <limits>

namespace Mathf
{

	// イプシロン
	inline static constexpr float epsilon{FLT_EPSILON};
	// 円周率
	inline static const float pi = 3.14159265358979f;
	// 正の無限
	inline static const float infinity = std::numeric_limits<float>::infinity();
	// 負の無限
	inline static const float negative_infinity = -std::numeric_limits<float>::infinity();
	// 度からラジアンへ変換する定数
	inline static const float deg_to_rad = pi / 180.0f;
	// ラジアンから度へ変換する定数
	inline static const float rad_to_deg = 180.0f / pi;

	/// <summary>
	/// 二つの値の差が許容範囲内にあるかチェックする
	/// </summary>
	/// <param name="tolerance">許容範囲</param>
	/// <returns>bool 近似</returns>
	inline static bool Approximately(const float &lhs, const float &rhs, const float &tolerance = epsilon)
	{
		float difference = fabsf(lhs - rhs);
		if (difference < tolerance)
		{
			return true;
		}
		return false;
	}


	/// <summary>
	/// 値を範囲内に収める
	/// </summary>
	/// <param name="value">制限する値</param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>float 計算結果</returns>
	inline static float Clamp(const float &value, const float &min, const float &max)
	{
		if (value > max)
		{
			return max;
		}
		else if (value < min)
		{
			return min;
		}

		return value;
	};

	/// <summary>
	/// 目標値まで指定した移動量で移動する。
	/// </summary>
	/// <param name="current">現在値</param>
	/// <param name="target">目標値</param>
	/// <param name="max_delta">移動量</param>
	/// <returns>float 移動結果</returns>
	inline  static float MoveTowards(const float &current, const float &target, const float &max_delta)
	{
		if (current + max_delta < target)
		{
			return current + max_delta;
		}
		return  target;
	}

	/// <summary>
	/// 二つ値の中から最大値を返却する
	/// </summary>
	/// <returns>float 最大値</returns>
	inline static float Max(const float &lhs, const float &rhs)
	{
		return lhs > rhs ? lhs : rhs;
	}

	/// <summary>
	/// 複数の値の中から最大値を返却する
	/// </summary>
	/// <returns>float 最大値</returns>
	template<class  ...Args>
	inline static float Max(float a,float b,Args ...args)
	{
		float max = Max(a,b);
		for (float i : std::initializer_list<float>{ args... }) {
			max = Max(max, i);
		}
		return max;
	}


	/// <summary>
	/// 二つ値の中から最小値を返却する
	/// </summary>
	/// <returns>float 最大値</returns>
	inline static float Min(const float &lhs, const float &rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}

	/// <summary>
	/// 複数の値の中から最小値を返却する
	/// </summary>
	/// <returns>float 最大値</returns>
	template<class  ...Args>
	inline static float Min(float a, float b, Args ...args)
	{
		float min = Min(a, b);
		for (float i : std::initializer_list<float>{ args... }) {
			min = Min(min,i);
		}
		return min;
	}

};