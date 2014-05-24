// Victor Guerra <vguerra@gmail.com>
// Nov 2014

#include "hexcell.h"

// Printing out a Cell color.
std::ostream& operator<<(std::ostream& out, const CellColor& color) {
  return out << char(color);
}

// Printing out a Cell.
std::ostream& operator<<(std::ostream& out, const HexCell& cell) {
  return out << cell._color;
}
