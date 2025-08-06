#include "BSAI24060_Board.h"
#include <raylib.h>
#include <string>

int main()
{
    InitWindow(1300, 850, "Chess Game");
    SetTargetFPS(60);

    bool gameStarted = false;
    bool showMenu = true;
    int choice = -1;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ONE)) choice = 1;
        else if (IsKeyPressed(KEY_TWO)) choice = 2;
        else if (IsKeyPressed(KEY_ZERO) || IsKeyPressed(KEY_ESCAPE)) choice = 0;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (showMenu)
        {
            DrawText("Chess Game", 300, 200, 40, BLACK);
            DrawText("1) New Game", 320, 300, 20, BLACK);
            DrawText("2) Load Game", 320, 330, 20, BLACK);
            DrawText("0) Exit Game", 320, 360, 20, BLACK);
        }

        EndDrawing();

        if (showMenu && choice != -1)
        {
            switch (choice)
            {
            case 1:
            {
                showMenu = false;
                Board game;
                game.Play();
                showMenu = true;
            }
            break;
            case 2:
            {
                showMenu = false;
                Board game("Chess_Load.txt");
                game.Play();
                showMenu = true;
            }
            break;
            case 0:
                CloseWindow();
                return 0;
            }

            choice = -1;
        }
    }

    CloseWindow();
    return 0;
}