#include "Vector.hpp"


Vector2 Vector2::operator* (const Vector2 &other) const
{
    Vector2 ret;
    ret.x = this->x * other.x;
    ret.y = this->y * other.y;
    return ret;
}
Vector2 &Vector2::operator/= (const int &other)
{
    this->x /= other;
    this->y /= other;
    return *this;
}
Vector2 &Vector2::operator*= (const int &other)
{
    this->x *= other;
    this->y *= other;
    return *this;
}
Vector2 &Vector2::operator+= (const int &other)
{
    this->x += other;
    this->y += other;
    return *this;
}
Vector2 &Vector2::operator-= (const int &other)
{
    this->x -= other;
    this->y -= other;
    return *this;
}
Vector2 &Vector2::operator/= (const float &other)
{
    this->x /= other;
    this->y /= other;
    return *this;
}
Vector2 &Vector2::operator*= (const float &other)
{
    this->x *= other;
    this->y *= other;
    return *this;
}
Vector2 &Vector2::operator+= (const float &other)
{
    this->x += other;
    this->y += other;
    return *this;
}
Vector2 &Vector2::operator-= (const float &other)
{
    this->x -= other;
    this->y -= other;
    return *this;
}
Vector2 &Vector2::operator/= (const Vector2 &other)
{
    this->x /= other.x;
    this->y /= other.y;
    return *this;
}
Vector2 &Vector2::operator*= (const Vector2 &other)
{
    this->x *= other.x;
    this->y *= other.y;
    return *this;
}
Vector2 &Vector2::operator+= (const Vector2 &other)
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}
Vector2 &Vector2::operator-= (const Vector2 &other)
{

    this->x -= other.x;
    this->y -= other.y;
    return *this;
}
Vector2 Vector2::operator* (const double &other) const
{
    Vector2 ret;
    ret.x = this->x * other;
    ret.y = this->y * other;

    return ret;
}
Vector2 Vector2::operator* (const float &other) const
{
    Vector2 ret;
    ret.x = this->x * other;
    ret.y = this->y * other;

    return ret;
}
Vector2 Vector2::operator* (const int &other) const
{
    Vector2 ret;
    ret.x = this->x * other;
    ret.y = this->y * other;
    return ret;
}
Vector2 Vector2::operator- (const Vector2 &other) const
{
    Vector2 ret;
    ret.x = this->x - other.x;
    ret.y = this->y - other.y;
    return ret;
}
Vector2 Vector2::operator/ (const Vector2 &other) const
{
    Vector2 ret;
    ret.x = this->x / other.x;
    ret.y = this->y / other.y;
    return ret;
}
Vector3 &Vector3::operator/= (const int &other)
{
    this->x /= other;
    this->y /= other;
    this->z /= other;
    return *this;
}
Vector3 &Vector3::operator*= (const int &other)
{
    this->x *= other;
    this->y *= other;
    this->z *= other;
    return *this;
}
Vector3 &Vector3::operator+= (const int &other)
{
    this->x += other;
    this->y += other;
    this->z += other;
    return *this;
}
Vector3 &Vector3::operator-= (const int &other)
{
    this->x -= other;
    this->y -= other;
    this->z -= other;
    return *this;
}
Vector3 &Vector3::operator/= (const float &other)
{
    this->x /= other;
    this->y /= other;
    this->z /= other;
    return *this;
}
Vector3 &Vector3::operator*= (const float &other)
{
    this->x *= other;
    this->y *= other;
    this->z *= other;
    return *this;
}
Vector3 &Vector3::operator+= (const float &other)
{
    this->x += other;
    this->y += other;
    this->z += other;
    return *this;
}
Vector3 &Vector3::operator-= (const float &other)
{
    this->x -= other;
    this->y -= other;
    this->z -= other;
    return *this;
}
Vector3 &Vector3::operator/= (const Vector3 &other)
{
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
    return *this;
}
Vector3 &Vector3::operator*= (const Vector3 &other)
{
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
    return *this;
}
Vector3 &Vector3::operator+= (const Vector3 &other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}
Vector3 &Vector3::operator-= (const Vector3 &other)
{

    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}
Vector2 Vector2::operator+ (const Vector2 &other) const
{
    Vector2 ret;
    ret.x = this->x + other.x;
    ret.y = this->y + other.y;
    return ret;
}
Vector2 Vector2::operator/ (const float &other) const
{
    Vector2 ret;
    ret.x = this->x / other;
    ret.y = this->y / other;
    return ret;
}
Vector3 Vector3::operator/ (const int &other) const
{
    Vector3 ret;
    ret.x = this->x / other;
    ret.y = this->y / other;
    ret.z = this->z / other;
    return ret;
}Vector3 Vector3::operator* (const int &other) const
{
    Vector3 ret;
    ret.x = this->x * other;
    ret.y = this->y * other;
    ret.z = this->z * other;
    return ret;
}Vector3 Vector3::operator+ (const int &other) const
{
    Vector3 ret;
    ret.x = this->x + other;
    ret.y = this->y + other;
    ret.z = this->z + other;
    return ret;
}Vector3 Vector3::operator- (const int &other) const
{
    Vector3 ret;
    ret.x = this->x - other;
    ret.y = this->y - other;
    ret.z = this->z - other;
    return ret;
}Vector3 Vector3::operator/ (const float &other) const
{
    Vector3 ret;
    ret.x = this->x / other;
    ret.y = this->y / other;
    ret.z = this->z / other;
    return ret;
}Vector3 Vector3::operator* (const float &other) const
{
    Vector3 ret;
    ret.x = this->x * other;
    ret.y = this->y * other;
    ret.z = this->z * other;
    return ret;
}Vector3 Vector3::operator+ (const float &other) const
{
    Vector3 ret;
    ret.x = this->x + other;
    ret.y = this->y + other;
    ret.z = this->z + other;
    return ret;
}Vector3 Vector3::operator- (const float &other) const
{
    Vector3 ret;
    ret.x = this->x - other;
    ret.y = this->y - other;
    ret.z = this->z - other;
    return ret;
}Vector3 Vector3::operator/ (const Vector3 &other) const
{
    Vector3 ret;
    ret.x = this->x / other.x;
    ret.y = this->y / other.y;
    ret.z = this->z / other.z;
    return ret;
}Vector3 Vector3::operator* (const Vector3 &other) const
{
    Vector3 ret;
    ret.x = this->x * other.x;
    ret.y = this->y * other.y;
    ret.z = this->z * other.z;
    return ret;
}Vector3 Vector3::operator+ (const Vector3 &other) const
{
    Vector3 ret;
    ret.x = this->x + other.x;
    ret.y = this->y + other.y;
    ret.z = this->z + other.z;
    return ret;
}Vector3 Vector3::operator- (const Vector3 &other) const
{
    Vector3 ret;
    ret.x = this->x - other.x;
    ret.y = this->y - other.y;
    ret.z = this->z - other.z;
    return ret;
}