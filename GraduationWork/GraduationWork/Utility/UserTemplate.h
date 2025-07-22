#pragma once

//絶対値を求める関数テンプレート
template<typename T>
T Abs(T value)
{
	T result;
	//正の値の場合、そのまま返す
	if (value > 0)
	{
		result = value;
	}
	// 負の値の場合、その絶対値を返す
	else
	{
		result = -value;
	}

	return result;
}

//最大値を求める関数テンプレート
template <typename T>
T Max(T a, T b)
{
	T result;
	//bがaより大きい場合はbを返す
	if (a < b)
	{
		result = b;
	}
	//aがb以上の場合はaを返す
	else
	{
		result = a;
	}

	return result;
}

//最小値を求める関数テンプレート
template<typename T>
T Min(T a, T b)
{
	T result;
	//bがaより小さい場合はbを返す
	if (a > b)
	{
		result = b;
	}
	//aがb以下の場合はaを返す
	else
	{
		result = a;
	}

	return result;
}
template<typename T>
T Clamp(T value, T min, T max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

