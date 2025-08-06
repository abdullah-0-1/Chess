#include "BSAI24060_Board.h"
#include<iomanip>
#include"raylib.h"
#include<string>
#include<fstream>
Board::Board()
{
    init();
    ClearHighlight();
    BoardTexture = LoadTexture("Chess_Board.png");
}
Board::Board(const string& filename)
{
    LoadGame(filename);
    ClearHighlight();
    BoardTexture = LoadTexture("Chess_Board.png");
}

COLOR Board::changeTurn(COLOR t)
{
    return (t == PWHITE) ? PBLACK : PWHITE;
}
void Board::SelectPiece(int& Pr, int& Pc)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetMousePosition();
        int clickedRow = static_cast<int>(mousePos.y / 100);
        int clickedCol = static_cast<int>(mousePos.x / 100);

        if (clickedRow >= 0 && clickedRow < 8 && clickedCol >= 0 && clickedCol < 8)
        {
            Pr = clickedRow;
            Pc = clickedCol;
            return;
        }
    }
}

string Board::GetNameInput(const std::string& prompt)
{
    string name;
    bool done = false;
    while (!done && !WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(prompt.c_str(), 200, 300, 30, BLACK);
        DrawRectangle(200, 350, 400, 50, LIGHTGRAY);
        DrawText(name.c_str(), 210, 360, 30, BLACK);
        DrawText("Press ENTER to confirm", 200, 420, 20, DARKGRAY);
        EndDrawing();

        int key = GetCharPressed();
        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && name.length() < 20)
            {
                name += (char)key;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !name.empty())
        {
            name.pop_back();
        }

        if (IsKeyPressed(KEY_ENTER) && !name.empty())
        {
            done = true;
        }
    }
    return name;
}

void Board::init()
{
    //PBLACK
    B[0][0] = new Rook(PBLACK, { 0, 0 }, this), B[0][7] = new Rook(PBLACK, { 0, 7 }, this);
    B[0][1] = new Knight(PBLACK, { 0, 1 }, this), B[0][6] = new Knight(PBLACK, { 0, 6 }, this);
    B[0][2] = new Bishop(PBLACK, { 0, 2 }, this), B[0][5] = new Bishop(PBLACK, { 0, 5 }, this);
    B[0][3] = new Queen(PBLACK, { 0, 3 }, this), B[0][4] = new King(PBLACK, { 0, 4 }, this);

    //PWHITE
    B[7][0] = new Rook(PWHITE, { 7, 0 }, this), B[7][7] = new Rook(PWHITE, { 7, 7 }, this);
    B[7][1] = new Knight(PWHITE, { 7, 1 }, this), B[7][6] = new Knight(PWHITE, { 7, 6 }, this);
    B[7][2] = new Bishop(PWHITE, { 7, 2 }, this), B[7][5] = new Bishop(PWHITE, { 7, 5 }, this);
    B[7][3] = new Queen(PWHITE, { 7, 3 }, this), B[7][4] = new King(PWHITE, { 7, 4 }, this);


    for (int ri = 1; ri < 7; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (ri == 1)
            {
                B[ri][ci] = new Pawn(PBLACK, { ri,ci }, this);
            }
            else if (ri == 6)
            {
                B[ri][ci] = new Pawn(PWHITE, { ri,ci }, this);
            }
            else
            {
                B[ri][ci] = nullptr;
            }
        }
    }
    string name1 = GetNameInput("Enter name for White:");
    string name2 = GetNameInput("Enter name for Black:");

    Ps[0] = new Player(name1, PWHITE);
    Ps[1] = new Player(name2, PBLACK);
    turn = PWHITE;
}

void Board::DrawCapturedPieces()
{
    int xWhite = 850, yWhite = 50;
    int xBlack = 1050, yBlack = 50;
    int size = 60, pad = 10;
    DrawText("White Captured", xWhite, 20, 20, BLACK);
    for (size_t i = 0; i < WhiteCaptured.size(); ++i)
    {
        DrawTextureEx(WhiteCaptured[i]->GetTexture(), { (float)xWhite, (float)yWhite }, 0, 0.6f, WHITE);
        yWhite += size + pad;
    }
    DrawText("Black Captured", xBlack, 20, 20, BLACK);
    for (size_t i = 0; i < BlackCaptured.size(); ++i)
    {
        DrawTextureEx(BlackCaptured[i]->GetTexture(), { (float)xBlack, (float)yBlack }, 0, 0.6f, WHITE);
        yBlack += size + pad;
    }
}

void Board::DrawBoard()
{
    Rectangle src = { 0, 0, (float)BoardTexture.width, (float)BoardTexture.height };
    Rectangle dst = { 0, 0, 800, 800 };
    Vector2 origin = { 0.0f, 0.0f };

    DrawTexturePro(BoardTexture, src, dst, origin, 0.0f, WHITE);
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (B[ri][ci])
            {
                B[ri][ci]->draw();
            }
        }
    }
    DrawCapturedPieces();
}

bool Board::IsValidSelection(int r, int c)
{
    Vector2 mousePos = GetMousePosition();
    int gridR = static_cast<int>(mousePos.y / 100);
    int gridC = static_cast<int>(mousePos.x / 100);
    return gridR == r && gridC == c && r >= 0 && r < 8 && c >= 0 && c < 8 &&
        B[r][c] && B[r][c]->GetClr() == turn;
}

bool Board::IsValidDestination(int sr, int sc, int er, int ec)
{
    Vector2 mousePos = GetMousePosition();
    int gridEr = static_cast<int>(mousePos.y / 100);
    int gridEc = static_cast<int>(mousePos.x / 100);
    return gridEr == er && gridEc == ec && er >= 0 && er < 8 && ec >= 0 && ec < 8 &&
        (!B[er][ec] || B[er][ec]->GetClr() != B[sr][sc]->GetClr());
}

bool Board::IsValidDestinationLogic(int sr, int sc, int er, int ec)
{
    return er >= 0 && er < 8 && ec >= 0 && ec < 8 &&
        (!B[er][ec] || B[er][ec]->GetClr() != B[sr][sc]->GetClr());
}

void Board::MovePiece(int sr, int sc, int er, int ec)
{
    if (B[er][ec] != nullptr)
    {
        if (B[er][ec]->GetClr() == PWHITE)
        {
            WhiteCaptured.push_back(B[er][ec]);
        }
        else
        {
            BlackCaptured.push_back(B[er][ec]);
        }
        //delete B[er][ec];
    }
    B[er][ec] = B[sr][sc];
    B[sr][sc] = nullptr;
    B[er][ec]->SetPosition({ er, ec });
}

Piece* Board::PieceAt(Position P)const
{
    return B[P.Pr][P.Pc];
}

void Board::ClearHighlight()
{
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            HB[ri][ci] = '-';
        }
    }
}

void Board::ComputeAndPrintHighlight(int sr, int sc)
{
    selectedRow = sr;
    selectedCol = sc;

    ClearHighlight();

    if (!B[sr][sc] || B[sr][sc]->GetClr() != turn)
        return;

    DrawRectangle(sc * 100, sr * 100, 100, 100, Fade(YELLOW, 0.6f));

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (r == sr && c == sc)
            {
                continue;
            }

            Position E = { r, c };
            bool isValidMove = false;

            if (B[sr][sc]->AmIKing() && B[r][c] &&
                ((turn == PWHITE && B[r][c]->GetSymbol() == 'r') ||
                    (turn == PBLACK && B[r][c]->GetSymbol() == 'R')))
            {
                if (IsCasteling({ sr, sc }, { r, c }))
                {
                    DrawRectangle(c * 100, r * 100, 100, 100, Fade(BLUE, 0.5f));
                    if (B[r][c])
                        B[r][c]->draw();

                    HB[r][c] = 'y';
                    continue;
                }
            }

            if (IsValidDestinationLogic(sr, sc, r, c) &&
                B[sr][sc]->IsLegal(E) &&
                !IsSelfCheck(turn, sr, sc, r, c))
            {
                isValidMove = true;
            }

            if (isValidMove)
            {
                if (B[r][c] && B[r][c]->GetClr() != turn)
                {
                    DrawRectangle(c * 100, r * 100, 100, 100, Fade(RED, 0.5f));
                    B[r][c]->draw();
                    HB[r][c] = 'k';
                }
                else if (!B[r][c])
                {
                    DrawRectangle(c * 100, r * 100, 100, 100, Fade(GREEN, 0.4f));
                    HB[r][c] = 'x';
                }
            }
        }
    }

    // Redraw
    if (B[sr][sc])
    {
        B[sr][sc]->draw();
    }
}


void Board::KingFinding(COLOR t, int& kr, int& kc)
{
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (B[ri][ci] && B[ri][ci]->GetClr() == t && B[ri][ci]->AmIKing())
            {
                kr = ri;
                kc = ci;
                return;
            }
        }
    }
}

bool Board::IsCheck(COLOR t)
{
    int kr, kc;
    t = changeTurn(t);
    KingFinding(t, kr, kc);
    t = changeTurn(t);
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (IsValidSelection(ri, ci))
            {
                Position kingPos = { kr, kc };
                if (B[ri][ci]->IsLegal(kingPos))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::IsSelfCheck(COLOR t, int sr, int sc, int er, int ec)
{
    Piece* OrigDPiece = B[er][ec];
    Piece* movingPiece = B[sr][sc];

    //temporary move
    B[er][ec] = movingPiece;
    B[sr][sc] = nullptr;
    if (movingPiece)
    {
        movingPiece->SetPosition({ er, ec });
    }

    int kr, kc;
    KingFinding(t, kr, kc);
    COLOR oppTurn = changeTurn(t);
    bool isInCheck = false;

    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (B[ri][ci] && B[ri][ci]->GetClr() == oppTurn)
            {
                Position kingPos = { kr, kc };
                if (B[ri][ci]->IsLegal(kingPos))
                {
                    isInCheck = true;
                    break;
                }
            }
        }
        if (isInCheck) break;
    }

    // Restore
    B[sr][sc] = movingPiece;
    B[er][ec] = OrigDPiece;
    if (movingPiece)
    {
        movingPiece->SetPosition({ sr, sc });
    }

    return isInCheck;
}


bool Board::IsCheckmate(COLOR t)
{

    if (!IsCheck(t))
    {
        return false;
    }
    t = changeTurn(t);

    for (int sr = 0; sr < 8; sr++)
    {
        for (int sc = 0; sc < 8; sc++)
        {
            if (B[sr][sc] && B[sr][sc]->GetClr() == t)
            {
                for (int er = 0; er < 8; er++)
                {
                    for (int ec = 0; ec < 8; ec++)
                    {
                        if (IsValidDestinationLogic(sr, sc, er, ec) &&
                            B[sr][sc]->IsLegal({ er, ec }) &&
                            !IsSelfCheck(t, sr, sc, er, ec))
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool Board::IsStalemate(COLOR t)
{

    if (IsCheck(t))
    {
        return false;
    }
    t = changeTurn(t);

    for (int sr = 0; sr < 8; sr++)
    {
        for (int sc = 0; sc < 8; sc++)
        {
            if (B[sr][sc] && B[sr][sc]->GetClr() == t)
            {
                for (int er = 0; er < 8; er++)
                {
                    for (int ec = 0; ec < 8; ec++)
                    {
                        if (IsValidDestinationLogic(sr, sc, er, ec) &&
                            B[sr][sc]->IsLegal({ er, ec }) &&
                            !IsSelfCheck(t, sr, sc, er, ec))
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

void Board::SaveGame(const string& filename)
{
    ofstream wtr(filename);
    if (!wtr)
    {
        cerr << "Error: Could not open file for saving." << endl;
        return;
    }

    wtr << Ps[0]->GetName() << endl;
    wtr << Ps[1]->GetName() << endl;

    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (B[ri][ci])
            {
                wtr << B[ri][ci]->GetSymbol();
            }
            else {
                wtr << '-';
            }
        }
        wtr << endl;
    }
    turn = changeTurn(turn);
    wtr << turn << endl;
    turn = changeTurn(turn);

    wtr.close();
}

void Board::LoadGame(const string& filename)
{
    ifstream rdr(filename);
    if (!rdr)
    {
        cerr << "Error: Could not open file for loading." << endl;
        return;
    }

    for (int i = 0; i < 2; ++i)
    {
        string name;
        rdr >> name;
        Ps[i] = new Player(name, i == 0 ? PWHITE : PBLACK);
    }

    char symbol;
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            rdr >> symbol;
            CreatePieceFromSymbol(ri, ci, symbol);
        }
    }

    int turnValue;
    rdr >> turnValue;
    turn = static_cast<COLOR>(turnValue);

    rdr.close();
    cout << "Game loaded successfully!" << endl;
}

void Board::CreatePieceFromSymbol(int ri, int ci, char symbol)
{
    Position pos = { ri, ci };

    switch (symbol)
    {
        //PWHITE
    case 'r':
        B[ri][ci] = new Rook(PWHITE, pos, this);
        break;
    case 'h':
        B[ri][ci] = new Knight(PWHITE, pos, this);
        break;
    case 'b':
        B[ri][ci] = new Bishop(PWHITE, pos, this);
        break;
    case 'q':
        B[ri][ci] = new Queen(PWHITE, pos, this);
        break;
    case 'k':
        B[ri][ci] = new King(PWHITE, pos, this);
        break;
    case 'p':
        B[ri][ci] = new Pawn(PWHITE, pos, this);
        break;

        //PBLACK
    case 'R':
        B[ri][ci] = new Rook(PBLACK, pos, this);
        break;
    case 'H':
        B[ri][ci] = new Knight(PBLACK, pos, this);
        break;
    case 'B':
        B[ri][ci] = new Bishop(PBLACK, pos, this);
        break;
    case 'Q':
        B[ri][ci] = new Queen(PBLACK, pos, this);
        break;
    case 'K':
        B[ri][ci] = new King(PBLACK, pos, this);
        break;
    case 'P':
        B[ri][ci] = new Pawn(PBLACK, pos, this);
        break;
    case '-':
        B[ri][ci] = nullptr;
        break;
    }
}


bool Board::IsCasteling(Position K, Position R)
{
    if (!B[K.Pr][K.Pc]->AmIKing())
    {
        return false;
    }

    Rook* rook = dynamic_cast<Rook*>(B[R.Pr][R.Pc]);
    if (!rook || B[R.Pr][R.Pc]->GetClr() != turn)
    {
        return false;
    }

    if (K.Pr != R.Pr)
    {
        return false;
    }

    int start = min(K.Pc, R.Pc) + 1;
    int end = max(K.Pc, R.Pc) - 1;

    for (int ci = start; ci <= end; ci++)
    {
        if (B[K.Pr][ci] != nullptr)
        {
            return false;
        }
    }

    if (K.Pc < R.Pc)
    {
        for (int i = 1; i <= 2; i++)
        {
            if (IsSelfCheck(turn, K.Pr, K.Pc, K.Pr, K.Pc - i))
            {
                return false;
            }
        }
    }
    else
    {
        for (int i = 1; i <= 2; i++)
        {
            if (IsSelfCheck(turn, K.Pr, K.Pc, K.Pr, K.Pc - i))
            {
                return false;
            }
        }
    }
    return true;
}

void Board::Casteling(Position K, Position R)
{
    bool IsCastleKingSide = (R.Pc > K.Pc);
    int NewKc, NewRc;
    if (IsCastleKingSide)
    {
        NewKc = K.Pc + 2;
        NewRc = K.Pc + 1;
    }
    else
    {
        NewKc = K.Pc - 2;
        NewRc = K.Pc - 1;
    }

    if (B[K.Pr][NewKc] != nullptr)
    {
        delete B[K.Pr][NewKc];
    }

    if (B[R.Pr][NewRc] != nullptr)
    {
        delete B[R.Pr][NewRc];
    }
    B[K.Pr][NewKc] = B[K.Pr][K.Pc];
    B[K.Pr][K.Pc] = nullptr;
    B[K.Pr][NewKc]->SetPosition({ K.Pr,NewKc });

    B[R.Pr][NewRc] = B[R.Pr][R.Pc];
    B[R.Pr][R.Pc] = nullptr;
    B[R.Pr][NewRc]->SetPosition({ R.Pr,NewRc });
}

bool Board::IsValidSym(char sym)
{
    if (turn == PWHITE && (sym == 'q' || sym == 'r' || sym == 'b' || sym == 'h'))
    {
        return true;
    }
    else if (turn == PBLACK && (sym == 'Q' || sym == 'R' || sym == 'B' || sym == 'H'))
    {
        return true;
    }
    return false;
}

void Board::Promote(int r, int c, COLOR pColor, char sym) {
    if (B[r][c] != nullptr)
    {
        delete B[r][c];
    }

    Position pos = { r, c };

    switch (sym)
    {
    case 'q':
        B[r][c] = new Queen(PWHITE, pos, this);
        break;
    case 'r':
        B[r][c] = new Rook(PWHITE, pos, this);
        break;
    case 'b':
        B[r][c] = new Bishop(PWHITE, pos, this);
        break;
    case 'h':
        B[r][c] = new Knight(PWHITE, pos, this);
        break;

    case 'Q':
        B[r][c] = new Queen(PBLACK, pos, this);
        break;
    case 'R':
        B[r][c] = new Rook(PBLACK, pos, this);
        break;
    case 'B':
        B[r][c] = new Bishop(PBLACK, pos, this);
        break;
    case 'H':
        B[r][c] = new Knight(PBLACK, pos, this);
        break;
    }
}

void Board::PawnPromotion()
{
   char sym = ' ';
   if (turn == PWHITE)
   {
       for (int ci = 0; ci < 8; ci++)
       {
           if (B[0][ci] && B[0][ci]->GetSymbol() == 'p')
           {
               bool promoted = false;
               while (!promoted && !WindowShouldClose())
               {
                   BeginDrawing();
                   ClearBackground(RAYWHITE);
                   DrawBoard();
                   DrawRectangle(850, 100, 200, 200, Fade(BLACK, 0.8f));
                   DrawText("Promote Pawn to:", 870, 120, 20, WHITE);
                   DrawText("q: Queen", 870, 150, 18, WHITE);
                   DrawText("r: Rook", 870, 170, 18, WHITE);
                   DrawText("b: Bishop", 870, 190, 18, WHITE);
                   DrawText("h: Knight", 870, 210, 18, WHITE);
                   EndDrawing();

                   if (IsKeyPressed(KEY_Q))
                   {
                       sym = 'q'; promoted = true;
                   }
                   if (IsKeyPressed(KEY_R))
                   {
                       sym = 'r'; promoted = true;
                   }
                   if (IsKeyPressed(KEY_B))
                   {
                       sym = 'b'; promoted = true;
                   }
                   if (IsKeyPressed(KEY_H))
                   {
                       sym = 'h'; promoted = true;
                   }
               }
               if (IsValidSym(sym))
               {
                   Promote(0, ci, PWHITE, sym);
               }
               return;
           }
       }
   }
   else if (turn == PBLACK)
   {
       for (int ci = 0; ci < 8; ci++)
       {
           if (B[7][ci] && B[7][ci]->GetSymbol() == 'P')
           {
               bool promoted = false;
               while (!promoted && !WindowShouldClose())
               {
                   BeginDrawing();
                   ClearBackground(RAYWHITE);
                   DrawBoard();
                   DrawRectangle(850, 100, 200, 200, Fade(BLACK, 0.8f));
                   DrawText("Promote Pawn to:", 870, 120, 20, WHITE);
                   DrawText("Q: Queen", 870, 150, 18, WHITE);
                   DrawText("R: Rook", 870, 170, 18, WHITE);
                   DrawText("B: Bishop", 870, 190, 18, WHITE);
                   DrawText("H: Knight", 870, 210, 18, WHITE);
                   EndDrawing();

                   if (IsKeyPressed(KEY_Q))
                   {
                       sym = 'Q'; promoted = true;
                   }
                   if (IsKeyPressed(KEY_R))
                   {
                       sym = 'R'; promoted = true;
                   }
                   if (IsKeyPressed(KEY_B))
                   {
                       sym = 'B'; promoted = true;
                   }
                   if (IsKeyPressed(KEY_H))
                   {
                       sym = 'H'; promoted = true;
                   }
               }
               if (IsValidSym(sym))
               {
                   Promote(7, ci, PBLACK, sym);
               }
               return;
           }
       }
   }
}

void Board::SetBoardTexture(Texture2D tex)
{
    BoardTexture = tex;
}
Texture2D Board::GetBoardTexture() const
{
    return BoardTexture;
}

void Board::SaveForReplay()
{
    vector<vector<char>> state(8, vector<char>(8, '-'));
    for (int ri = 0; ri < 8; ++ri)
    {
        for (int ci = 0; ci < 8; ++ci)
        {
            if (B[ri][ci])
            {
                state[ri][ci] = B[ri][ci]->GetSymbol();
            }
            else
            {
                state[ri][ci] = '-';
            }
        }
    }
    ReplayState.push_back(state);
}

void Board::ShowReplay()
{
    for (const auto& state : ReplayState)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Rectangle src = { 0, 0, (float)BoardTexture.width, (float)BoardTexture.height };
        Rectangle dst = { 0, 0, 800, 800 };
        Vector2 origin = { 0.0f, 0.0f };
        DrawTexturePro(BoardTexture, src, dst, origin, 0.0f, WHITE);

        for (int ri = 0; ri < 8; ++ri)
        {
            for (int ci = 0; ci < 8; ++ci)
            {
                char sym = state[ri][ci];
                if (sym != '-')
                {
                    Piece* tempPiece = nullptr;
                    Position pos = { ri, ci };
                    switch (sym)
                    {
                        // White
                    case 'r':
                        tempPiece = new Rook(PWHITE, pos, this); 
                        break;
                    case 'h':
                        tempPiece = new Knight(PWHITE, pos, this); 
                        break;
                    case 'b':
                        tempPiece = new Bishop(PWHITE, pos, this); 
                        break;
                    case 'q':
                        tempPiece = new Queen(PWHITE, pos, this); 
                        break;
                    case 'k': 
                        tempPiece = new King(PWHITE, pos, this); 
                        break;
                    case 'p': 
                        tempPiece = new Pawn(PWHITE, pos, this); 
                        break;
                        // Black
                    case 'R': 
                        tempPiece = new Rook(PBLACK, pos, this); 
                        break;
                    case 'H': 
                        tempPiece = new Knight(PBLACK, pos, this); 
                        break;
                    case 'B': 
                        tempPiece = new Bishop(PBLACK, pos, this); 
                        break;
                    case 'Q':
                        tempPiece = new Queen(PBLACK, pos, this); 
                        break;
                    case 'K':
                        tempPiece = new King(PBLACK, pos, this); 
                        break;
                    case 'P': 
                        tempPiece = new Pawn(PBLACK, pos, this); 
                        break;
                    }
                    if (tempPiece)
                    {
                        tempPiece->draw();
                        delete tempPiece;
                    }
                }
            }
        }

        DrawText("Replay...", 850, 400, 30, BLACK);
        EndDrawing();
        WaitTime(1.2);
    }
}

Board::~Board()
{
    for (int ri = 0; ri < 8; ++ri)
    {
        for (int ci = 0; ci < 8; ++ci)
        {
            if (B[ri][ci])
            {
                delete B[ri][ci];
                B[ri][ci] = nullptr;
            }
        }
    }

    for (auto* piece : WhiteCaptured)
    {
        delete piece;
    }
    WhiteCaptured.clear();

    for (auto* piece : BlackCaptured)
    {
        delete piece;
    }
    BlackCaptured.clear();

    delete Ps[0];
    delete Ps[1];
    Ps[0] = Ps[1] = nullptr;

    UnloadTexture(BoardTexture);
}


void Board::Play()
{
    Position S = { -1, -1 }, D = { -1, -1 };

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawBoard();
        DrawText((Ps[turn]->GetName() + "'s Turn").c_str(), 10, 810, 20, BLACK);
        do {
            do
            {
                ClearHighlight();
                do
                {
                    S.Pr = -1;
                    S.Pc = -1;
                    SelectPiece(S.Pr, S.Pc);
                    EndDrawing();
                    BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawBoard();
                    DrawText((Ps[turn]->GetName() + "'s Turn").c_str(), 10, 810, 20, BLACK);

                    if (WindowShouldClose())
                    {
                        return;
                    }
                } while (!IsValidSelection(S.Pr, S.Pc));

                ComputeAndPrintHighlight(S.Pr, S.Pc);

                D.Pr = -1;
                D.Pc = -1;
                do
                {
                    SelectPiece(D.Pr, D.Pc);
                    EndDrawing();
                    BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawBoard();
                    ComputeAndPrintHighlight(S.Pr, S.Pc);
                    DrawText((Ps[turn]->GetName() + "'s Turn").c_str(), 10, 810, 20, BLACK);

                    if (WindowShouldClose())
                    {
                        return;
                    }

                    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
                    {
                        S.Pr = -1;
                        S.Pc = -1;
                        break;
                    }
                } while (D.Pr == -1 || D.Pc == -1);

                if (S.Pr == -1 || S.Pc == -1)
                {
                    continue;
                }

            } while (!IsValidDestination(S.Pr, S.Pc, D.Pr, D.Pc) && !IsCasteling(S, D));
        } while (!B[S.Pr][S.Pc]->IsLegal(D) && !IsCasteling(S, D) || IsSelfCheck(turn, S.Pr, S.Pc, D.Pr, D.Pc));

        if (IsCasteling(S, D))
        {
            Casteling(S, D);
        }
        else
        {
            MovePiece(S.Pr, S.Pc, D.Pr, D.Pc);
        }

            PawnPromotion();


        
        SaveForReplay();


        if (IsCheckmate(turn))
        {
            DrawText("CHECKMATE!", 850, 100, 50, RED);
            EndDrawing();
            WaitTime(2.0);
            break;
        }
        else if (IsCheck(turn))
        {
            turn = changeTurn(turn);
            DrawText((Ps[turn]->GetName() + " in Check").c_str(), 850, 100, 35, RED);   //300
            EndDrawing();
            WaitTime(1.0);
            BeginDrawing();
            turn = changeTurn(turn);
        }
        else if (IsStalemate(turn))
        {
            DrawText("STALEMATE! Draw", 800, 100, 40, BLUE);
            EndDrawing();
            WaitTime(2.0);
            break;
        }

        SaveGame("Chess_Load.txt");
        turn = changeTurn(turn);
        ClearHighlight();

        EndDrawing();
    }

    bool askReplay = false;
    while (!askReplay && !WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Game Over! Watch replay? (Y/N)", 300, 400, 30, BLACK);
        EndDrawing();
        if (IsKeyPressed(KEY_Y)) 
        { 
            askReplay = true;
            ShowReplay(); 
        }
        if (IsKeyPressed(KEY_N)) 
        {
            askReplay = true; 
        }
    }
}
