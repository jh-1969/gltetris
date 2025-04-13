#pragma once

#include <config.hpp>
#include <canvas.hpp>



class Grid {
  private:
    int sideTexture;
    int backgroundTexture;

    char vals[COLUMNS][ROWS];
  
  public:
    Grid(int sideTexture, int backgroundTexture);
    
    void draw(Canvas* canvas);
    char get(int i, int j);
    void set(int i, int j, char val);
    void reset();
};
