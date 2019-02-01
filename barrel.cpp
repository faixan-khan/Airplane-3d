#include "cuboid.h"
#include "barrel.h"
#include "main.h"
#define GLM_ENABLE_EXPERIMENTAL
unsigned long long bouncer3 = 3;

float randomGen3(float min, float max) {
  return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}

Barrel::Barrel(float x, float y, float z, color_t color) {
    this->set_position(x, y, z);
    this->set_rotation(0, 0, 0);
    this->set_speed(0, 0, 0);
    this->l = 1.0, this->b=0.5, this->h=0.4;
    this->gift_val = (int)randomGen3(0, 11);

    this->barr = Cuboid(x, y, z, this->l, this->b, this->h, color);
    this->gift = Cuboid(x, y + 2*this->h, z, this->h/2, this->h/2, this->h/2, COLOR_GIFT);
}

void Barrel::draw(glm::mat4 VP) {
  this->barr.draw(VP);
  this->gift.draw(VP);
}

void Barrel::set_speed(float x, float y, float z) {
  this->speed = glm::vec3(x, y, z);
  this->barr.set_speed(x, y, z);
  this->gift.set_speed(x, y, z);
}

void Barrel::update_speed(float x, float y, float z) {
  this->speed += glm::vec3(x, y, z);
  this->barr.update_speed(x, y, z);
  this->gift.update_speed(x, y, z);
}

void Barrel::set_position(float x, float y, float z) {
  this->position = glm::vec3(x, y, z);
  this->barr.set_position(x, y, z);
  this->gift.set_position(x, y + 2*this->h, z);
}

void Barrel::set_rotation(float x, float y, float z) {
    this->rotation = glm::vec3(x, y, z);
    this->barr.set_rotation(x, y, z);
    this->gift.set_rotation(x, y, z);
}

void Barrel::update_rotation(float x, float y, float z) {
    this->rotation += glm::vec3(x, y, z);
    this->barr.update_rotation(x, y, z);
    this->gift.update_rotation(x, y, z);
}

void Barrel::update_position(float x, float y, float z) {
    this->position += glm::vec3(x, y, z);
    this->barr.update_position(x, y, z);
    this->gift.update_position(x, y, z);
}

void Barrel::tick() {
  this->update_position(0, 0.02*cos((bouncer3++)/8), 0);
}

// void Barrel::move(GLFWwindow *window) {
//   int gift  = glfwGetKey(window, GLFW_KEY_LEFT), right = glfwGetKey(window, GLFW_KEY_RIGHT), up = glfwGetKey(window, GLFW_KEY_UP), down = glfwGetKey(window, GLFW_KEY_DOWN), space = glfwGetKey(window, GLFW_KEY_SPACE);
//   if (up) {
//     this->update_position(-0.1*cos(this->rotation*M_PI/180.0f), 0, 0.1*sin(this->rotation * M_PI / 180.0f));
//   }
//   else if (down) {
//     this->update_position(0.1*cos(this->rotation*M_PI/180.0f), 0, -0.1*sin(this->rotation * M_PI / 180.0f));
//   }
//   if (gift) {
//     this->update_rotation(1.0);
//   } else if (right) {
//     this->update_rotation(-1.0);
//   }
//   if (space && this->position.y < 3.15) {
//     this->update_position(0, 7.0, 0);
//   }
//   if (this->position.y > 3.15) {
//     this->update_position(0, -0.15, 0);
//   }
//   this->base.move(window);
//   // this->sail.move(window);
// }

bounding_box_t Barrel::bounding_box() {
  float x = this->position.x, y = this->position.y, z = this->position.z;
  bounding_box_t bbox = {x, y, z, 2*this->l, 2*this->b, 2*this->h};
  return bbox;
}
