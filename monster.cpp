#include "cuboid.h"
#include "monster.h"
#include "main.h"

unsigned long long bouncer2 = 3;
float randomGen4(float min, float max) {
  return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}
int deathCount;
bool bossFlag;

Monster::Monster(float x, float y, float z, float scale, int type, color_t color) {
    this->set_position(x, y, z);
    this->set_rotation(0, 0, 0);
    this->set_speed(0, 0, 0);
    this->set_health(100);
    this->isDead = false;
    this->attackDamage = 10;
    this->fireball = Cuboid(x, y, z, 0.2, 0.2, 0.2, color);
    this->l = 3*scale, this->b=3*scale, this->h=3*scale;
    this->type = type;
    this->nGifts = 3;
    if(type == 69) {
      // boss Monster
      this->attackDamage = 20;
      this->set_health(200);
      this->nGifts = 7;
    } else if (type == 1) {
      this->attackDamage = 15;
      this->set_health(150);
      this->nGifts = 5;
    }
    this->center = Cuboid(x, y, z, l, b, h, color);
    this->left = Cuboid(x, y, z + 3*l/2, l/2, b/2, h/2, color);
    this->right = Cuboid(x, y, z - 3*l/2, l/2, b/2, h/2, color);
    this->top = Cuboid(x, y + 3*l/2, z, l/2, b/2, h/2, color);
    for(int i=0; i<nGifts; i++){
      this->gifts[i] = Cuboid(x, y - 10, z, 0.2, 0.2, 0.2, COLOR_GIFT);
    }
    // this->booster = Cuboid(x, y, z, 0.3, 0.3, 0.3, COLOR_BOOSTER);
}

void Monster::draw(glm::mat4 VP) {
  this->center.draw(VP);
  this->left.draw(VP);
  this->right.draw(VP);
  this->top.draw(VP);
  // this->booster.draw(VP);
  this->fireball.draw(VP);
  for(int i=0; i<nGifts; i++) {
    this->gifts[i].draw(VP);
  }
}

void Monster::set_speed(float x, float y, float z) {
  this->speed = glm::vec3(x, y, z);
  this->center.set_speed(x, y, z);
  this->left.set_speed(x, y, z);
  this->right.set_speed(x, y, z);
  this->top.set_speed(x, y, z);
  // this->booster.set_speed(x, y, z);
  // for(int i=0; i<nGifts; i++) {
  //   this->gifts[i].set_speed(x, y, z);
  // }
}

void Monster::update_speed(float x, float y, float z) {
  this->speed += glm::vec3(x, y, z);
  this->center.update_speed(x, y, z);
  this->left.update_speed(x, y, z);
  this->right.update_speed(x, y, z);
  this->top.update_speed(x, y, z);
  // this->booster.update_speed(x, y, z);
  // for(int i=0; i<nGifts; i++) {
  //   this->gifts[i].set_speed(x, y, z);
  // }
}

void Monster::set_position(float x, float y, float z) {
  this->position = glm::vec3(x, y, z);
  this->center.set_position(x, y, z);
  this->fireball.set_position(x, y, z);
  this->left.set_position(x, y, z + 4.5);
  this->right.set_position(x, y, z - 4.5);
  this->top.set_position(x, y + 4.5, z);
  // this->booster.set_position(x, y - 10, z);
  // for(int i=0; i<nGifts; i++) {
  //   this->gifts[i].set_position(x, y - 10, z);
  // }
}

void Monster::set_rotation(float x, float y, float z) {
    this->rotation = glm::vec3(x, y, z);
    this->center.set_rotation(x, y, z);
    this->fireball.set_rotation(x, y, z);
    this->left.set_rotation(x, y, z);
    this->right.set_rotation(x, y, z);
    this->top.set_rotation(x, y, z);
    // this->booster.set_rotation(x, y, z);
    // for(int i=0; i<nGifts; i++) {
    //   this->gifts[i].set_rotation(x, y, z);
    // }
}

void Monster::update_rotation(float x, float y, float z) {
    this->rotation += glm::vec3(x, y, z);
    this->center.update_rotation(x, y, z);
    this->fireball.update_rotation(x, y, z);
    this->left.update_rotation(x, y, z);
    this->right.update_rotation(x, y, z);
    this->top.update_rotation(x, y, z);
    // this->booster.update_rotation(x, y, z);
    // for(int i=0; i<nGifts; i++) {
    //   this->gifts[i].update_rotation(x, y, z);
    // }
}

void Monster::update_health(long long x) {
    this->health +=x;
}

void Monster::set_health(long long x) {
    this->health = x;
}

void Monster::update_position(float x, float y, float z) {
    this->position += glm::vec3(x, y, z);
    this->center.update_position(x, y, z);
    this->fireball.update_position(x, y, z);
    this->left.update_position(x, y, z);
    this->right.update_position(x, y, z);
    this->top.update_position(x, y, z);
    // this->booster.update_position(x, y, z);
    // for(int i=0; i<nGifts; i++) {
    //   this->gifts[i].update_position(x, y, z);
    // }
}

void Monster::tick() {
  // this->update_position(0, 0.02*cos((bouncer2++)/8), 0);
  this->center.tick();
  this->left.tick();
  this->right.tick();
  this->top.tick();
  // this->booster.tick();
  if(this->health<=0 && !this->isDead) {
    this->isDead = true;
    deathCount++;
    this->spreadTheLove();
    this->oldPos = this->position;
    this->set_position(-10,-100, -10);
    if(type==0) {
      this->set_health(-100);
    } else if(type==1){
      this->set_health(-150);
    }
    for(int i=0; i<nGifts; i++) {
      this->gifts[i].tick();
    }
  }
  if (this->fireball.position.y > -1 && this->fireball.speed.y != 0) {
    this->fireball.speed.y -= 0.1;
  } else {
    this->fireball.set_position(this->position.x, this->position.y, this->position.z);
    this->fireball.set_speed(0, 0, 0);
  }
  this->fireball.tick();
}

void Monster::spreadTheLove() {
  if(this->type==69) {
    // this->booster.set_position(this->position.x, this->position.y + 1, this->position.z);
    // this->booster.shoot(randomGen4(0, 360), 0.5);
  } else {
    for(int i=0; i<nGifts; i++) {
      this->gifts[i].set_position(this->oldPos.x - 2, this->oldPos.y + 1, this->oldPos.z);
      // this->gifts[i].shoot(randomGen4(0, 360), 1.5);
    }
  }
}


bounding_box_t Monster::bounding_box() {
  float x = this->position.x, y = this->position.y, z = this->position.z;
  bounding_box_t bbox = {x, y, z, 2*this->l, 2*this->b, 2*this->h};
  return bbox;
}
