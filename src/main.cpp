#include "main.h"
#include "timer.h"
#include "cuboid.h"
#include "camera.h"
#include "boat.h"
#include "monster.h"
#include "barrel.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Cuboid ocean, rocks[100], healthPoints[100], gifts[10], booster;
Monster m[50], boss, mo;
Boat boat;
Barrel barrels[100];
extern bool bossFlag;
bool isBoost = false;
unsigned long long cccounter = 0, back_counter = 0;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
extern Camera cam;
Timer t60(1.0 / 60);
ao_device *device;
ao_sample_format format;
int default_driver;
char *bufferN, *bufferH, *bufferL;
int buf_size, sounder;
int sampleN, sampleH, sampleL;
float freq = 440.0;

// Declaring camera angle views
extern int camView;
unsigned long long int score = 0;
extern int deathCount;

float randomGen(float min, float max) {
  return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);
    Matrices.view = glm::lookAt( cam.eye, cam.target, cam.up ); // Rotating cam for 3D
    glm::mat4 VP = Matrices.projection * Matrices.view;

    glm::mat4 MVP;  // MVP = Projection * View * Model

    //ocean.draw(VP);
    boat.draw(VP);
    //for(int i=0; i<100; i++) {
    //  rocks[i].draw(VP);
    //  healthPoints[i].draw(VP);
    //  barrels[i].draw(VP);
    //  if(i%2 == 0) {
    //    m[i/2].draw(VP);
    //  }
    //}
    //for(int i=0; i<10; i++) {
    //  gifts[i].draw(VP);
    //}
    //// mo.draw(VP);
    //boss.draw(VP);
    booster.draw(VP);
}

void updateTitle() {
  char s[100];
  sprintf(s, "Health: %lld, Score: %llu", boat.health, score);
  glfwSetWindowTitle(window, s);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void tick_input(GLFWwindow *window) {
  if(isBoost && cccounter<500){
    cccounter++;
    boat.move(window, 10.0);
  } else {
    boat.move(window, 1.0);
    isBoost = false;
    cccounter = 0;
  }
    cam.update(window, boat, camView);  // Updates camera based on camView
}

void updateBackground() {
  color_t blue = { 185, 245, 246 }, brown = { 0, 16, 133 };
  if (((back_counter++)/800)%2 == 0) {
    if(COLOR_BACKGROUND.r != brown.r) {
      COLOR_BACKGROUND.r--;
    }
    if(COLOR_BACKGROUND.g != brown.g) {
      COLOR_BACKGROUND.g--;
    }
    if(COLOR_BACKGROUND.b != brown.b) {
      COLOR_BACKGROUND.b--;
    }
  } else {
    if(COLOR_BACKGROUND.r != blue.r) {
      COLOR_BACKGROUND.r++;
    }
    if(COLOR_BACKGROUND.g != blue.g) {
      COLOR_BACKGROUND.g++;
    }
    if(COLOR_BACKGROUND.b != blue.b) {
      COLOR_BACKGROUND.b++;
    }
  }
  glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f);
}

void tick_elements() {
  // double c1, c2;
  // glfwGetCursorPos(window, &c1, &c2);
  // std::cout<<c1<<" "<<c2<<std::endl;
  booster.tick();
    //ocean.tick();
    boat.tick();

    // Collission detection with rocks
    for(int i=0; i<100; i++) {
      if (detect_collision(boat.bounding_box(), rocks[i].bounding_box())) {
        boat.update_position(2*cos(boat.rotation.y*M_PI / 180.0f), 0, -2*sin(boat.rotation.y*M_PI / 180.0f));
        boat.update_health(-1);
        for(int j=0; j<1000; j++){
          ao_play(device, bufferL + (j)%buf_size, 1);
        }
      }
      if (detect_collision(boat.bounding_box(), barrels[i].bounding_box())) {
        barrels[i].set_position(randomGen(-500, 500), 2.5, randomGen(-500, 500));
        score+= barrels[i].gift_val;
        for(int j=0; j<1000; j++){
          ao_play(device, bufferH + (j)%buf_size, 1);
        }
      }
      if (detect_collision(boat.bounding_box(), healthPoints[i].bounding_box())) {
        healthPoints[i].set_position(randomGen(-500, 500), 2.5, randomGen(-500, 500));
        boat.update_health((int)randomGen(2, 10));
        for(int j=0; j<1000; j++){
          ao_play(device, bufferH + (j)%buf_size, 1);
        }
      }
      healthPoints[i].tick();
      rocks[i].tick();
      barrels[i].tick();
      // if(i%2 == 0) {
      //   if( detect_collision(boat.fireball.bounding_box(), m[i/2].bounding_box()) ) {
      //     m[i/2].update_health(-30);
      //     cout<<m[i/2].health;
      //   }
      // //   m[i/2].tick();
      // }
    }
    // mo.tick();
    // if(mo.fireball.position == mo.position)
    //   mo.fireball.shoot(45, 2);
    float x_dist, z_dist, temp_dist;
    for (int i = 0; i < 50; i++) {
      m[i].tick();
      x_dist = -m[i].position.x + boat.position.x;
      z_dist = -m[i].position.z + boat.position.z;
      temp_dist = sqrt(x_dist*x_dist + z_dist*z_dist);
      if (m[i].fireball.position == m[i].position && m[i].health > 0)
        m[i].fireball.shoot((180.0f - atan2(z_dist,x_dist)*180.0f/M_PI), temp_dist/30);
      if (detect_collision(m[i].fireball.bounding_box(), boat.bounding_box())) {
        boat.health -= m[i].attackDamage/5;
      }
      if (detect_collision(boat.fireball.bounding_box(), m[i].bounding_box())) {
        m[i].health -= 50;
        if(m[i].health <= 0) {
          for(int j=0; j<10; j++) {
            gifts[j].set_position(randomGen(m[i].position.x-0.25, m[i].position.x+0.25), 2.0, randomGen(m[i].position.y-0.25, m[i].position.y+0.25));
            // cout<<gifts[j].position.x<<" "<<gifts[j].position.y<<" "<<gifts[j].position.z<<endl;
          }
        }
      }
    }
    for(int i=0; i<10; i++) {
      if (detect_collision(boat.bounding_box(), gifts[i].bounding_box())) {
        score+=(int)randomGen(3, 10);
        gifts[i].set_position(-2, -2, -2);
      }
      gifts[i].tick();
    }
    x_dist = -boss.position.x + boat.position.x;
    z_dist = -boss.position.z + boat.position.z;
    temp_dist = sqrt(x_dist*x_dist + z_dist*z_dist);
    if (boss.fireball.position == boss.position)
      boss.fireball.shoot((180.0f - atan2(z_dist,x_dist)*180.0f/M_PI), temp_dist/30);
    if (detect_collision(boss.fireball.bounding_box(), boat.bounding_box())) {
      boat.health -= boss.attackDamage/5;
    }
    if (bossFlag && detect_collision(boat.fireball.bounding_box(), boss.bounding_box())) {
      boss.health -= 3;
      if(boss.health<=0) {
        booster.set_position(boss.position.x, 2.0, boss.position.z);
        cout<<booster.position.x<<" "<<booster.position.y<<" "<<booster.position.z<<endl;
        boss.set_position(-10, -100, -10);
        bossFlag = false;
        deathCount++;
      }
    }
    if(deathCount%3==0 && deathCount!=0 && !bossFlag) {
      boss.set_position(randomGen(boat.position.x-50, boat.position.x+50), 10, randomGen(boat.position.z-50, boat.position.z+50));
      cout<<boss.position.x<<" "<<boss.position.y<<" "<<boss.position.z<<endl;
      boss.set_health(200);
      bossFlag = true;
    }
    if(detect_collision(boat.bounding_box(), booster.bounding_box())) {
      isBoost = true;
      booster.set_position(-10, -10, -10);
    }
    boss.tick();
    // Updating title bar
    updateTitle();
    updateBackground();
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    isBoost = false;
    deathCount = 0;
    glm::vec3 eye ( 10, 7, 0 ), target (0, 0, 0), up (0, 1, 0);
    cam = Camera(eye, target, up);
    ocean       = Cuboid(0.0, 0.0, 0.0, 1000.0, 2.0, 1000.0, COLOR_OCEAN_BLUE);
    boat       = Boat(0.0, 0.0, 0.0, 3.0, 1.0, 1.5, COLOR_RED);
    for (int i = 0; i < 100; i++) {
        rocks[i] = Cuboid(randomGen(-500, 500), 2.5, randomGen(-500, 500), 0.5, 0.5, 0.5, COLOR_BLACK);
        healthPoints[i] = Cuboid(randomGen(-500, 500), 2.5, randomGen(-500, 500), 0.3, 0.3, 0.3, COLOR_GREEN);
        barrels[i] = Barrel(randomGen(-500, 500), 2.5, randomGen(-500, 500), COLOR_BARREL);
        if(i%2 == 0) {
          if(i%4 == 0) {
            m[i/2] = Monster(randomGen(-500, 500), 2.5, randomGen(-500, 500), 1.0, 1, COLOR_BOOSTER);
          } else {
            m[i/2] = Monster(randomGen(-500, 500), 2.5, randomGen(-500, 500), 1.0, 0, COLOR_GREEN);
          }
        }
    }
    for(int i =0; i<10; i++) {
      gifts[i] = Cuboid(-3, -3, -3, 0.2, 0.2, 0.2, COLOR_GIFT);
    }
    boss = Monster(-10, -100, -10, 2.0, 69, COLOR_BLACK);
    bossFlag = false;
    mo = Monster(-1, 2.5, -3, 1.0, 1, COLOR_BOOSTER);
    booster = Cuboid(-2, -2, -2, 2, 2, 2, COLOR_BOOSTER);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1920;
    int height = 1080;
    ao_initialize();
    default_driver = ao_default_driver_id();
    memset(&format, 0, sizeof(format));
    format.bits = 16;
    format.channels = 2;
    format.rate = 44100;
    format.byte_format = AO_FMT_LITTLE;
    device = ao_open_live(default_driver, &format, NULL /* no options */);
    if (device == NULL) {
  		fprintf(stderr, "Error opening device.\n");
  		return 1;
  	}
    window = initGLFW(width, height);
    buf_size = format.bits/8 * format.channels * format.rate;
    bufferN = (char*)calloc(buf_size,
        sizeof(char));
    bufferH = (char*)calloc(buf_size,
        sizeof(char));
    bufferL = (char*)calloc(buf_size,
        sizeof(char));

    for (int i = 0; i < format.rate; i++) {
      sampleN = (int)(0.45 * 32768.0 *
        sin(2 * M_PI * freq * ((float) i/format.rate)));
      sampleH = (int)(0.75 * 32768.0 *
        sin(2 * M_PI * freq*2 * ((float) i/format.rate)));
      sampleL = (int)(0.75 * 32768.0 *
        sin(2 * M_PI * freq*(0.5) * ((float) i/format.rate)));

      /* Put the same stuff in left and right channel */
      bufferN[4*i] = bufferN[4*i+2] = sampleN & 0xff;
      bufferN[4*i+1] = bufferN[4*i+3] = (sampleN >> 8) & 0xff;

      bufferH[4*i] = bufferH[4*i+2] = sampleH & 0xff;
      bufferH[4*i+1] = bufferH[4*i+3] = (sampleH >> 8) & 0xff;

      bufferL[4*i] = bufferL[4*i+2] = sampleL & 0xff;
      bufferL[4*i+1] = bufferL[4*i+3] = (sampleL >> 8) & 0xff;
    }
    initGL (window, width, height);
    sounder=0;
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        // sounder++;
	    ao_play(device, bufferN + (sounder++)%buf_size, 1);
        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }
    ao_close(device);

	  ao_shutdown();
    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.length + b.length)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height)) &&
           (abs(a.z - b.z) * 2 < (a.width + b.width));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    // Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
    Matrices.projection = glm::perspective(1.57f, (float)(1920/1080), 0.01f, 500.0f);
}
