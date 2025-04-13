#pragma once

#include <config.hpp>
#include <grid.hpp>



class Tetromino {
  private:
    char x, y, shadow_y, rot;
    TETROMINO_TYPE type;
    int texture;

    bool collision(int px, int py, Grid* grid);
    int place_at(int px, int py, Grid* grid); 
      //returns the number of completed lines or -1 on a game loss
      //this function deletes completed lines

    void reset();
    int get_rotated_index(int i, int j);
  
  public:
    Tetromino(int texture);

    int place(Grid* grid);
    int place_shadow(Grid* grid);
      //both use place_at and returns its value;
    
    void rotate(Grid* grid);
    bool advance(Grid* grid);
    void move_right(Grid* grid);
    void move_left(Grid* grid);

    void draw(Canvas* canvas, Grid* grid);
};
