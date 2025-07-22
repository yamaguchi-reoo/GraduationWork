#include "Vector2D.h"
#include <iostream>
#include <algorithm>

Vector2D::Vector2D() :x(0.0f), y(0.0f)
{
}

Vector2D::Vector2D(float scalar) :x(scalar), y(scalar)
{
}

Vector2D::Vector2D(float mx, float my) :x(mx), y(my)
{
}

Vector2D::~Vector2D()
{
}

Vector2D& Vector2D::operator=(const Vector2D& location)
{
    this->x = location.x;
    this->y = location.y;

    return *this;
}

const Vector2D Vector2D::operator+(const Vector2D& location) const
{
    Vector2D result = Vector2D(0.0f);

    result.x = this->x + location.x;
    result.y = this->y + location.y;

    return result;
}

Vector2D& Vector2D::operator+=(const Vector2D& location)
{
    this->x += location.x;
    this->y += location.y;

    return *this;
}

const Vector2D Vector2D::operator-(const Vector2D& location) const
{
    Vector2D result = Vector2D(0.0f);

    result.x = this->x - location.x;
    result.y = this->y - location.y;

    return result;
}

Vector2D& Vector2D::operator-=(const Vector2D& location)
{
    this->x -= location.x;
    this->y -= location.y;

    return *this;
}

const Vector2D Vector2D::operator*(const float& scalar) const
{
    Vector2D result = Vector2D(0.0f);

    result.x = this->x * scalar;
    result.y = this->y * scalar;

    return result;
}

const Vector2D Vector2D::operator*(const Vector2D& location) const
{
    Vector2D result = Vector2D(0.0f);

    result.x = this->x * location.x;
    result.y = this->y * location.y;

    return result;
}

Vector2D& Vector2D::operator*=(const float& scalar)
{
    this->x *= scalar;
    this->y *= scalar;

    return *this;
}

Vector2D& Vector2D::operator*=(const Vector2D& location)
{
    this->x *= location.x;
    this->y *= location.y;

    return *this;
}

const Vector2D Vector2D::operator/(const float& scalar) const
{
    //値がほぼ0の場合は、結果を0ベクトルにする
    if (abs(scalar) < 1e-6f) {
        return Vector2D(0.0f);
    }

    return Vector2D(this->x / scalar, this->y / scalar);
}

const Vector2D Vector2D::operator/(const Vector2D& location) const
{
    //値がほぼ0の場合は、結果を0ベクトルにする
    if ((abs(location.x) < 1e-6f || abs(location.y) < 1e-6f)) {
        return Vector2D(0.0f);
    }

    return Vector2D(this->x / location.x, this->y / location.y);
}

Vector2D& Vector2D::operator/=(const float& scalar)
{
    //値がほぼ0の場合は、結果を0ベクトルにする
    if (abs(scalar) < 1e-6f)
    {
        this->x = 0.0f;
        this->y = 0.0f;
    }
    else
    {
        this->x /= scalar;
        this->y /= scalar;
    }

    return *this;
}

Vector2D& Vector2D::operator/=(const Vector2D& location)
{
    //値がほぼ0の場合は、結果を0ベクトルにする
    if ((abs(location.x) < 1e-6f || abs(location.y) < 1e-6f))
    {
        this->x = 0.0f;
        this->y = 0.0f;
    }
    else
    {
        this->x /= location.x;
        this->y /= location.y;
    }

    return *this;
}

void Vector2D::ToInt(int* x, int* y) const
{
    //整数に変換
    *x = static_cast<int>(this->x);
    *y = static_cast<int>(this->y);
}

float Vector2D::Length() const
{
    return std::sqrt(x * x + y * y);
}

Vector2D Vector2D::Normalize() const
{
    float len = Length();
    if (len < 1e-6f) return Vector2D(0.0f);
    return *this / len;
}
