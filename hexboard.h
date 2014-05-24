// Victor Guerra <vguerra@gmail.com>
// Nov 2014

#include "graph.h"
#include "hexcell.h"
#include "util.h"
#include <iostream>
#include <vector>

#ifndef HEXBOARD_H_
#define HEXCBOARD_H_

class HexBoard {
 public:
  explicit HexBoard(unsigned dimension);  // We need the dimensions of the boar
                                          // for initialization.
  ~HexBoard();
  void Init(); // Initializes all parameters relevant for the board configuration.
  bool is_valid_move(unsigned row, unsigned col) const;  // Checks if the move is valid
  bool is_finished();  // Whether the game has come to an end.
  bool is_winner(Graph* cell_graph, const CellColor& color);
  bool is_cell_empty(unsigned row, unsigned col) const;  // Checks if the cell is empty or not.
  bool make_move(unsigned row,
                 unsigned col, Graph *cell_graph, std::vector<HexCell>& cells, const CellColor& color);  // If a move is valid, we process it.
  void play();  // Run the loop until game is over.
  void make_human_move();  // Performs a move by a human player.
  void make_AI_move();  // Performs a move by the computer using a Monte Carlo simulation.
  friend std::ostream& operator<<(std::ostream& out, const HexBoard& board);

 private:
  DISALLOW_COPY_AND_ASSIGN(HexBoard);  // Using google's technik for disabling
                                       // copy and assign.
  const unsigned _MC_TRIES = 10000;
  unsigned _dimension, _boarders_base;
  bool _finished;
  bool _use_AI;
  std::vector<HexCell> _played_cells;  // Flattened representation of the cells
                                       // on board.
  CellColor _turn;                     // Color in turn.
  CellColor _first_player;             // Color used by 1rst player
  CellColor _second_player;            // Color used by 2nd player
  Graph* _cell_graph;  // Graph used to test connectivity for determining if a
                       // color has won.
  bool coords_in_range(unsigned row, unsigned col) const;  // Checking that row
                                                           // and col are
                                                           // coordinates in
                                                           // range of the
                                                           // board.
  inline unsigned coord_to_index(unsigned row, unsigned col)
      const;         // Mapping cell coordinates (row, col) to a index
  void flip_turn();  // yield turn to next player.
  static CellColor flip_color(CellColor color);
};

#endif  // HEXBOARD_H_
