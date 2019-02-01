#include "main.h"
#include "cuboid.h"
#include "boat.h"

#ifndef CAMERA_H
#define CAMERA_H


class Camera {
public:
    Camera() {}
    glm::vec3 eye, target, up;
    Camera(glm::vec3 eye, glm::vec3 target, glm::vec3 up);
    void set_eye(float x, float y, float z);
    void set_target(float x, float y, float z);
    void set_up(float x, float y, float z);
    void update_eye(float x, float y, float z);
    void update_target(float x, float y, float z);
    void update_up(float x, float y, float z);
    void update(GLFWwindow *window, Boat boat, int camView);
private:
    VAO *object;
};

#endif // CAMERA_H
