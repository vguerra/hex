HEX BOARD GAME
===

This is a C++ (using some C++11 features) implementation of the [HEX](http://en.wikipedia.org/wiki/Hex_(board_game)) board game.

Build it
===

Simply clone this repository and do:
```bash
make play
```
This will compile the sources (if needed) and start the game.

Play it
===
Before you are able to play, a couple of questions need to be answerd:

* "1 or 2 players?" : If you wish to play against the computer choose 1 player only.
* "Which figure you want to play with?" : In order to mark moves on the board, X or O (capital o) are available. Choose one.
* "Which figure should go first?": Once each player has a figure assigned, choose who should go first.
* Afterwards the board is displayed and players are ready to input their moves. The board looks by default like this:

Turn for shape: 'X'

                 X
      _________________________
      A  B  C  D  E  F  G  H  I
  \ 0  .  .  .  .  .  .  .  .  . 0 \
    \ 1  .  .  .  .  .  .  .  .  . 1 \
      \ 2  .  .  .  .  .  .  .  .  . 2 \
        \ 3  .  .  .  .  .  .  .  .  . 3 \
        O \ 4  .  .  .  .  .  .  .  .  . 4 \ O
            \ 5  .  .  .  .  .  .  .  .  . 5 \
              \ 6  .  .  .  .  .  .  .  .  . 6 \
                \ 7  .  .  .  .  .  .  .  .  . 7 \
                  \ 8  .  .  .  .  .  .  .  .  . 8 \
                        A  B  C  D  E  F  G  H  I
                        ------------------------
                                   X

Enter row(number) and column(letter) (e.g. 2A): _
