// Victor Guerra <vguerra@gmail.com>
// Nov 2014

#include "util.h"
#include <iostream>

#ifndef HEXCELL_H_
#define HEXCELL_H_

// All possible colors a Cell can have
enum class CellColor : short { EMPTY = 46, BLUE = 79, RED = 88 };
std::ostream& operator<<(std::ostream& out, const CellColor& color);

class HexCell {
  public:
  explicit HexCell(CellColor color = CellColor::EMPTY, unsigned id = 0)
      : _color(color), _id(id) {}
  ~HexCell() {}
  void set_color(CellColor new_color) {
    _color = new_color;
  }                                               // Set color helper
  void set_id(unsigned id) { _id = id; }          // Set id helper
  CellColor get_color() const { return _color; }  // Get color helper
  unsigned get_id() const { return _id; }         // Get id helper
  friend std::ostream& operator<<(std::ostream& out, const HexCell& cell);

  private:
  CellColor _color;
  unsigned _id;
};

#endif  // HEXCELL_H_
