#include "BSAI24060_Board.h"
using namespace std;

void Choice(int& choice)
{
    cout << "Enter your choice\n"
        << "1) New Game\n"
        << "2) Load Game\n"
        << "0) Exit Game\n";
    cin >> choice;
}

int main()
{
    int choice;
    Choice(choice);

    switch (choice)
    {
    case 1:
    {
        Board game;
        game.Play();
        break;
    }
    case 2:
    {
        Board game("Chess_Load.txt");
        game.Play();
        break;
    }
    case 0:
    {
        cout << "Game Ended";
        return 0;
    }
    default:
    {
        cout << "Invalid choice! Please try again.";
        return 1;
    }
    }

    return 0;
}