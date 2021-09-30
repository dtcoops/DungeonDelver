#include "Vector2.h"
#include <cmath>

Vector2::Vector2() {}

Vector2::Vector2(float x, float y)
    {
        this-> x = x;
        this-> y = y;
    }

Vector2 Vector2::Normalize()
    {
        float length = (float) sqrt(x * x + y * y);
        return Vector2(x / length, y / length);
    }
