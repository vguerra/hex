// Victor Guerra <vguerra@gmail.com>
// Nov 2014

#include "hex.h"
#include "hexboard.h"

void print_help() {
  std::cout << " Usage: ./hex [-d <dimensions = 9> ]" << std::endl;
}

int main(int argc, char* argv[]) {
  unsigned dimensions = 0;

  if (argc == 1) {
    dimensions = 9;
  } else if (argc == 3 && !strcmp(argv[1], "-d")) {
    dimensions = static_cast<unsigned>(atoi(argv[2]));
  } else {
    print_help();
  }

  HexBoard board(dimensions);

  // Run while loop until user does not want to play anymore.
  board.Init();
  board.play();

  return EXIT_SUCCESS;
}
