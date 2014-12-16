/**************************************************************************
 Name:           Carlos Espinoza
 Date:           11/14/07
 Description:    Implementation for Othello class
****************************************************************************/
#include "othello.h"

// Constructor.
Othello::Othello(bool multi)
{
  b_count = 0;
  w_count = 0;
  forfeits = 0;
  multiplayer_mode = multi;
}

// Places a piece on the selected coordinates
void Othello::make_move (const string& move)
{
  if (move != "") {
    int row, col;
    convert(row,col,move);
    if (forfeits == 1) --forfeits;
    if (next_mover() == HUMAN) {
      board[row][col].set_black();
      ++b_count;
    }else {
      board[row][col].set_white();
      ++w_count;
    }
    process_flip(row,col);
  }else ++forfeits;
  game::make_move(move);
}

// Cleans out the board and sets it up for a new game.
// Resets all the counters.
void Othello::restart ()
{
  for (int r = 0; r < ROWS; ++r) {
    for (int c = 0; c < COLS; ++c) {
      if (r == c && (r == 3 || r == 4))
        board[r][c].set_white();
      else if ((r == 3 && c == 4) || (r == 4 && c == 3))
        board[r][c].set_black();
      else board[r][c].reset();
    }
  }
  b_count = w_count = 2;
  forfeits = 0;
  game::restart();
}

// Prints the board and the pieces on the board to the screen.
// Also outputs the current scores in the game.
void Othello::display_status() const
{
  system("clear");
  cout << BACK << YELLOW << " Othello - ";
  if(multiplayer()) cout << "Multiplayer.              " << endl;
  else cout << "Single Player.            " << endl;
  cout<<WHITE<<"     A   B   C   D   E   F   G   H   "<<endl;
  for (int r = 0; r < ROWS; ++r)
  { 
    cout<<GREEN<<"   --------------------------------- "<<endl;
    cout<<" "<<WHITE<<r+1<<" "<<BACK;
    for (int c = 0; c < COLS; ++c)
    {
      cout<<GREEN<<"| ";
	    if (board[r][c].is_white()) cout<<WHITE<<"\x23 ";
	    else if (board[r][c].is_black()) 
        cout<<RESET<<B_GREEN<<BLACK<<"\x23"<<BACK<<" ";
      else cout<<"  ";
    }
    cout<<GREEN<<"| "<<endl;
  }
  cout<<GREEN<<"   --------------------------------- "<<endl;
  if (next_mover() == HUMAN)
    cout<< RESET << B_GREEN << BLACK << "Black's turn";
  else cout << WHITE << "White's turn";
  cout<<BACK<<GREEN<<"   Black = "<<left<<setw(2)<<b_count
      <<" White = "<<left<<setw(2)<<w_count<<" "<<RESET<<endl;
  if (forfeits == 1) {
    if (last_mover() == HUMAN)
      cout<<YELLOW<<"Black player forfeited turn: No possible moves.\n"<<RESET;
    else
      cout<<YELLOW<<"White player forfeited turn: No possible moves.\n"<<RESET;
  }else if (!are_moves_available() && !is_game_over()) {
    if (next_mover() == HUMAN)
      cout<<YELLOW<<"BLACK MUST PASS: No possible moves. Press ENTER\n"<<RESET;
    else
      cout<<YELLOW<<"WHITE MUST PASS: No possible moves. Press ENTER\n"<<RESET;
  }
}

// Function checks for valid input first, if valid
// then it checks that the move is a legal move.
bool Othello::is_legal(const string& move) const
{
  if (move == "" && !are_moves_available()) return true;
  if (move == "q" || move == "Q") exit(99); //quit program
  // Check if input is valid or not.
  if (move.size() != 2) return false;
  string s1, s2;
  string legal_char = "abcdefghABCDEFGH";
  string legal_nums = "12345678";
  s1 = move.substr(0,1);
  s2 = move.substr(1);
  if ((s1.find_first_of(legal_char) == string::npos ||
       s2.find_first_of(legal_nums) == string::npos)&&
      (s1.find_first_of(legal_nums) == string::npos ||
       s2.find_first_of(legal_char) == string::npos))
    return false;
  // Convert string move to coordinates.
  int row, col;
  convert(row,col,move);
  // Now check that the piece isn't being placed on top of another.
  if (!board[row][col].is_empty()) return false;
  // Check if the move would flank the opposite color in any direction.
  return (is_it_flanked(row,col));
}

// Returns true if board is full or if black or white
// have no pieces on board. Otherwise, returns false.
bool Othello::is_game_over() const
{
  if (pieces_on_board() == 64) return true;
  if (w_count == 0 || b_count == 0) return true;
  if (!are_moves_available()) return (!(forfeits < 1));
  return false;
}

// Returns a copy of the current game.
game* Othello::clone( ) const
{
  return new Othello(*this);
}

// AI part of the project.
// Computes all the moves that the next player can make.
void Othello::compute_moves(queue<string>& moves) const
{
  for (int r = 0; r < ROWS; ++r) {
    for (int c = 0; c < COLS; ++c) {
      if (board[r][c].is_empty()) {
        if (is_it_flanked(r,c)) moves.push(coord_to_string(r,c));
      }
    }
  }
}

// AI part of the project.
// Evaluates a current board position and gives it a score
// Positive (+) values are GOOD for the COMPUTER. White player.
// Negative (-) values are good for human. Black player.
int Othello::evaluate( ) const
{
  int b_value = b_count;
  int w_value = w_count;
  int corner = 25;
  int x_sqr = 5;
  int stable = 10;

  for (int r = 0; r < ROWS; ++r) {
    for (int c = 0; c < COLS; ++c) {
      // Evaluate if the current square is not empty.
      if (!board[r][c].is_empty()) {

        //BEGIN: Check for X-squares before 35 moves
        if (moves_completed() < 20) {
          if (((r == c) && (r != 3 || r != 4)) || 
               (r == 1 && c == 6) || (r == 2 && c == 5) ||
               (r == 5 && c == 2) || (r == 6 && c == 1)) {
            if (board[r][c].is_black()) b_value += x_sqr;
            else w_value -= x_sqr;
          }
        } //END: X-squares.
        
        //BEGIN: Check for corners
        if (((r == 0 || r == 7) && r == c) ||
             (r == 0 && c == 7) ||
             (r == 7 && c == 0))  {
          if (board[r][c].is_black()) b_value += corner;
          else w_value += corner;
        } //END: Check corners

        //BEGIN: Check for piece stability.
        if (is_it_flanked(r,c)) {
          if (board[r][c].is_black()) w_value += stable;
          else b_value -= stable;
        }//END: stable pieces.

      }
    } //END for loop c.
  } //END for loop r.
  return (w_value - b_value);
}

// Checks to see if there any more moves available.
// Goes through every spot on the board and looks for empty ones.
// It performs a check on that spot to see if it would be a legal move.
bool Othello::are_moves_available() const
{
  for (int i = 0; i < 2; ++i) {
    for (int r = 0; r < ROWS; ++r) {
      for (int c = 0; c < COLS; ++c) {
        if (board[r][c].is_empty()) if (is_it_flanked(r,c)) return true;
      }
    }
  }
  return false;
}

// Checks to see if the move flanks a color in any direction.
// Returns false if no direction is flanked.
bool Othello::is_it_flanked(int r, int c) const
{
  if (is_ee_flanked(r,c)) return true;
  if (is_ne_flanked(r,c)) return true;
  if (is_nn_flanked(r,c)) return true;
  if (is_nw_flanked(r,c)) return true;
  if (is_ww_flanked(r,c)) return true;
  if (is_sw_flanked(r,c)) return true;
  if (is_ss_flanked(r,c)) return true;
  if (is_se_flanked(r,c)) return true;
  return false;
}

// Checks to see if the direction to the NORTH would
// flank any opposite pieces.
bool Othello::is_nn_flanked(int r, int c) const
{
  if (r == 0) return false;
  if (board[r-1][c].is_empty()) return false;
  int ro = 2;
  if (next_mover() == HUMAN) {
    if (board[r-1][c].is_black()) return false;
    while (r-ro >= 0 && !board[r-ro][c].is_empty()) {
      if (board[r-ro][c].is_black()) return true;
      ++ro;
    }
  }else {
    if (board[r-1][c].is_white()) return false;
    while (r-ro >= 0 && !board[r-ro][c].is_empty()) {
      if (board[r-ro][c].is_white()) return true;
      ++ro;
    }
  }
  return false;
}

// Checks to see if the direction to the NORTHEAST would
// flank any opposite pieces.
bool Othello::is_ne_flanked(int r, int c) const
{
  if (r == 0 || c == 7) return false;
  if (board[r-1][c+1].is_empty()) return false;
  int ro = 2, co = 2;
  if (next_mover() == HUMAN) {
    if (board[r-1][c+1].is_black()) return false;
    while (r-ro >= 0 && c+co <= 7 && !board[r-ro][c+co].is_empty()) {
      if (board[r-ro][c+co].is_black()) return true;
      ++ro; ++co;
    }
  }else {
    if (board[r-1][c+1].is_white()) return false;
    while (r-ro >= 0 && c+co <= 7 && !board[r-ro][c+co].is_empty()) {
      if (board[r-ro][c+co].is_white()) return true;
      ++ro; ++co;
    }
  }
  return false;
}

// Checks to see if the direction to the EAST would
// flank any opposite pieces.
bool Othello::is_ee_flanked(int r, int c) const
{
  if (c == 7) return false;
  if (board[r][c+1].is_empty()) return false;
  int co = 2;
  if (next_mover() == HUMAN) {
    if (board[r][c+1].is_black()) return false;
    while (c+co <= 7 && !board[r][c+co].is_empty()) {
      if (board[r][c+co].is_black()) return true;
      ++co;
    }
  }else {
    if (board[r][c+1].is_white()) return false;
    while (c+co <= 7 && !board[r][c+co].is_empty()) {
      if (board[r][c+co].is_white()) return true;
      ++co;
    }
  }
  return false;
}

// Checks to see if the direction to the SOUTHEAST would
// flank any opposite pieces.
bool Othello::is_se_flanked(int r, int c) const
{
  if (r == 7 || c == 7) return false;
  if (board[r+1][c+1].is_empty()) return false;
  int ro = 2, co = 2;
  if (next_mover() == HUMAN) {
    if (board[r+1][c+1].is_black()) return false;
    while (r+ro <= 7 && c+co <= 7 && !board[r+ro][c+co].is_empty()) {
      if (board[r+ro][c+co].is_black()) return true;
      ++ro; ++co;
    }
  }else {
    if (board[r+1][c+1].is_white()) return false;
    while (r+ro <= 7 && c+co <= 7 && !board[r+ro][c+co].is_empty()) {
      if (board[r+ro][c+co].is_white()) return true;
      ++ro; ++co;
    }
  }
  return false;
}

// Checks to see if the direction to the SOUTH would
// flank any opposite pieces.
bool Othello::is_ss_flanked(int r, int c) const
{
  if (r == 7) return false;
  if (board[r+1][c].is_empty()) return false;
  int ro = 2;
  if (next_mover() == HUMAN) {
    if (board[r+1][c].is_black()) return false;
    while (r+ro <= 7 && !board[r+ro][c].is_empty()) {
      if (board[r+ro][c].is_black()) return true;
      ++ro;
    }
  }else {
    if (board[r+1][c].is_white()) return false;
    while (r+ro <= 7 && !board[r+ro][c].is_empty()) {
      if (board[r+ro][c].is_white()) return true;
      ++ro;
    }
  }
  return false;
}

// Checks to see if the direction to the SOUTHWEST would
// flank any opposite pieces.
bool Othello::is_sw_flanked(int r, int c) const
{
  if (r == 7 || c == 0) return false;
  if (board[r+1][c-1].is_empty()) return false;
  int ro = 2, co = 2;
  if (next_mover() == HUMAN) {
    if (board[r+1][c-1].is_black()) return false;
    while (r+ro <= 7 && c-co >= 0 && !board[r+ro][c-co].is_empty()) {
      if (board[r+ro][c-co].is_black()) return true;
      ++ro; ++co;
    }
  }else {
    if (board[r+1][c-1].is_white()) return false;
    while (r+ro <= 7 && c-co >= 0 && !board[r+ro][c-co].is_empty()) {
      if (board[r+ro][c-co].is_white()) return true;
      ++ro; ++co;
    }
  }
  return false;
}

// Checks to see if the direction to the WEST would
// flank any opposite pieces.
bool Othello::is_ww_flanked(int r, int c) const
{
  if (c == 0) return false;
  if (board[r][c-1].is_empty()) return false;
  int co = 2;
  if (next_mover() == HUMAN) {
    if (board[r][c-1].is_black()) return false;
    while (c-co >= 0 && !board[r][c-co].is_empty()) {
      if (board[r][c-co].is_black()) return true;
      ++co;
    }
  }else {
    if (board[r][c-1].is_white()) return false;
    while (c-co >= 0 && !board[r][c-co].is_empty()) {
      if (board[r][c-co].is_white()) return true;
      ++co;
    }
  }
  return false;
}

// Checks to see if the direction to the NORTHWEST would
// flank any opposite pieces.
bool Othello::is_nw_flanked(int r, int c) const
{
  if (r == 0 || c == 0) return false;
  if (board[r-1][c-1].is_empty()) return false;
  int ro = 2, co = 2;
  if (next_mover() == HUMAN) {
    if (board[r-1][c-1].is_black()) return false;
    while (r-ro >= 0 && c-co >= 0 && !board[r-ro][c-co].is_empty()) {
      if (board[r-ro][c-co].is_black()) return true;
      ++ro; ++co;
    }
  }else {
    if (board[r-1][c-1].is_white()) return false;
    while (r-ro >= 0 && c-co >= 0 && !board[r-ro][c-co].is_empty()) {
      if (board[r-ro][c-co].is_white()) return true;
      ++ro; ++co;
    }
  }
  return false;
} 

// Checks ALL DIRECTIONS to see which pieces in any direction
// can be flipped. If the pieces can can be flipped, it calls
// function 'flip_angle' with the appropriate direction angle.
void Othello::process_flip(int r, int c)
{
  if (is_ee_flanked(r,c)) flip_angle(r,c+1,0);     //E  - 0 degrees
  if (is_ne_flanked(r,c)) flip_angle(r-1,c+1,45);  //NE - 45 degrees
  if (is_nn_flanked(r,c)) flip_angle(r-1,c,90);    //N  - 90 degrees
  if (is_nw_flanked(r,c)) flip_angle(r-1,c-1,135); //NW - 135 degrees
  if (is_ww_flanked(r,c)) flip_angle(r,c-1,180);   //W  - 180 degrees
  if (is_sw_flanked(r,c)) flip_angle(r+1,c-1,225); //SW - 225 degrees
  if (is_ss_flanked(r,c)) flip_angle(r+1,c,270);   //S  - 270 degrees
  if (is_se_flanked(r,c)) flip_angle(r+1,c+1,315); //SE - 315 degrees
}

// Handles all the pieces that need to be flipped on the
// GIVEN DIRECTION. It works recursively on the angle by
// checking which pieces can be flipped. If a piece can be
// flipped it sends its coordinates to 'flip_and_update'.
void Othello::flip_angle(int r, int c, int angle)
{
  int ro, co;
  if (angle == 0) { ro = 0; co = 1; }
  else if (angle == 45)  { ro = -1; co = 1; }
  else if (angle == 90)  { ro = -1; co = 0; }
  else if (angle == 135) { ro = -1; co = -1; }
  else if (angle == 180) { ro = 0; co = -1; }
  else if (angle == 225) { ro = 1; co = -1; }
  else if (angle == 270) { ro = 1; co = 0; }
  else if (angle == 315) { ro = 1; co = 1; }
  if (board[r][c] != board[r+ro][c+co]) {
    flip_and_update(r,c);
    return;
  }else{
    flip_angle(r+ro,c+co,angle);
    flip_and_update(r,c);
  }
}

// Flips any INDIVIDUAL PIECE that is sent to it.
// It also updates the piece counters to keep track of score.
void Othello::flip_and_update(const int r, const int c)
{
  board[r][c].flip();
  if (board[r][c].is_black()) {
    ++b_count;
    --w_count;
  }else {
    ++w_count;
    --b_count;
  }
}

// takes a string and the address of 2 ints. It converts
// the string to ints, and it also prepares them to
// be used to access the board array.
void convert(int& r, int& c, string move)
{
  char ch;
  istringstream iss(move);
  if ((move.substr(0,1)).find_first_of("12345678") == string::npos)
    iss >> ch >> r;
  else iss >> r >> ch;
  r -= 1;
  if (ch == 'a' || ch == 'A') c = 0;
  if (ch == 'b' || ch == 'B') c = 1;
  if (ch == 'c' || ch == 'C') c = 2;
  if (ch == 'd' || ch == 'D') c = 3;
  if (ch == 'e' || ch == 'E') c = 4;
  if (ch == 'f' || ch == 'F') c = 5;
  if (ch == 'g' || ch == 'G') c = 6;
  if (ch == 'h' || ch == 'H') c = 7;
}

// Takes a coordinate and returns its string equivalent.
// Must be sent the row first and then the column.
string coord_to_string(const int r, const int c)
{
  string rv;
  if (r == 0) rv = "1";
  else if (r == 1) rv = "2";
  else if (r == 2) rv = "3";
  else if (r == 3) rv = "4";
  else if (r == 4) rv = "5";
  else if (r == 5) rv = "6";
  else if (r == 6) rv = "7";
  else if (r == 7) rv = "8";
  if (c == 0) rv += "a";
  else if (c == 1) return (rv + "b");
  else if (c == 2) return (rv + "c");
  else if (c == 3) return (rv + "d");
  else if (c == 4) return (rv + "e");
  else if (c == 5) return (rv + "f");
  else if (c == 6) return (rv + "g");
  return (rv + "h");
}
