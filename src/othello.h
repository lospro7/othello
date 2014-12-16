/**************************************************************************
 Name:           Carlos Espinoza
 Date:           11/14/07
 Description:    Othello game class.
                   Many of the functions derive from the 'game' class.
                 It uses the Piece class to make an array of Pieces as
                 a representation of the board.
                   It has 8 boolean functions that check if any of the
                 moves that are about to be made would flank a line
                 of opposite colored pieces.
                   It has three other functions that handle the three
                 stages a piece goes through in order to be flipped.
                 - One handles all directions possible.
                 - The second handles the line of pieces (recursive).
                 - The last flips the individual pieces.
                 The AI part of Othello is handled by three functions:
                 - clone()
                 - compute_moves(queue<string>)
                 - evaluate()
                 The game can also be played in either single or
                 multiplayer mode, which is determined by member
                 variable 'multiplay'
****************************************************************************/
#ifndef OTHELLO_H
#define OTHELLO_H
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <queue>
#include <string>
#include <sstream>
#include "game.h"
#include "piece.h"
#include "colors.h"

class Piece;

class Othello : public game {
public:

  static const int ROWS = 8;
  static const int COLS = 8;

  // Constructor
  Othello(bool multi = false);

  // Overridden parent derived functions
  void make_move(const std::string& move);
  void restart();
  game* clone() const;
  void compute_moves(std::queue<std::string>& moves) const;
  void display_status() const;
  int evaluate() const;
  bool is_game_over() const;
  bool is_legal(const std::string& move) const;
  int get_score() const { return (w_count - b_count); }

  //Member variables
  int forfeits;            // Number of forfeits.

private:
  // Othello game functions
  int pieces_on_board() const { return (w_count + b_count); }
  bool multiplayer() const { return multiplayer_mode; }
  bool are_moves_available() const;

  // Calls all of the following flanked functions.
  bool is_it_flanked(int r, int c) const;
  // Check if direction is flanked functions.
  bool is_nn_flanked(int r, int c) const;
  bool is_ne_flanked(int r, int c) const;
  bool is_ee_flanked(int r, int c) const;
  bool is_se_flanked(int r, int c) const;
  bool is_ss_flanked(int r, int c) const;
  bool is_sw_flanked(int r, int c) const;
  bool is_ww_flanked(int r, int c) const;
  bool is_nw_flanked(int r, int c) const;

  // Flip handling functions.
  void process_flip(int r, int c);
  void flip_angle(int r, int c, int angle);
  void flip_and_update(const int r, const int c);

  // Member variable
  Piece board[ROWS][COLS]; // 2-D Board array.
  int b_count;             // Black pieces counter.
  int w_count;             // White pieces counter.
  bool multiplayer_mode;   // Multi-or-single player.
};

// convers a string to 2 ints.
void convert(int& r, int& c, string move);
// Takes a coordinate and returns its string equivalent.
string coord_to_string(const int r, const int c);

#endif
