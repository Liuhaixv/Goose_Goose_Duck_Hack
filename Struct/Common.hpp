#pragma once
#include <cmath>

struct Vector2
{
    float x, y;

    float distanceTo(IN Vector2& v2) {
        return sqrt(pow(x - v2.x, 2) + pow(y - v2.y, 2));
    }
};

struct Vector3
{
    float x, y, z;

    float distanceTo(IN Vector2& v2) {
        return sqrt(pow(x - v2.x, 2) + pow(y - v2.y, 2));
    }
    float distanceTo(IN Vector3& v3, IN const bool vector2Distance = false) {
        if (vector2Distance) {
            return sqrt(pow(x - v3.x, 2) + pow(y - v3.y, 2));
        }

        return sqrt(pow(x - v3.x, 2) + pow(y - v3.y, 2) + pow(z - v3.z, 2));
    }
};

struct Vector4
{
    float x, y, z, w;
};
