#include "main.h"

#ifndef CUBOID_H
#define CUBOID_H
#define GLM_ENABLE_EXPERIMENTAL

class Cuboid {
public:
    Cuboid() {}
    Cuboid(float x, float y, float z, float l, float b, float h, color_t color);
    float l, b, h;
    glm::vec3 position;
    glm::vec3 speed;
    glm::vec3 rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void update_position(float x, float y, float z);
    void set_rotation(float x, float y, float z);
    void update_rotation(float x, float y, float z);
    void set_speed(float x, float y, float z);
    void update_speed(float x, float y, float z);
    void move(GLFWwindow *window);
    void shoot(float x, float p);
    bounding_box_t bounding_box();
    void tick();
private:
    VAO *object;
};

#endif // CUBOID_H
