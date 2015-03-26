#pragma once

#include <math.h>

// 2D vector class with basic operations and functions for easier to read math.
class Vector2
{
private:

	float X;
	float Y;

public:

	// ========================
	// ===== Constructors =====
	// ========================

	Vector2()
	{
		X = Y = 0;
	}

	Vector2(float n)
	{
		X = Y = n;
	}

	Vector2(float x, float y)
	{
		X = x;
		Y = y;
	}

	// =====================
	// ===== Operators =====
	// =====================

	// Negation
	Vector2 operator -() const
	{
		return Vector2(-X, -Y);
	}

	// Elementwise addition for two vectors
	Vector2 operator +(const Vector2 vec) const
	{
		return Vector2(X + vec.X, Y + vec.Y);
	}

	// Elementwise addition for vector and scalar
	Vector2 operator +(const float value) const
	{
		return Vector2(X + value, Y + value);
	}

	// Increment vector elementwise by vector
	Vector2& operator +=(const Vector2 vec)
	{
		X += vec.X;
		Y += vec.Y;
		return *this;
	}

	// Increment vector elementwise by scalar
	Vector2& operator +=(const float value)
	{
		X += value;
		Y += value;
		return *this;
	}

	// Elementwise subtraction for two vectors
	Vector2 operator -(const Vector2 vec) const
	{
		return Vector2(X - vec.X, Y - vec.Y);
	}

	// Elementwise subtraction for vector and scalar
	Vector2 operator -(const float value) const
	{
		return Vector2(X - value, Y - value);
	}

	// Decrement vector elementwise by vector
	Vector2& operator -=(const Vector2 vec)
	{
		X -= vec.X;
		Y -= vec.Y;
		return *this;
	}

	// Decrement vector elementwise by scalar
	Vector2& operator -=(const float value)
	{
		X -= value;
		Y -= value;
		return *this;
	}

	// Elementwise multiplication for two vectors
	Vector2 operator *(const Vector2 vec) const
	{
		return Vector2(X * vec.X, Y * vec.Y);
	}

	// Elementwise multiplication for vector and scalar
	Vector2 operator *(const float value) const
	{
		return Vector2(X * value, Y * value);
	}

	// Multiply vector elementwise by vector
	Vector2& operator *=(const Vector2 vec)
	{
		X *= vec.X;
		Y *= vec.Y;
		return *this;
	}

	// Multiply vector elementwise by scalar
	Vector2& operator *=(const float value)
	{
		X *= value;
		Y *= value;
		return *this;
	}

	// Elementwise division for two vectors
	Vector2 operator /(const Vector2 vec) const
	{
		return Vector2(X / vec.X, Y / vec.Y);
	}

	// Elementwise division for vector and scalar
	Vector2 operator /(const float value) const
	{
		return Vector2(X / value, Y / value);
	}

	// Divide vector elementwise by vector
	Vector2& operator /=(const Vector2 vec)
	{
		X /= vec.X;
		Y /= vec.Y;
		return *this;
	}

	// Divide vector elementwise by scalar
	Vector2& operator /=(const float value)
	{
		X /= value;
		Y /= value;
		return *this;
	}

	// Assign vector
	Vector2& operator =(const Vector2 vec)
	{
		X = vec.X;
		Y = vec.Y;
		return *this;
	}

	// =====================
	// ===== Functions =====
	// =====================

	// Return the square of the vector's magnitude
	float lengthSquared()
	{
		return X * X + Y * Y;
	}

	// Return the vector's magnitude
	float length()
	{
		return sqrt(lengthSquared());
	}

	// Normalize the vector
	void normalize()
	{
		float scale = 1.0f / length();
		X *= scale;
		Y *= scale;
	}

	// Return a normalized vector
	Vector2 normalized() const
	{
		Vector2 vector = *this;
		vector.normalize();
		return vector;
	}

};