#include "main.h"
#include "cuboid.h"

#ifndef MONSTER_H
#define MONSTER_H


class Monster {
public:
    Monster() {}
    Monster(float x, float y, float z, float scale, int type, color_t color1);
    Cuboid center, right, left, top, gifts[10], booster, fireball;
    float l, b, h;
    int type, attackDamage, nGifts;
    glm::vec3 position;
    glm::vec3 speed;
    long long health;
    bool isDead;
    glm::vec3 rotation, oldPos;
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
    void spreadTheLove();
    bounding_box_t bounding_box();
    void move(GLFWwindow *window);
private:
    VAO *object;
};

#endif // MONSTER_H
