#include "main.h"
#include "cuboid.h"
#include "sail.h"

#ifndef BOAT_H
#define BOAT_H


class Boat {
public:
    Boat() {}
    Boat(float x, float y, float z, float l, float b, float h, color_t color1);
    Cuboid base, cannon, fireball;
    float l, b, h;
    Sail sail;
    glm::vec3 position;
    glm::vec3 speed;
    int wind;
    unsigned long long counter, initCount;
    long long health;
    glm::vec3 rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void update_position(float x, float y, float z);
    void set_rotation(float x, float y, float z);
    void update_rotation(float x, float y, float z);
    void set_speed(float x, float y, float z);
    void update_speed(float x, float y, float z);
    void set_health(long long x);
    void update_health(long long x);
    void tick();
    void windHandler();
    bounding_box_t bounding_box();
    void move(GLFWwindow *window, float scale);
private:
    VAO *object;
};

#endif // BOAT_H
