#include <unistd.h>
#include <iostream>

#include <engine.hpp>
#include <shader.hpp>
#include <canvas.hpp>
#include <config.hpp>
#include <texturelist.hpp>

#include <glad.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>




char grid[COLUMNS][ROWS];
int s = WINDOW_HEIGHT / ROWS;

TETROMINO_TYPE type;

char tx, ty, sy;
char rot;

double stop = 1.0;
double speedUp = 1.0;
double acc = 0.0;

int blockTexture = 0;
int sideTexture = 0;



static char get_rotated_index(char rot, int i, int j);
static void draw_grid(Canvas* canvas);
static void draw_tetrimono(Canvas* canvas);
static bool check_collision(int px, int py);
static void place_tetromino(int px, int py);
static void reset();

static void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);



int main() {
  Engine* engine = new Engine(WINDOW_WIDTH, WINDOW_HEIGHT);
  engine->add_glfw_key_callback(key_callback);

  Shader* shader = new Shader(
    "assets/shaders/def.vert",
    "assets/shaders/def.frag"
  );
  
  TextureList* texturelist = new TextureList();
  blockTexture = texturelist->add("/home/jh/Pictures/gmail/gmail.jpeg", GL_RGB);
  sideTexture = texturelist->add("/home/jh/Pictures/wallpapers/sunrise.png", GL_RGB);
  texturelist->create_ssbo();

  Canvas* canvas = new Canvas(shader, texturelist);
  
  memset(grid, -1, COLUMNS * ROWS);
  srand(time(0));
  reset();

  do {
    if(glfwGetTime() - acc > stop / speedUp) {
      acc = glfwGetTime();

      ty++;

      if(check_collision(tx, ty)) {
        ty--;
        place_tetromino(tx, ty);
        reset();
      }
    }

    engine->clear();

    draw_tetrimono(canvas);
    draw_grid(canvas);

    canvas->draw();

  } while(engine->running());
  
  delete canvas;
  delete shader;
  delete engine;

  return 0;
}

static char get_rotated_index(char rot, int i, int j) {
  switch(rot) {
    case 0:
      return i * 4 + j;
    case 1:
      return (3 - j) * 4 + i;
    case 2:
      return (3 - i) * 4 + (3 - j);
    case 3:
      return j * 4 + (3 - i);

    default:
      std::cout << "rotation out of bounds" << std::endl;
      exit(1);
  }
}

static void draw_grid(Canvas* canvas) {
  int fieldOff = WINDOW_WIDTH / 4;
 
  for(int i = 0; i < COLUMNS; i++) {
    for(int j = 0; j < ROWS; j++) {
      if(grid[i][j] > -1) {
        int x = fieldOff + i * s;
        int y = j * s;
        canvas->draw_sprite(x, y, s, s, 0.0, COLORS[grid[i][j]], blockTexture);
      }
    }
  }

  canvas->draw_sprite(0, 0, fieldOff, WINDOW_HEIGHT, 0.0, COLORS[WHITE], sideTexture);
  canvas->draw_sprite(WINDOW_WIDTH, 0, -fieldOff, WINDOW_HEIGHT, 0.0, COLORS[WHITE], sideTexture);
}

static void draw_tetrimono(Canvas* canvas) {
  sy = ROWS;

  int pxs[4];
  int pys[4];
  int cols[4];
  int pi = 0;

  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      int index = get_rotated_index(rot, i, j);
      int val = TETROMINO_MAP[type][index];
                 
      if(val > -1) {
        int ix = tx + j;
        int iy = ty + i;
        
        int x = WINDOW_WIDTH / 4 + ix * s;
        int y = iy * s;

        pxs[pi] = x;
        pys[pi] = y;
        cols[pi] = val;
        pi++;

        for(int k = MAX(iy, 0); k < ROWS; k++) {
          if(grid[ix][k] > -1) {
            sy = MIN(sy, k);
          }
        }
      }
    }
  }
  sy -= 2;
  while(check_collision(tx, sy))
    sy--;

  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      int index = get_rotated_index(rot, i, j);
      int val = TETROMINO_MAP[type][index];
                 
      if(val > -1) {
        int ix = tx + j;
        int iy = sy + i;
        
        int x = WINDOW_WIDTH / 4 + ix * s;
        int y = iy * s;

        canvas->draw_sprite(x, y, s, s, 0.0, COLORS[GREY], blockTexture);
      }
    }
  }

  for(int i = 0; i < 4; i++) {
    canvas->draw_sprite(pxs[i], pys[i], s, s, 0.0, COLORS[cols[i]], blockTexture);
  }
}

static bool check_collision(int px, int py) {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      int index = get_rotated_index(rot, i, j);
      
      if(TETROMINO_MAP[type][index] > -1) {
        int x = px + j;
        int y = py + i;

        if(x < 0 || x > COLUMNS - 1)
          return true;
        else if(y > ROWS - 1)
          return true;
        else if(y >= 0 && grid[x][y] > -1)
          return true;
      }
    }
  }
  return false;
}

static void place_tetromino(int px, int py) {
  int maxY = 0;
  int minY = ROWS;

  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      int index = get_rotated_index(rot, i, j);
      
      if(TETROMINO_MAP[type][index] > -1) {
        int x = px + j;
        int y = py + i;

        maxY = MAX(maxY, y);
        minY = MIN(minY, y);

        grid[x][y] = TETROMINO_MAP[type][index];
      }
    }
  }

  for(int j = minY; j <= maxY; j++) {
    bool full = true;

    for(int i = 0; i < COLUMNS; i++) {
      if(grid[i][j] == -1)
        full = false;
    }
    if(full) {
      stop -= SLOWDOWN;

      for(int i = 0; i < COLUMNS; i++) {
        for(int k = j - 1; k > -1; k--) {
          grid[i][k + 1] = grid[i][k];
        }
      } 
    }
  }
}

static void reset() {
  tx = COLUMNS / 2 - 2;
  ty = -3;
  sy = -3;
  rot = 0;

  type = (TETROMINO_TYPE)(rand() % 7);
}

static void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if(action == GLFW_PRESS) {
    if(key == GLFW_KEY_W || key == GLFW_KEY_UP) {
      rot++;
      if(rot > 3)
        rot = 0;
      
      if(type != THE_T) {
        if(check_collision(tx, ty)) {
          rot--;
          if(rot < 0)
            rot = 3;
        }
      } else {
        switch(rot) {
          case 0:
            tx++;
            if(check_collision(tx, ty)) {
              rot--;
              tx--;
              if(rot < 0)
                rot = 3;
            }
            break;

          case 1:
            ty++;
            if(check_collision(tx, ty)) {
              rot--;
              ty--;
              if(rot < 0)
                rot = 3;
            }
            break;

          case 2:
            tx--;
            if(check_collision(tx, ty)) {
              rot--;
              tx++;
              if(rot < 0)
                rot = 3;
            }
            break;

          case 3:
            if(check_collision(tx, ty)) {
              rot--;
              if(rot < 0)
                rot = 3;
            }
            break;
        }
      }
    }
    if(key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
      tx--;
      if(check_collision(tx, ty))
        tx++;
    }
    if(key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
      tx++;
      if(check_collision(tx, ty))
        tx--;
    }
    if(key == GLFW_KEY_S || key == GLFW_KEY_DOWN) {
      speedUp = 10.0;
    }
    if(key == GLFW_KEY_SPACE) {
      place_tetromino(tx, sy);
      reset();
    }
  }

  if(action == GLFW_RELEASE) {
    if(key == GLFW_KEY_S || key == GLFW_KEY_DOWN) {
      speedUp = 1.0;
    }
  }
}
