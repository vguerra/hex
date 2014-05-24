// Victor Guerra <vguerra@gmail.com>
// Nov 2014

#include "hexboard.h"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <set>
#include <string>
#include <regex>
#include <cctype>
#include <random>
#include <iterator>

// public methods

HexBoard::HexBoard(unsigned dimension)
    : _dimension(dimension),
      _boarders_base(_dimension * _dimension),
      _finished(false),
      _use_AI(false),
      _turn(CellColor::EMPTY),
      _first_player(CellColor::EMPTY),
      _second_player(CellColor::EMPTY) {
  // Create a Graph containing all possible vertices N*N
  // plust 4 nodes that are going to be used to test connectivity
  // to check if a given player has won.
  _cell_graph = new Graph(_dimension * _dimension + 4);
}

HexBoard::~HexBoard() { delete _cell_graph; }

void HexBoard::Init() {
  // Initializing vector of cells
  _played_cells.clear();
  _played_cells.resize(_boarders_base + 4);

  // We assign an id to each cell, that is usefull to remember the original
  // position of the cell when shuffling the cells in the monte carlo simulation

  for (unsigned index = 0; index < (_boarders_base + 4); ++index) {
    _played_cells[index].set_id(index);
  }

  // Assigning the right colours to the four cardinal points on the board.
  _played_cells[_boarders_base].set_color(CellColor::RED);       // North
  _played_cells[_boarders_base + 1].set_color(CellColor::RED);   // South
  _played_cells[_boarders_base + 2].set_color(CellColor::BLUE);  // West
  _played_cells[_boarders_base + 3].set_color(CellColor::BLUE);  // East

  // Interacting with stdin/stdout to configure board.

  char players = '0', first_player = '0', first_turn = '0';
  std::cout << std::string(100, '\n');

  while (players != '1' && players != '2') {
    std::cout << "1 or 2 players? ";
    std::cin >> players;
    std::cin.ignore(100, '\n');
  }

  if (players == '1') {
    _use_AI = true;
  } else {
    _use_AI = false;
  }

  while (toupper(first_player) != 'X' && toupper(first_player) != 'O') {
    std::cout << "Which figure you want to play with? X or O? ";
    std::cin >> first_player;
    std::cin.ignore(100, '\n');
  }

  if (toupper(first_player) == 'X') {
    _first_player = CellColor::RED;
    _second_player = CellColor::BLUE;
  } else {
    _first_player = CellColor::BLUE;
    _second_player = CellColor::RED;
  }

  while (toupper(first_turn) != 'X' && toupper(first_turn) != 'O') {
    std::cout << "Which figure should go first? X or O? ";
    std::cin >> first_turn;
    std::cout << "selected " << first_turn << "\n";
    std::cin.ignore(100, '\n');
  }

  // when beginning to play we flip turn
  // so we invert the choice of the user when choosing
  // which figure goes first.
  if (toupper(first_turn) == 'X') {
    _turn = CellColor::RED;
  } else {
    _turn = CellColor::BLUE;
  }
}

// We start to play and get out of this procedure until
// a player wins.
void HexBoard::play() {
  flip_turn();
  while (!is_finished()) {  // Checking if previous player made a winning move.
    flip_turn();            // Turn for next player.
    std::cout << std::string(100, '\n');
    std::cout << *this;

    if (_turn == _second_player && _use_AI) {
      make_AI_move();
    } else {
      make_human_move();
    }
  }
  std::cout << std::string(100, '\n');
  std::cout << *this;
}

// This method calculates computer's move.
void HexBoard::make_AI_move() {
  unsigned MC_tries = _MC_TRIES;
  std::vector<unsigned> MC_counts(_dimension * _dimension,
                                  0);  // We count the success full tries for
                                       // each of the free cells available.

  while (MC_tries--) {
    CellColor ai_turn = _turn;
    // We need a copy of the actual state of the board: The cells and the graph.
    auto tmp_cells = _played_cells;
    Graph* tmp_graph = new Graph(*_cell_graph);

    unsigned first_free = UINT_MAX;

    int long seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::knuth_b generator(static_cast<unsigned>(seed));

    // We shuffle the cells to simulate a random sequence of plays.
    std::shuffle(tmp_cells.begin(), tmp_cells.end(), generator);

    // We walk the sequence of cells, painting them accordingly.
    for (auto cell = tmp_cells.begin(); cell != tmp_cells.end(); ++cell) {
      unsigned row = static_cast<unsigned>(cell->get_id() / _dimension);
      unsigned col = static_cast<unsigned>(cell->get_id() % _dimension);
      if (make_move(row, col, tmp_graph, tmp_cells, ai_turn)) {
        if (first_free == UINT_MAX) {
          first_free = cell->get_id();
        }
        ai_turn = flip_color(ai_turn);
      }
    }

    // Now that every cell has been painted we verify is this simulation tooks
    // us to a win, i.e. is the computer the winner?
    if (is_winner(tmp_graph, _turn)) {
      ++MC_counts[first_free];
    }
    delete tmp_graph;
  }

  // FInally we determine the computers move according to the results of the
  // simulation.
  long int major_count_index = std::distance(
      MC_counts.begin(), std::max_element(MC_counts.begin(), MC_counts.end()));
  unsigned row = static_cast<unsigned>(major_count_index / _dimension);
  unsigned col = static_cast<unsigned>(major_count_index % _dimension);
  make_move(row, col, _cell_graph, _played_cells, _turn);
}

// This method ask human players for coordinates of move and performs the move.
void HexBoard::make_human_move() {
  char col;
  unsigned row;
  std::string line;
  bool repeat = true;

  while (repeat) {
    std::cout << "Enter row(number) and column(letter) (e.g. 2A): ";
    std::getline(std::cin, line);
    std::cmatch cm;
    if (std::regex_match(line.c_str(), cm, std::regex("(^[0-9]+)([a-zA-Z])"))) {
      row = static_cast<unsigned>(std::atoi(cm[1].str().c_str()));
      col = cm[2].str().at(0);
      if (!make_move(row, static_cast<unsigned>(toupper(col) - 65), _cell_graph,
                     _played_cells, _turn)) {
        std::cout << "Invalid Move!";
        std::cin.get();
      } else {
        repeat = false;
      }
    } else {
      std::cout << "Invalid Coordinates Input!";
      std::cin.get();
      // std::cin.ignore(100, '\n').get();
    }
  }
}

bool HexBoard::is_valid_move(unsigned row, unsigned col) const {
  return coords_in_range(row, col) && is_cell_empty(row, col);
}

bool HexBoard::is_cell_empty(unsigned row, unsigned col) const {
  return _played_cells[coord_to_index(row, col)].get_color() ==
         CellColor::EMPTY;
}

bool HexBoard::make_move(unsigned row, unsigned col, Graph* cell_graph,
                         std::vector<HexCell>& cells, const CellColor& color) {
  if (is_valid_move(row, col)) {
    unsigned cell_index = coord_to_index(row, col);
    unsigned source, tail;
    source = tail = cell_index;
    cells[cell_index].set_color(color);
    // This next block of code handles the case on which the move
    // happens on the boarder rows and columns.
    // If the turn is on RED and move happens on row = 0 or row = dimension - 1
    // we connect the proper cardinal point node to the cell (North or South).
    // The same analogy is used for BLUE using the cardinal points West and
    // East.
    if (color == CellColor::RED) {
      if (row == 0) {
        tail = _boarders_base;
      } else if (row == _dimension - 1) {
        tail = _boarders_base + 1;
      }
    } else {
      if (col == 0) {
        tail = _boarders_base + 2;
      } else if (col == _dimension - 1) {
        tail = _boarders_base + 3;
      }
    }
    if (source != tail) {
      cell_graph->add(source, tail);
      cell_graph->set_edge_value(source, tail, 1);
    }

    // We inspect the neighboards in order to create
    // the corresponding edges on the graph. All adjacent cells
    // have to be checked
    int row_pivot = static_cast<int>(row);
    int column_pivot = static_cast<int>(col);
    for (int i = row_pivot - 1; i <= row_pivot + 1; ++i) {
      for (int j = column_pivot - 1; j <= column_pivot + 1; ++j) {
        unsigned tail_index =
            coord_to_index(static_cast<unsigned>(i), static_cast<unsigned>(j));
        if (coords_in_range(static_cast<unsigned>(i),
                            static_cast<unsigned>(j)) &&
            cells[tail_index].get_color() == _turn) {
          cell_graph->add(cell_index, tail_index);
          cell_graph->set_edge_value(cell_index, tail_index, 1);
        }
      }
    }
    return true;
  }
  return false;
}

bool HexBoard::is_winner(Graph* cell_graph, const CellColor& color) {
  std::set<unsigned int> reachable_cells;
  unsigned source, tail;
  if (color == CellColor::RED) {
    source = _boarders_base;
    tail = source + 1;
  } else {
    source = _boarders_base + 2;
    tail = source + 1;
  }
  // Geting a list of cells reachable from source
  // If the wanted carninal point is reachable then we got a winner.
  cell_graph->mst(source, reachable_cells);
  if (reachable_cells.find(tail) != reachable_cells.end()) {
    return true;
  }
  return false;
}

bool HexBoard::is_finished() {
  if (!_finished) {
    _finished = is_winner(_cell_graph, _turn);
  }
  return _finished;
}

// friend methods
// Printing the board.
std::ostream& operator<<(std::ostream& out, const HexBoard& board) {
  std::string indentation("");
  std::stringstream legend_letter_row;
  unsigned legend_letter = 'A';
  int pad = (3 * (static_cast<int>(board._dimension) - 1) + 1) / 2;
  if (!board._finished) {
    out << "Turn for shape: '" << board._turn << "'\n\n";
  } else {
    out << "The WINNER is: '" << board._turn << "'\n\n";
  }
  out << "      " << std::setfill(' ') << std::setw(pad) << 'X' << "\n"
      << "      ";
  for (unsigned i = 0; i < board._dimension - 1; ++i) {
    out << "___";
  }
  out << "_\n"
      << "      ";
  for (unsigned i = 0; i < board._dimension; ++i) {
    legend_letter_row << static_cast<char>(legend_letter + i) << "  ";
  }
  legend_letter_row << "\n";
  out << indentation << legend_letter_row.str();

  for (unsigned i = 0; i < board._dimension; ++i) {
    bool show_char = (static_cast<int>(i) == (board._dimension / 2));
    out << indentation << std::setw(2) << (show_char ? "O " : "  ") << "\\ "
        << i << " ";
    for (unsigned j = 0; j < board._dimension; ++j) {
      unsigned index = i * board._dimension + j;
      out << " " << board._played_cells[index] << " ";
    }
    out << i << " \\" << (show_char ? " O" : "") << "\n";
    indentation.append("  ");
  }
  indentation.append("      ");
  out << indentation << legend_letter_row.str();
  out << indentation;
  for (unsigned i = 0; i < board._dimension - 1; ++i) {
    out << "---";
  }

  out << "\n" << indentation << std::setfill(' ') << std::setw(pad) << 'X'
      << "\n\n";
  return out;
}

// private methods

bool HexBoard::coords_in_range(unsigned row, unsigned col) const {
  return ((_dimension - 1 - row) < _dimension &&
          (_dimension - 1 - col) < _dimension);
}

inline unsigned HexBoard::coord_to_index(unsigned row, unsigned col) const {
  return row * _dimension + col;
}

void HexBoard::flip_turn() {
  std::cout << "Changing " << _turn << "\n";
  if (_turn == CellColor::BLUE) {
    _turn = CellColor::RED;
  } else {
    _turn = CellColor::BLUE;
  }
}

CellColor HexBoard::flip_color(CellColor color) {
  if (color == CellColor::BLUE) {
    return CellColor::RED;
  }
  return CellColor::BLUE;
}
