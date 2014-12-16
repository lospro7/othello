/**************************************************************************
 Name:           Carlos Espinoza
 Date:           11/14/07
 Description:    Piece class.
                 Othello board pieces.
                 A piece can be of color "W" for white or "B" for black.
                 A piece can also be "empty" which basically means that
                 the piece is not on the board.
****************************************************************************/
#ifndef PIECE_H
#define PIECE_H
#include <string>

class Piece {
public:
  // constructor
  Piece() { color = ""; }
  // Modifiers
  void flip();
  void set_black() { color = "B"; }
  void set_white() { color = "W"; }
  void reset() { color = ""; }
  // Accessor
  bool is_empty() const;
  bool is_black() const;
  bool is_white() const;
  std::string get_color() const { return color; };
  // Operators
  bool operator!= (const Piece & rhs) const;
  bool operator== (const Piece & rhs) const;

  private:
  std::string color;
};

#endif
