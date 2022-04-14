#pragma once
#include <math.h>

struct Vector2
{
    float x;
    float y;

    Vector2 ()
    {
        x = 0.0f;
        y = 0.0f;
    }
    Vector2 (float xPos, float yPos)
    {
        x = xPos;
        y = yPos;
    }

    void normalizeXY ()
    {
        x = x / length ();
        y = y / length ();
    }

    float length () { return sqrtf (x * x + y * y); }

    Vector2 operator/ (const Vector2 &other)
    {
        Vector2 ret;
        ret.x = this->x / other.x;
        ret.y = this->y / other.y;
        return ret;
    }

    Vector2 operator/ (const float &other)
    {
        Vector2 ret;
        ret.x = this->x / other;
        ret.y = this->y / other;
        return ret;
    }

    Vector2 operator+ (const Vector2 &other)
    {
        Vector2 ret;
        ret.x = this->x + other.x;
        ret.y = this->y + other.y;
        return ret;
    }

    Vector2 operator* (const Vector2 &other)
    {
        Vector2 ret;
        ret.x = this->x * other.x;
        ret.y = this->y * other.y;
        return ret;
    }

    Vector2 operator- (const Vector2 &other)
    {
        Vector2 ret;
        ret.x = this->x - other.x;
        ret.y = this->y - other.y;
        return ret;
    }

    Vector2 operator* (const int &other)
    {
        Vector2 ret;
        ret.x = this->x * other;
        ret.y = this->y * other;
        return ret;
    }

    Vector2 operator* (const float &other)
    {
        Vector2 ret;
        ret.x = this->x * other;
        ret.y = this->y * other;

        return ret;
    }

    Vector2 operator* (const double &other)
    {
        Vector2 ret;
        ret.x = this->x * (float)other;
        ret.y = this->y * (float)other;

        return ret;
    }

    Vector2 &operator-= (const Vector2 &other)
    {

        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }
    Vector2 &operator+= (const Vector2 &other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }
    Vector2 &operator*= (const Vector2 &other)
    {
        this->x *= other.x;
        this->y *= other.y;
        return *this;
    }
    Vector2 &operator/= (const Vector2 &other)
    {
        this->x /= other.x;
        this->y /= other.y;
        return *this;
    }
    Vector2 &operator-= (const float &other)
    {
        this->x -= other;
        this->y -= other;
        return *this;
    }
    Vector2 &operator+= (const float &other)
    {
        this->x += other;
        this->y += other;
        return *this;
    }
    Vector2 &operator*= (const float &other)
    {
        this->x *= other;
        this->y *= other;
        return *this;
    }
    Vector2 &operator/= (const float &other)
    {
        this->x /= other;
        this->y /= other;
        return *this;
    }
    Vector2 &operator-= (const int &other)
    {
        this->x -= other;
        this->y -= other;
        return *this;
    }
    Vector2 &operator+= (const int &other)
    {
        this->x += other;
        this->y += other;
        return *this;
    }
    Vector2 &operator*= (const int &other)
    {
        this->x *= other;
        this->y *= other;
        return *this;
    }
    Vector2 &operator/= (const int &other)
    {
        this->x /= other;
        this->y /= other;
        return *this;
    }
};

struct Vector3
{
    float x;
    float y;
    float z;

    Vector3 ()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
    Vector3 (float xPos, float yPos, float zPos)
    {
        x = xPos;
        y = yPos;
        z = zPos;
    }

    void normalizeXY ()
    {
        x = x / magnitude ();
        y = y / magnitude ();
    }

    float magnitude () { return sqrtf (x * x + y * y); }

    float length () { return sqrtf (x * x + y * y + z * z); }

    Vector3 &operator-= (const Vector3 &other)
    {

        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }
    Vector3 &operator+= (const Vector3 &other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }
    Vector3 &operator*= (const Vector3 &other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
        return *this;
    }
    Vector3 &operator/= (const Vector3 &other)
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
        return *this;
    }
    Vector3 &operator-= (const float &other)
    {
        this->x -= other;
        this->y -= other;
        this->z -= other;
        return *this;
    }
    Vector3 &operator+= (const float &other)
    {
        this->x += other;
        this->y += other;
        this->z += other;
        return *this;
    }
    Vector3 &operator*= (const float &other)
    {
        this->x *= other;
        this->y *= other;
        this->z *= other;
        return *this;
    }
    Vector3 &operator/= (const float &other)
    {
        this->x /= other;
        this->y /= other;
        this->z /= other;
        return *this;
    }
    Vector3 &operator-= (const int &other)
    {
        this->x -= other;
        this->y -= other;
        this->z -= other;
        return *this;
    }
    Vector3 &operator+= (const int &other)
    {
        this->x += other;
        this->y += other;
        this->z += other;
        return *this;
    }
    Vector3 &operator*= (const int &other)
    {
        this->x *= other;
        this->y *= other;
        this->z *= other;
        return *this;
    }
    Vector3 &operator/= (const int &other)
    {
        this->x /= other;
        this->y /= other;
        this->z /= other;
        return *this;
    }

    Vector3 operator- (const Vector3 &other)
    {
        Vector3 ret;
        ret.x = this->x - other.x;
        ret.y = this->y - other.y;
        ret.z = this->z - other.z;
        return ret;
    }
    Vector3 operator+ (const Vector3 &other)
    {
        Vector3 ret;
        ret.x = this->x + other.x;
        ret.y = this->y + other.y;
        ret.z = this->z + other.z;
        return ret;
    }
    Vector3 operator* (const Vector3 &other)
    {
        Vector3 ret;
        ret.x = this->x * other.x;
        ret.y = this->y * other.y;
        ret.z = this->z * other.z;
        return ret;
    }
    Vector3 operator/ (const Vector3 &other)
    {
        Vector3 ret;
        ret.x = this->x / other.x;
        ret.y = this->y / other.y;
        ret.z = this->z / other.z;
        return ret;
    }
    Vector3 operator- (const float &other)
    {
        Vector3 ret;
        ret.x = this->x - other;
        ret.y = this->y - other;
        ret.z = this->z - other;
        return ret;
    }
    Vector3 operator+ (const float &other)
    {
        Vector3 ret;
        ret.x = this->x + other;
        ret.y = this->y + other;
        ret.z = this->z + other;
        return ret;
    }
    Vector3 operator* (const float &other)
    {
        Vector3 ret;
        ret.x = this->x * other;
        ret.y = this->y * other;
        ret.z = this->z * other;
        return ret;
    }
    Vector3 operator/ (const float &other)
    {
        Vector3 ret;
        ret.x = this->x / other;
        ret.y = this->y / other;
        ret.z = this->z / other;
        return ret;
    }
    Vector3 operator- (const int &other)
    {
        Vector3 ret;
        ret.x = this->x - other;
        ret.y = this->y - other;
        ret.z = this->z - other;
        return ret;
    }
    Vector3 operator+ (const int &other)
    {
        Vector3 ret;
        ret.x = this->x + other;
        ret.y = this->y + other;
        ret.z = this->z + other;
        return ret;
    }
    Vector3 operator* (const int &other)
    {
        Vector3 ret;
        ret.x = this->x * other;
        ret.y = this->y * other;
        ret.z = this->z * other;
        return ret;
    }
    Vector3 operator/ (const int &other)
    {
        Vector3 ret;
        ret.x = this->x / other;
        ret.y = this->y / other;
        ret.z = this->z / other;
        return ret;
    }
};