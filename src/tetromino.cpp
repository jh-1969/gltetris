#include <tetromino.hpp>



bool Tetromino::collision(int px, int py, Grid* grid) {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      int index = this->get_rotated_index(i, j);
      
      if(TETROMINO_MAP[type][index] > -1) {
        int ix = px + j;
        int iy = py + i;

        if(ix < 0 || ix > COLUMNS - 1)
          return true;
        else if(iy > ROWS - 1)
          return true;
        else if(iy >= 0 && grid->get(ix, iy) > -1)
          return true;
      }
    }
  }

  return false;
}

int Tetromino::place_at(int px, int py, Grid* grid) {
  int maxY = 0;
  int minY = ROWS;

  int fullCount = 0;

  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      int index = this->get_rotated_index(i, j);
      
      if(TETROMINO_MAP[type][index] > -1) {
        int ix = px + j;
        int iy = py + i;

        maxY = MAX(maxY, iy);
        minY = MIN(minY, iy);

        grid->set(ix, iy, TETROMINO_MAP[type][index]);
      }
    }
  }
  
  if(minY < 0)
    return -1;

  for(int j = minY; j <= maxY; j++) {
    bool full = true;

    for(int i = 0; i < COLUMNS; i++) {
      if(grid->get(i, j) == -1)
        full = false;
    }

    if(full) {
      fullCount++;

      for(int i = 0; i < COLUMNS; i++) {
        for(int k = j - 1; k > -1; k--) {
          grid->set(i, k + 1, grid->get(i, k));
        }
      } 
    }
  }
  
  this->reset();
  return fullCount;
}

int Tetromino::get_rotated_index(int i, int j) {
  switch(this->rot) {
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

void Tetromino::reset() {
  this->x = COLUMNS / 2 - 2;
  this->y = -3;
  this->shadow_y = ROWS;
  this->rot = 0;

  this->type = (TETROMINO_TYPE)(rand() % 7);
}

Tetromino::Tetromino(int texture) {
  this->texture = texture;
  reset();
}

int Tetromino::place(Grid* grid) {
  return this->place_at(this->x, this->y, grid);
}

int Tetromino::place_shadow(Grid* grid) {
  return this->place_at(this->x, this->shadow_y, grid);
}

void Tetromino::rotate(Grid* grid) {
  this->rot++;
  if(this->rot > 3)
    this->rot = 0;
  
  if(this->type != THE_T) {
    if(this->collision(this->x, this->y, grid)) {
      this->rot--;
      if(this->rot < 0)
        this->rot = 3;
    }
  } else {
    switch(this->rot) {
      case 0:
        this->x++;
        if(this->collision(this->x, this->y, grid)) {
          this->rot--;
          this->x--;
          if(this->rot < 0)
            this->rot = 3;
        }
        break;

      case 1:
        this->y++;
        if(this->collision(this->x, this->y, grid)) {
          this->rot--;
          this->y--;
          if(this->rot < 0)
            this->rot = 3;
        }
        break;

      case 2:
        this->x--;
        if(this->collision(this->x, this->y, grid)) {
          this->rot--;
          this->x++;
          if(this->rot < 0)
            this->rot = 3;
        }
        break;

      case 3:
        if(this->collision(this->x, this->y, grid)) {
          this->rot--;
          if(this->rot < 0)
            this->rot = 3;
        }
        break;
    }
  }
}

bool Tetromino::advance(Grid* grid) {
  if(this->collision(this->x, this->y + 1, grid))
    return true;

  this->y++;
  return false;
}

void Tetromino::move_right(Grid* grid) {
  this->x++;
  if(this->collision(this->x, this->y, grid))
    this->x--;
}

void Tetromino::move_left(Grid* grid) {
  this->x--;
  if(this->collision(this->x, this->y, grid))
    this->x++;
}

void Tetromino::draw(Canvas* canvas, Grid* grid) {
  this->shadow_y = ROWS;
  int s = WINDOW_HEIGHT / ROWS;

  int pxs[4];
  int pys[4];
  int cols[4];
  int pi = 0;

  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      int index = this->get_rotated_index(i, j);
      int val = TETROMINO_MAP[type][index];
                 
      if(val > -1) {
        int ix = this->x + j;
        int iy = this->y + i;
        
        int lx = WINDOW_WIDTH / 4 + ix * s;
        int ly = iy * s;

        pxs[pi] = lx;
        pys[pi] = ly;
        cols[pi] = val;
        pi++;

        for(int k = MAX(iy, 0); k < ROWS; k++) {
          if(grid->get(ix, k) > -1) {
            this->shadow_y = MIN(this->shadow_y, k);
          }
        }
      }
    }
  }
  
  this->shadow_y -= 2;
  while(this->collision(this->x, this->shadow_y, grid))
    this->shadow_y--;

  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      int index = this->get_rotated_index(i, j);
      int val = TETROMINO_MAP[type][index];
                 
      if(val > -1) {
        int ix = this->x + j;
        int iy = this->shadow_y + i;
        
        int lx = WINDOW_WIDTH / 4 + ix * s;
        int ly = iy * s;

        canvas->draw_sprite(lx, ly, s, s, 0.0, COLORS[GREY], this->texture);
      }
    }
  }

  for(int i = 0; i < 4; i++) {
    canvas->draw_sprite(pxs[i], pys[i], s, s, 0.0, COLORS[cols[i]], this->texture);
  }
}
