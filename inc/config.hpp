#include <glm/glm.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define ROWS 20
#define COLUMNS 10

#define SLOWDOWN 0.02;

enum TETROMINO_TYPE {
  THE_T,
  SQUERE,
  STICK,
  PERISCOPE_L,
  PERISCOPE_R,
  DOG_L,
  DOG_R,
};

const glm::vec3 COLORS[] {
  glm::vec3(1.0, 0.0, 0.0),
  glm::vec3(0.0, 1.0, 0.0),
  glm::vec3(0.0, 0.0, 1.0),
  glm::vec3(1.0, 1.0, 0.0),
  glm::vec3(0.0, 1.0, 1.0),
  glm::vec3(1.0, 0.0, 1.0),
  glm::vec3(1.0, 0.5, 0.0),
  glm::vec3(0.3, 0.3, 0.3),
  glm::vec3(1.0, 1.0, 1.0),
};

enum COLOR_NAMES {
  RED,
  GREEN,
  BLUE,
  YELLOW,
  PINK,
  PURPLE,
  ORANGE,
  GREY,
  WHITE,
};

const char TETROMINO_MAP[7][16] = {
  {
    -1, -1, -1, -1,
    -1,  0, -1, -1,
     0,  0,  0, -1,
    -1, -1, -1, -1,
  },{
    -1, -1, -1, -1,
    -1,  1,  1, -1,
    -1,  1,  1, -1,
    -1, -1, -1, -1,
  },{
    -1, -1, -1, -1,
    -1, -1, -1, -1,
     2,  2,  2,  2,
    -1, -1, -1, -1,
  },{
    -1, -1, -1, -1,
    -1, -1,  3, -1,
     3,  3,  3, -1,
    -1, -1, -1, -1,
  },{
    -1, -1, -1, -1,
    -1,  4, -1, -1,
    -1,  4,  4,  4,
    -1, -1, -1, -1,
  },{
    -1, -1, -1, -1,
    -1,  5,  5, -1,
     5,  5, -1, -1,
    -1, -1, -1, -1,
  },{
    -1, -1, -1, -1,
    -1,  6,  6, -1,
    -1, -1,  6,  6,
    -1, -1, -1, -1,
  }
};

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)
