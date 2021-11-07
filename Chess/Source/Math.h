#pragma once
class Vector2 {
public:
    float x;
    float y;

    Vector2() : x{ 0.f }, y{ 0.f }{};
    Vector2(int inX, int inY) : x{ inX }, y{ inY }{};

    static const Vector2 Zero;
    static const Vector2 UnitX;
    static const Vector2 UnitY;
    static const Vector2 NegUnitX;
    static const Vector2 NegUnitY;

};
