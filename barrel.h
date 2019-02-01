#include "main.h"
#include "cuboid.h"

#ifndef BARREL_H
#define BARREL_H


class Barrel {
public:
    Barrel() {}
    Barrel(float x, float y, float z, color_t color1);
    Cuboid barr, gift;
    float l, b, h;
    glm::vec3 position;
    glm::vec3 speed;
    int gift_val;
    glm::vec3 rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void update_position(float x, float y, float z);
    void set_rotation(float x, float y, float z);
    void update_rotation(float x, float y, float z);
    void set_speed(float x, float y, float z);
    void update_speed(float x, float y, float z);
    void tick();
    bounding_box_t bounding_box();
    void move(GLFWwindow *window);
private:
    VAO *object;
};

#endif // BARREL_H
