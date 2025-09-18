#include "raytracing.h"

// Vector3 operations
Vector3 vector3_create(float x, float y, float z) {
    Vector3 v = {x, y, z};
    return v;
}

Vector3 vector3_add(Vector3 a, Vector3 b) {
    return vector3_create(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 vector3_sub(Vector3 a, Vector3 b) {
    return vector3_create(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3 vector3_scale(Vector3 v, float s) {
    return vector3_create(v.x * s, v.y * s, v.z * s);
}

float vector3_dot(Vector3 a, Vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vector3_length(Vector3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 vector3_normalize(Vector3 v) {
    float len = vector3_length(v);
    if (len > 0.0f) {
        return vector3_scale(v, 1.0f / len);
    }
    return vector3_create(0, 0, 0);
}

// Color operations
Color color_create(float r, float g, float b) {
    Color c = {r, g, b};
    return c;
}

Color color_add(Color a, Color b) {
    return color_create(a.r + b.r, a.g + b.g, a.b + b.b);
}

Color color_scale(Color c, float s) {
    return color_create(c.r * s, c.g * s, c.b * s);
}

Color color_multiply(Color a, Color b) {
    return color_create(a.r * b.r, a.g * b.g, a.b * b.b);
}

Uint8 lighting_to_grayscale(Color color) {
    float gray = (color.r + color.g + color.b) / 3.0f;
    return (Uint8)(fmaxf(0.0f, fminf(1.0f, gray)) * 255);
}