#include <grid.hpp>

Grid::Grid(int sideTexture, int backgroundTexture) {
  this->sideTexture = sideTexture;
  this->backgroundTexture = backgroundTexture;
  
  memset(this->vals, -1, COLUMNS * ROWS);
}
    
void Grid::draw(Canvas* canvas) {
  int fieldOff = WINDOW_WIDTH / 4;
  int s = WINDOW_WIDTH / ROWS;
 
  canvas->draw_sprite(fieldOff, 0, fieldOff * 2,  WINDOW_HEIGHT, 0.0, COLORS[WHITE], this->backgroundTexture);

  for(int i = 0; i < COLUMNS; i++) {
    for(int j = 0; j < ROWS; j++) {
      if(this->vals[i][j] > -1) {
        int x = fieldOff + i * s;
        int y = j * s;
        canvas->draw_sprite(x, y, s, s, 0.0, COLORS[this->vals[i][j]], 0);
      }
    }
  }

  canvas->draw_sprite(0, 0, fieldOff, WINDOW_HEIGHT, 0.0, COLORS[WHITE], this->sideTexture);
  canvas->draw_sprite(fieldOff * 3, 0, fieldOff, WINDOW_HEIGHT, 0.0, COLORS[WHITE], this->sideTexture);

}

char Grid::get(int i, int j) {
  return this->vals[i][j];
}

void Grid::set(int i, int j, char val) {
  this->vals[i][j] = val;
}

void Grid::reset() {
  memset(this->vals, -1, COLUMNS * ROWS);
}
