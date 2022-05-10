#include "Vec.h"
void arrayToVec(std::array<float, 3> arr[3], vec3 vec[3])
{
	for (int i = 0; i < 3; i++) {
		vec[i].x = arr[i][0];
		vec[i].y = arr[i][1];
		vec[i].z = arr[i][2];
	}
}
void arrayToVec(float arr[3], vec3 &vec)
{
	vec.x = arr[0];
	vec.y = arr[1];
	vec.z = arr[2];
}
void arrayToVec(float arr[2], vec2 &vec)
{
	vec.x = arr[0];
	vec.y = arr[1];
}
void arrayToVec(std::array<float, 2> arr[3], vec2 vec[3])
{
	for (int i = 0; i < 3; i++) {
		vec[i].x = arr[i][0];
		vec[i].y = arr[i][1];
	}
}
//git
vec3 vec3::Normalize() //TODO: should not set and return... confusing
{
	double i = sqrt(
		(double)x * (double)x +
		(double)y * (double)y +
		(double)z * (double)z);
	if (i == 0)
	{
		return vec3(0, 0, 0);
	}
	this->x = (float)(x / i);
	this->y = (float)(y / i);
	this->z = (float)(z / i);
	return vec3(x, y, z);
}
vec3 vec3::Normalize(const vec3& ref)
{
	vec3 ret;
	double i = sqrt(
		(double)ref.x * (double)ref.x +
		(double)ref.y * (double)ref.y +
		(double)ref.z * (double)ref.z);
	if (i == 0)
	{
		return vec3(0, 0, 0);
	}
	ret.x = (float)(ref.x / i);
	ret.y = (float)(ref.y / i);
	ret.z = (float)(ref.z / i);
	return ret;
}

vec3 vec3::X(const vec3& other)
{
	float a = (this->y * other.z) - (this->z * other.y);
	float b = (this->z * other.x) - (this->x * other.z);
	float c = (this->x * other.y) - (this->y * other.x);
	return vec3(a, b, c);
}

float vec3::operator*(vec3& other)
{
	return(this->x * other.x + this->y * other.y + this->z * other.z);
}

vec3 vec3::operator*(float other)
{
	return vec3(other * this->x, other * this->y, other* this->z);
}

vec3 vec3::mul(float other)
{
	return vec3(other * this->x, other * this->y, other * this->z);
}

vec3 vec3::mul(vec3 other)
{
	return vec3(other.x * this->x, other.y * this->y, other.z * this->z);
}

vec3 vec3::operator/(vec3 other)
{
	return vec3(other.x / this->x, other.y / this->y, other.z / this->z);
}

vec3 vec3::operator/(float other)
{
	return vec3(this->x / other, this->y / other, this->z / other);
}

vec3 vec3::operator+(vec3 other)
{
	return vec3(this->x + other.x, this->y + other.y, this->z + other.z);
}

vec3 vec3::operator-(vec3 other)
{
	return vec3(this->x - other.x, this->y - other.y, this->z - other.z);
}

float vec3::angle(vec3 other)
{
	vec3 a = Normalize(*this);
	vec3 b = Normalize(other);
	vec3 Vmin = a.X(b);
	float min = Vmin.x < Vmin.y ? Vmin.x : Vmin.y;
	min = min < Vmin.z ? min : Vmin.z;
	if (min < 0.0) {
		return (float)(- acos(a * b));
	}
	return (float)acos(a * b);
}

float vec3::angleNM(vec3 other)
{
	vec3 a = Normalize(*this);
	vec3 b = Normalize(other);
	return (float)acos(a * b);
}

float vec3::getWithNumber(int i)
{
	switch (i)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		return x;
	}
}

vec3 vec3::mirror()
{
	return vec3(-this->x,-this->y,-this->z);
}

bool vec3::operator==(vec3& other)
{
	return (this->x == other.x && this->y == other.y && this->z == other.z);
}

void vec3::operator=(vec3 other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

void vec3::operator=(std::array<float, 3> other)
{
	this->x = other[0];
	this->y = other[1];
	this->z = other[2];
}

DirectX::XMVECTOR vec3::toXMvector()
{
	DirectX::XMVECTOR a = { x,y,z,1 };
	return a;
}

DirectX::XMFLOAT3 vec3::toXMFloat3()
{
	return DirectX::XMFLOAT3(x,y,z);
}



float vec3::length()
{
	return (float)sqrt((double)(x * x + y * y + z * z));
}

void vec4::Normalize()
{
	double i = sqrt(
		(double)xyz.x * (double)xyz.x +
		(double)xyz.y * (double)xyz.y +
		(double)xyz.z * (double)xyz.z +
		(double)w * (double)w);

	this->xyz.x = (float)(xyz.x / i);
	this->xyz.y = (float)(xyz.y / i);
	this->xyz.z = (float)(xyz.z / i);
	this->w = (float)(w / i);
}

float vec4::operator*(vec4& other)
{
	return(this->xyz.x * other.xyz.x + this->xyz.y * other.xyz.y + this->xyz.z * other.xyz.z + this->w * other.w);
}

vec4 vec4::operator+(vec4& other)
{
	return vec4(this->xyz.x + other.xyz.x, this->xyz.y + other.xyz.y, this->xyz.z + other.xyz.z, this->w + other.w);
}

vec4 vec4::operator-(vec4& other)
{
	return vec4(this->xyz.x - other.xyz.x, this->xyz.y - other.xyz.y, this->xyz.z - other.xyz.z, this->w - other.w);
}

bool vec4::operator==(vec4& other)
{
	return (this->xyz.x == other.xyz.x && this->xyz.y == other.xyz.y && this->xyz.z == other.xyz.z && this->w == other.w);
}

void vec4::operator=(vec4 other)
{
	this->xyz.x = other.xyz.x;
	this->xyz.y = other.xyz.y;
	this->xyz.z = other.xyz.z;
	this->w = other.w;
}

float vec4::length()
{
	return (float)sqrt((double)(xyz.x * xyz.x + xyz.y * xyz.y + xyz.z * xyz.z + w * w));
}

vec2 vec2::operator*(float other)
{
	return vec2(this->x * other, this->y * other);
}

vec2 vec2::operator+(vec2 other)
{
	return vec2(this->x + other.x, this->y + other.y);
}

vec2 vec2::operator-(vec2 other)
{
	return vec2(this->x - other.x, this->y - other.y);
}

vec2 vec2::operator/(vec2 other)
{
	return vec2(this->x / other.x, this->y / other.y);
}

vec2 vec2::operator/(float other)
{
	return vec2(this->x / other, this->y / other);
}

float vec2::dot(vec2 a)
{
	return a.x*this->x + a.y*this->y;
}
