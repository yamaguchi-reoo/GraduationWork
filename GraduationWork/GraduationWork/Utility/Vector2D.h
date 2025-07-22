#pragma once

class Vector2D
{
public:
	float x;
	float y;

public:
	//コンストラクタ
	Vector2D();
	Vector2D(float scalar);
	Vector2D(float mx, float my);
	//デストラクタ
	~Vector2D();

public:
	//代入
	Vector2D& operator = (const Vector2D& location);

	//加算
	const Vector2D operator + (const Vector2D& location)const;
	//自分加算
	Vector2D& operator += (const Vector2D& location);


	//減算
	const Vector2D operator - (const Vector2D& location)const;
	//自分に減算
	Vector2D& operator -= (const Vector2D& location);


	//乗算(スカラー)
	const Vector2D operator * (const float& scalar)const;
	//乗算
	const Vector2D operator * (const Vector2D& location)const;
	//自分に乗算(スカラー)
	Vector2D& operator *= (const float& scalor);
	//自分に乗算
	Vector2D& operator *= (const Vector2D& location);


	//除算(スカラー)
	const Vector2D operator / (const float& scalar)const;
	//除算
	const Vector2D operator / (const Vector2D& location)const;
	//自分に除算(スカラー)
	Vector2D& operator /= (const float& scalor);
	//自分に除算
	Vector2D& operator /= (const Vector2D& location);

	//整数型に変換
	void ToInt(int* x, int* y)const;

	float Length()const;

	Vector2D Normalize()const;
};

