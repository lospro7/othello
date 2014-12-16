/**************************************************************************
 Name:           Carlos Espinoza
 Date:           11/14/07
 Description:    Othello application.
                 Displays the main menu with game options.
                 Then prompts the user.
                 The user has the option to play single (unavailable)
                 or multiplayer.
                 At the end of a game the user is asked to play again.
                 Calls functions from 'game' and 'Othello' class to run.
****************************************************************************/
#include <iostream>
#include <cstdlib>
#include "game.h"
#include "othello.h"
#include "colors.h"
#include "piece.h"
using namespace std;

// Display main menu.
void display_menu();
// Prompt the user for a choice.
string get_choice();
// Plays one game of Othello.
void play_othello(bool multiplayer);
// Prompts the player to replay the game or not.
bool play_again();

int main()
{
  string ch = "";
  while (ch != "q" && ch != "Q")
  {
    display_menu();         // Display menu.
    ch = get_choice();      // Get users options
    if (ch == "1") {        // Play a one player game.
      do play_othello(false);
      while (play_again());
    }
    if (ch == "2") {        // Play a two player game.
      do play_othello(true);
      while (play_again());
    }
  }
  cout << RESET;
  return EXIT_SUCCESS;
}

// Display main menu.
void display_menu()
{
  system("clear");
  cout << YELLOW;
  cout << "Othello - Developed by Carlos Espinoza." << endl << endl;
  cout << "  To make a move enter a column letter and a" << endl;
  cout << "  row number, or vice versa. Enter Q anytime to quit." << endl;
  cout << "    Sample moves:  d3, 5f, 8H, e2, A6" << endl << endl;
  cout << "  Options :" << endl;
  cout << "   1 - 1 Player game." << endl;
  cout << "   2 - 2 Player game." << endl;
  cout << "   Q - Quit game." << endl << endl << RESET;
}

// Prompt the user for a choice.
string get_choice()
{
  string ch;
  cout << "  Select an option : ";
  getline(cin,ch);
  while (ch != "1" && ch != "2" && ch != "q" && ch != "Q") {
    cout << "  Select an option : ";
    getline(cin,ch);
  }
  return ch;
}

// Plays one game of Othello.
// It also announces the winner.
void play_othello(bool multiplayer)
{
  Othello othello(multiplayer);
  game::who winner;
  winner = othello.play();
  cout << YELLOW << " GAME OVER:";
  if (othello.forfeits > 0) cout << " No more moves available." << endl;
  if (winner == 0) cout << " Black Player wins!!!";
  else if (winner == 2) cout << " White Player wins!!!";
  else cout << " It's a TIE!";
  cout << endl << RESET;
}

// Prompts the player to replay the game or not.
bool play_again()
{
  string ch;
  cout << endl << " - Do you want to play again? [Y/N] ";
  getline(cin,ch);
  while(ch != "Y" && ch != "y" && ch != "N" && ch != "n")
  {
    cout << " - Do you want to play again? [Y/N] ";
    getline(cin,ch);
  }
  if (ch == "Y" || ch == "y") return true;
  return false;
}
