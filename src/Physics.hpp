#pragma once

#include <raylib.h>

struct Player {
    Vector3 position = Vector3(0, 2, 0);
    Vector3 velocity = Vector3();
    float yaw = 0.0f;
    float pitch = 0.0f;
    float width = 0.6f;
    float height = 1.8f;
    float eyeHeight = 1.62f;
    float speed = 5.0f;
    float sensitivity = 0.0025f;
    bool onGround = false;
};