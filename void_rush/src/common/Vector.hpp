#pragma once
#include <corecrt_math.h>
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

    float length ()const { return sqrtf (x * x + y * y); }

    Vector2 operator/ (const Vector2 &other) const;
    Vector2 operator/ (const float &other) const;
    Vector2 operator+ (const Vector2 &other) const;
    Vector2 operator* (const Vector2 &other) const;
    Vector2 operator- (const Vector2 &other) const;
    Vector2 operator* (const int &other) const;
    Vector2 operator* (const float &other) const;
    Vector2 operator* (const double &other) const;

    Vector2 &operator-= (const Vector2 &other);
    Vector2 &operator+= (const Vector2 &other);
    Vector2 &operator*= (const Vector2 &other);
    Vector2 &operator/= (const Vector2 &other);
    Vector2 &operator-= (const float &other);
    Vector2 &operator+= (const float &other);
    Vector2 &operator*= (const float &other);
    Vector2 &operator/= (const float &other);
    Vector2 &operator-= (const int &other);
    Vector2 &operator+= (const int &other);
    Vector2 &operator*= (const int &other);
    Vector2 &operator/= (const int &other);
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
    static Vector3 normalize(Vector3 vec){
        Vector3 ret = Vector3(0.0f, 0.0f, 0.0f);
        float len = vec.length();
        if(len != 0){
            ret.x = vec.x/len;
            ret.y = vec.y/len;
            ret.z = vec.z/len;
        }
        return ret;
    }

    float magnitude ()const { return sqrtf (x * x + y * y); }

    float length ()const { return sqrtf (x * x + y * y + z * z); }
    float magnitude ()const { return sqrtf (x * x + y * y); }
    static float magnitude(Vector2 vec){return sqrtf(vec.x*vec.x + vec.y*vec.y); }
    float length ()const { return sqrtf (x * x + y * y + z * z); }
    static float lenght(Vector3 vec){return sqrtf(vec.x*vec.x + vec.y*vec.y +vec.z*vec.z); }

    Vector3 &operator-= (const Vector3 &other);
    Vector3 &operator+= (const Vector3 &other);
    Vector3 &operator*= (const Vector3 &other);
    Vector3 &operator/= (const Vector3 &other);
    Vector3 &operator-= (const float &other);
    Vector3 &operator+= (const float &other);
    Vector3 &operator*= (const float &other);
    Vector3 &operator/= (const float &other);
    Vector3 &operator-= (const int &other);
    Vector3 &operator+= (const int &other);
    Vector3 &operator*= (const int &other);
    Vector3 &operator/= (const int &other);

    Vector3 operator- (const Vector3 &other) const;
    Vector3 operator+ (const Vector3 &other) const;
    Vector3 operator* (const Vector3 &other) const;
    Vector3 operator/ (const Vector3 &other) const;
    Vector3 operator- (const float &other) const;
    Vector3 operator+ (const float &other) const;
    Vector3 operator* (const float &other) const;
    Vector3 operator/ (const float &other) const;
    Vector3 operator- (const int &other) const;
    Vector3 operator+ (const int &other) const;
    Vector3 operator* (const int &other) const;
    Vector3 operator/ (const int &other) const;
};