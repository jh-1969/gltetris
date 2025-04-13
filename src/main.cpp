#include <unistd.h>
#include <iostream>

#include <engine.hpp>
#include <shader.hpp>
#include <canvas.hpp>
#include <config.hpp>
#include <texturelist.hpp>

#include <grid.hpp>
#include <tetromino.hpp>

#include <glm/glm.hpp>



//used to get input from key_callback
bool pressedRight = false, pressedLeft = false, pressedUp = false, pressedDown = false, pressedSpace = false, pressedEsc = false, pressedEnter = false;
bool releaseDown = false;

static void reset_input();



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
  int blockTex = texturelist->add("./assets/textures/block.jpg", false);
  int gridBgTex = texturelist->add("./assets/textures/grid_bg.jpg", false);
  int sideTex = texturelist->add("./assets/textures/side.jpg", false);
  int menuTex = texturelist->add("./assets/textures/menu_bg.jpg", false);
  int titleTex = texturelist->add("./assets/textures/title.png", true);
  int playTex = texturelist->add("./assets/textures/play_txt.png", true);
  int exitTex = texturelist->add("./assets/textures/exit_txt.png", true);
  texturelist->create_ssbo();

  Canvas* canvas = new Canvas(shader, texturelist);
  
  GAME_PHASE phase = MENU;
  srand(time(0));

  Grid* grid = new Grid(sideTex, gridBgTex);;
  Tetromino* tetromino = new Tetromino(blockTex);;

  bool gameRunning = true;
  bool selectedPlay = true;

  double stop = START_SPEED;
  double speedUp = 1.0;
  double acc = 0.0;

  do {
    engine->clear();

    switch(phase) {
      case MENU: {
        if(pressedUp)
          selectedPlay = true;
        if(pressedDown)
          selectedPlay = false;
        
        if(pressedEnter) {
          if(selectedPlay)
            phase = GAME;
          else
            gameRunning = false;
        }
        
        canvas->draw_sprite(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0, COLORS[WHITE], menuTex);
        canvas->draw_sprite(100, 100, 600, 180, 0.0, COLORS[WHITE], titleTex);
        canvas->draw_sprite(300, 400, 200, 80, 0.0,
            selectedPlay ? COLORS[LIGHT_GREY] : COLORS[GREY], playTex);
        canvas->draw_sprite(300, 500, 200, 80, 0.0,
            selectedPlay ? COLORS[GREY] : COLORS[LIGHT_GREY], exitTex);
        break;
      }

      case GAME: {
        int placedCount = 0;

        if(pressedRight)
          tetromino->move_right(grid);
        if(pressedLeft)
          tetromino->move_left(grid);
        if(pressedUp)
          tetromino->rotate(grid);
        if(pressedSpace)
          placedCount = tetromino->place_shadow(grid);
        if(pressedDown)
          speedUp = 10.0;
        if(releaseDown)
          speedUp = 1.0;

        if(glfwGetTime() - acc > stop / speedUp) {
          acc = glfwGetTime();

          if(tetromino->advance(grid)) {
            placedCount = tetromino->place(grid);
          }
        }
        
        grid->draw(canvas);
        tetromino->draw(canvas, grid);
        
        if(placedCount != 0 && placedCount == -1) {
          stop = START_SPEED;
          grid->reset();
          phase = MENU;
        } else {
          stop -= (double)placedCount * SPEEDUP;
        }
        break;
      }
    }
    
    canvas->draw();
    engine->flush();
    reset_input(); 
  } while(engine->running() && gameRunning);

  delete grid;
  delete tetromino;

  delete texturelist;
  delete canvas;
  delete shader;
  delete engine;

  return 0;
}

static void reset_input() {
  pressedUp = false;
  pressedDown = false;
  pressedLeft = false;
  pressedRight = false;
  pressedSpace = false;
  pressedEsc = false;
  pressedEnter = false;
  
  releaseDown = false;
}

static void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if(action == GLFW_PRESS) {
    if(key == GLFW_KEY_W || key == GLFW_KEY_UP)
      pressedUp = true;
    if(key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
      pressedLeft = true;
    if(key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
      pressedRight = true;
    if(key == GLFW_KEY_S || key == GLFW_KEY_DOWN)
      pressedDown = true;
    if(key == GLFW_KEY_SPACE)
      pressedSpace = true;
    if(key == GLFW_KEY_ESCAPE)
      pressedEsc = true;
    if(key == GLFW_KEY_ENTER)
      pressedEnter = true;
  }

  if(action == GLFW_RELEASE) {
    if(key == GLFW_KEY_S || key == GLFW_KEY_DOWN)
      releaseDown = true;
  }
}
