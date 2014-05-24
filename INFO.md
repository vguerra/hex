HEX Board Game -- Minimun Spanning Tree
========================================

Get the code running: 
---------------------

The first step is to uncompress the zip you downloaded 
from coursera. You get a folder containing the code, 
a Makefile and some text files. 

For this homework the files containing the relevant code are: 

hexcell.h: Definitions for representing Board Cells.
hexcell.cpp: Implementation of classes on hexcell.h
hexboard.h: Relevant definitions for representing a Board.
hexboard.cpp: Implementation of classes on hexboard.h
hex.h: Definitions for the game in general.
hex.cpp: Handling of argument for initializing the game. Input/Outpu handling. 
util.h: Extra utilities.

In order to compile the code successfully you have to 
use a recent compiler that supports c++11 (I tested with latest version of clang++).

Once you get a compiler that supports the c++11 standard simply run from your terminal: 

$> make 

That should compile the code and start the game using a 9x9 board.


Implementation details: 
-----------------------

There are several classes that represent the objects used for the implementation of the game: 

- A class that represents a board-cell ('HexCell'), which simply encapsulates what color the 
cell has and logic for read/write the color and priting the Cell to an output stream. 

- A class that represents a board ('HexBoard'). This class is a representation of the board, 
which consists of NxN cells. Internally the matrix of cells is representad by a vector of cells which 
at the initialization moment are marked as empty. The indexes used as row and column in the matrix,
are used to calculate the index where a given cell can be found in the vector. Four special cells 
are added to the vector, which represent the four cardinal points ( north , south, west, east). 
The purpose of this 4 cells is mentioned ahead. This class also has a graph which is used (at the moment) 
to test connectivity in order to tell wheter a user has won the game or not. 
Each node represents a cell in the board. Each edge indicates the contiguity of same-colored cells. 
In order words if the cell in position i,j is adjacent to the cell i,j+1 and both are coloured BLUE, 
then an edge is created in the graph. There are also members that track who's turn it is and
whether the game has come to an end. 

Now, in order to register a move, some checks have to be done. First, the coordinates have to be 
correct (within range) and secondly the cell has to be empty. Once the move is verified it is processed. 
When processing the move the neighborhood of the cell is checked in order to create the corresponding
edges between cells, and there is where the extra cells repesenting the 4 cardinal points come 
into play: All of the moves happening at the boarder of the board are handled specially, in the sense 
that cells are connected to their corresponding cardinal point (the colour is taken into account for this matter).
If the BLUE player has to connect the North and South boarders, then any cells on the marked as BLUE at the 
south and north boarder are going to be connected with their respective speciall cells. This facilitates
the following procedure, which is checking if there is a winner. 

In order to determine if there is already a winner, we run on the graph we have been updating along the game, 
the MST algorithm. The goal is to prove connectivete either between the South and North cells, or the West and 
East cells. The source used for the MST depends on which color did the last play, if a move for the BLUE 
color is registered, then the West cell is used as source, otherwise we use the North cell. The points to which 
we test connectivity are adjusted also accordingly (East cell when it is BLUE's turn and South cell when RED's).
If connectivity is proven then a winner is announced and the game ends. 

- Before starting the game, a couple of questions are asked to the user regarding configuration. Whether 
to play against the computer or another human. If user chooses to play against the computer then the moves
the computer makes are calculated through a simple Monte Carlo simulation of 10,000 tries that determines
what is probalistically the best move to make. Which player wins is determined as mentioned before, checking
connectivity of the cardinal points depending on the player on turn. 

