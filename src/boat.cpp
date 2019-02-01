#include "cuboid.h"
#include "boat.h"
#include "sail.h"
#include "main.h"
#include "camera.h"

unsigned long long bouncer = 0;
float randomGen2(float min, float max) {
  return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}
Camera cam;
extern int camView;

Boat::Boat(float x, float y, float z, float l, float b, float h, color_t color) {
    this->set_position(x, y, z);
    this->set_rotation(0, 0, 0);
    this->set_speed(0, 0, 0);
    this->set_health(100);
    this->l = l;
    this->b = b;
    this->h = h;
    this->base = Cuboid(x, y, z, l, b, h, color);
    this->fireball = Cuboid(x, y, z, 0.2, 0.2, 0.2, color);
    this->cannon = Cuboid(x, y + 2, z, 3, 0.15, 0.15, COLOR_BOOSTER);
    this->sail = Sail(x, y+ 2, z, 5, 4, 0.2, COLOR_WHITE);
    this->sail.set_rotation(this->rotation.y + 90);
    this->counter = 0;
    this->wind = 0;
}

void Boat::draw(glm::mat4 VP) {
  this->sail.draw(VP);
  this->base.draw(VP);
  this->cannon.draw(VP);
  this->fireball.draw(VP);
}

void Boat::set_speed(float x, float y, float z) {
  this->speed = glm::vec3(x, y, z);
  this->base.set_speed(x, y, z);
  this->sail.set_speed(x, y, z);
  this->cannon.set_speed(x, y, z);
}

void Boat::update_speed(float x, float y, float z) {
  this->speed += glm::vec3(x, y, z);
  this->base.update_speed(x, y, z);
  this->sail.update_speed(x, y, z);
  this->cannon.update_speed(x, y, z);
}

void Boat::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    this->base.set_position(x, y, z);
    this->sail.set_position(x, y+ 2, z);
    this->fireball.set_position(x, y, z);
    this->cannon.set_position(x, y+ 2, z);
}

void Boat::set_rotation(float x, float y, float z) {
    this->rotation = glm::vec3(x, y, z);
    this->base.set_rotation(x, y, z);
    this->base.set_rotation(x, y, z);
    this->fireball.set_rotation(x, y, z);
    this->cannon.set_rotation(x, y, this->rotation.z-30);
}

void Boat::update_rotation(float x, float y, float z) {
    this->rotation += glm::vec3(x, y, z);
    this->base.update_rotation(x, y, z);
    this->fireball.update_rotation(x, y, z);
    this->base.update_rotation(x, y, 0);
}

void Boat::update_health(long long x) {
    this->health +=x;
}

void Boat::set_health(long long x) {
    this->health = x;
}

void Boat::update_position(float x, float y, float z) {
    this->position += glm::vec3(x, y, z);
    this->base.update_position(x, y, z);
    this->sail.update_position(x, y, z);
    this->fireball.update_position(x, y, z);
    this->cannon.update_position(x, y, z);
}

void Boat::tick() {
  if(this->position.y> 2.5 && this->speed.y!=0) {
    this->update_speed(0, -0.1, 0);
  } else if(this->position.y<2.5) {
    this->position.y = 2.5;
    this->base.position.y = 2.5;
    this->sail.position.y = 2.5 + 2;
    this->cannon.position.y = 2.5 + 2;
    this->fireball.position.y = 2.5;
    this->set_speed(this->speed.x, 0, this->speed.y);
  }
    this->update_position(0, 0.05*cos((bouncer++)/8), 0);
    this->position.x += this->speed.x;
    this->position.y += this->speed.y;
    this->position.z += this->speed.z;
    this->position = this->base.position;
    this->rotation = this->base.rotation;
    this->cannon.position = this->base.position + glm::vec3(0, 1.5, 0);
    this->cannon.set_rotation(this->base.rotation.x, this->base.rotation.y, -30);
    this->sail.position = this->base.position + glm::vec3(0, 2.5, 0);
    if (this->fireball.position.y > -1 && this->fireball.speed.y != 0) {
      this->fireball.speed.y -= 0.1;
    } else {
      // if(this->fireball.position != this->position)
      // std::cout<<this->fireball.position.x<<" "<<this->fireball.position.y<<" "<<this->fireball.position.z<<" \n";
      this->fireball.set_position(this->position.x, this->position.y, this->position.z);
      this->fireball.set_speed(0, 0, 0);
    }
    this->windHandler();
    this->base.tick();
    this->sail.tick();
    this->cannon.tick();
    this->fireball.tick();
    // if(this->position.y> 2.5 && this->speed.y!=0) {
    //   this->update_speed(0, -0.1, 0);
    // } else if(this->position.y<2.5) {
    //   this->position.y = 2.5;
    //   this->base.position.y = 2.5;
    //   this->sail.position.y = 2.5 + 2;
    //   this->cannon.position.y = 2.5 + 2;
    //   this->fireball.position.y = 2.5;
    //   this->set_speed(this->speed.x, 0, this->speed.y);
    // } //else if(this->position.y > 2.5) {
    //   this->update_position(0, -0.1, 0);
    // }
}

void Boat::move(GLFWwindow *window, float scale) {
  int left  = glfwGetKey(window, GLFW_KEY_LEFT), right = glfwGetKey(window, GLFW_KEY_RIGHT), up = glfwGetKey(window, GLFW_KEY_UP), down = glfwGetKey(window, GLFW_KEY_DOWN), space = glfwGetKey(window, GLFW_KEY_SPACE), a = glfwGetKey(window, GLFW_KEY_A), c = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
  //// double cursor_x, cursor_y;
  //if (up) {
  //  this->update_position(scale*-0.1*cos(this->rotation.y*M_PI/180.0f), 0, scale*0.1*sin(this->rotation.y * M_PI / 180.0f));
  //}
  //else if (down) {
  //  this->update_position(scale*0.1*cos(this->rotation.y*M_PI/180.0f), 0, scale*-0.1*sin(this->rotation.y * M_PI / 180.0f));
  //}
  //if (left) {
  //  this->update_rotation(0, 1.0, 0);
  //} else if (right) {
  //  this->update_rotation(0, -1.0, 0);
  //}
  //if (space){// && this->position.y == 2.5) {
  //  //this->set_speed(0, 2.0, 0);
  //  //this->position.y+=0.01;;
  //}
  // if (this->position.y > 3.15) {
  //   this->update_position(0, -0.15, 0);
  // }
  if ( a && this->fireball.position == this->position) {
    this->wind = 0;
    this->fireball.shoot(this->rotation.y, 1.5);
  }
   if(glfwGetKey(window, GLFW_KEY_F)) {
    double cursor_x1, cursor_y1;
    glfwGetCursorPos(window, &cursor_x1, &cursor_y1);
    this->wind = 0;
    cursor_x1 -= 840;
    cam.update_target(0, cursor_y1/100, -cursor_x1/100);
    float x_dist = cam.target.x - cam.eye.x;
    float z_dist = cam.target.z - cam.eye.z;
    float theta = 180 - atan2(z_dist,x_dist)*180.0f/M_PI;
    int mouse_left = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (mouse_left && this->position.x == this->fireball.position.x) {
      this->fireball.set_position(cam.eye.x, cam.eye.y, cam.eye.z);
      this->fireball.shoot(theta, cursor_y1/300);
    }
  }
  this->base.move(window);
}

void Boat::windHandler() {
  //Increasing counter to change wind
  if ((this->counter++) % 500 == 0) {
      this->wind = (int)randomGen2(0, 4);
  }
  // Changing rotation of sail and position of this->based on wind
  if (this->wind == 0) {                                     // No wind, default
    this->sail.set_rotation(this->rotation.y+90);              // Sail is always perpendicular to this->  } else if (this->wind == 1 || this->wind == 2) {
    // Sail rotation
    if(this->sail.rotation < 90) {
      this->sail.update_rotation(1);
    } else if(this->sail.rotation > 90) {
      this->sail.update_rotation(-1);
    }
    // Boat position
    if(wind == 1) {
      this->update_position(0.01, 0, 0);
    } else {
      this->update_position(-0.01, 0, 0);
    }
  } else {
    // Sail rotation
    if(this->sail.rotation < 0) {
      this->sail.update_rotation(1);
    } else if(this->sail.rotation > 0) {
      this->sail.update_rotation(-1);
    }
    // Boat position
    if(wind == 3) {
      this->update_position(0, 0, 0.01);
    } else {
      this->update_position(0, 0, -0.01);
    }
  }

}

bounding_box_t Boat::bounding_box() {
  float x = this->position.x, y = this->position.y, z = this->position.z;
  bounding_box_t bbox = {x, y, z, 2*this->l, 2*this->b, 2*this->h};
  return bbox;
}
