/**************************************************************************
 Name:           Carlos Espinoza
 Date:           11/14/07
 Description:    Implementation for Piece class
****************************************************************************/
#include "piece.h"

// Constructor.
void Piece::flip()
{
  if (color == "B") color = "W";
  else color = "B";
}

// Returns true if the piece is blank
bool Piece::is_empty() const
{
  return (color == "");
}

// Returns true if the piece is black.
bool Piece::is_black() const
{
  return (color == "B");
}

// Returns true if the piece is white.
bool Piece::is_white() const
{
  return (color == "W");
}

// Operator != Compares two objects of type Piece.
bool Piece::operator!= (const Piece & rhs) const
{
  return (color != rhs.color);
}

// Operator == Compares two objects of type Piece.
bool Piece::operator== (const Piece & rhs) const
{
  return (color == rhs.color);
}
