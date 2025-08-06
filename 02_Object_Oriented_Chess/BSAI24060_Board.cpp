#include "BSAI24060_Board.h"
#include<iomanip>
#include<fstream>
Board::Board()
{
    init();
    ClearHighlight();
}
Board ::Board(const string& filename)
{
    LoadGame(filename);
    ClearHighlight();
}

COLOR Board::changeTurn(COLOR t)
{
    return (t == WHITE) ? BLACK : WHITE;
}
void Board::SelectPiece(int& Pr, int& Pc)
{
    getRowColbyLeftClick(Pr, Pc);
    Pc /= 2;
}
void Board::init()
{
    //BLACK
    B[0][0] = new Rook(BLACK, { 0, 0 }, this), B[0][7] = new Rook(BLACK, { 0, 7 }, this);
    B[0][1] = new Knight(BLACK, { 0, 1 }, this), B[0][6] = new Knight(BLACK, { 0, 6 }, this);
    B[0][2] = new Bishop(BLACK, { 0, 2 }, this), B[0][5] = new Bishop(BLACK, { 0, 5 }, this);
    B[0][3] = new Queen(BLACK, { 0, 3 }, this), B[0][4] = new King(BLACK, { 0, 4 }, this);

    //WHITE
    B[7][0] = new Rook(WHITE, { 7, 0 }, this), B[7][7] = new Rook(WHITE, { 7, 7 }, this);
    B[7][1] = new Knight(WHITE, { 7, 1 }, this), B[7][6] = new Knight(WHITE, { 7, 6 }, this);
    B[7][2] = new Bishop(WHITE, { 7, 2 }, this), B[7][5] = new Bishop(WHITE, { 7, 5 }, this);
    B[7][3] = new Queen(WHITE, { 7, 3 }, this), B[7][4] = new King(WHITE, { 7, 4 }, this);


    for (int ri = 1; ri < 7; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (ri == 1)
            {
                B[ri][ci] = new Pawn(BLACK, { ri,ci }, this);
            }
            else if (ri == 6)
            {
                B[ri][ci] = new Pawn(WHITE, { ri,ci }, this);
            }
            else
            {
                B[ri][ci] = nullptr;
            }
        }
    }
    string name1, name2;
    cout << "Enter name for White: ";
    cin >> name1;
    cout << "Enter name for Black: ";
    cin >> name2;

    Ps[0] = new Player(name1, WHITE);
    Ps[1] = new Player(name2, WHITE);
    turn = WHITE;
}
void Board::DrawBoard()
{
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (B[ri][ci])
            {
                B[ri][ci]->draw();
            }
            else
            {
                cout << '-';
            }
            cout << " ";
        }
        cout << endl;
    }
}
bool Board::IsValidSelection(int r, int c)
{
    return B[r][c] && B[r][c]->GetClr() == turn;  //B[r][c] if nullptr then wrong selection
}

bool Board::IsValidDestination(int sr, int sc, int er, int ec) {
    return er >= 0 && er < 8 && ec >= 0 && ec < 8 &&
        (!B[er][ec] || B[er][ec]->GetClr() != B[sr][sc]->GetClr()); //!B[er][ec] if nullptr then true
}

void Board::MovePiece(int sr, int sc, int er, int ec)
{
    if (B[er][ec] != nullptr) 
    {
        delete B[er][ec];
    }
    B[er][ec] = B[sr][sc];
    B[sr][sc] = nullptr;
    B[er][ec]->SetPosition({ er, ec });
    //ClearHighlight();
}

Piece* Board::PieceAt(Position P)const
{
    return B[P.Pr][P.Pc];
}

void Board::ClearHighlight()
{
    // Initialize all cells with no highlight
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            HB[ri][ci] = '-';
        }
    }
}

void Board::ComputeHighlight(int sr, int sc)
{

    ClearHighlight();

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Position E = { r, c };
            if (IsValidDestination(sr, sc, r, c) && B[sr][sc]->IsLegal(E) && !IsSelfCheck(turn,sr,sc,r,c))
            {
                if (B[r][c] != nullptr && B[r][c]->GetClr() != turn)
                {
                    HB[r][c] = 'k';
                }
                else if (B[r][c] != nullptr && B[r][c]->GetClr() == turn)
                {
                    HB[r][c] = 'y';
                }
                else
                {
                    HB[r][c] = 'x';
                }
            }
        }
    }
}

void Board::PrintHighlight()
{
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            gotoRowCol(ri, ci * 2);

            if (HB[ri][ci] == 'x')
            {
                color(1);
                cout << 'x';   //
            }
            else if (HB[ri][ci] == 'k')
            {
                color(4);
                if (B[ri][ci])
                {
                    B[ri][ci]->draw();
                    cout << " ";
                }
                else
                {
                    cout << '-';
                }
            }
            else if (HB[ri][ci] == 'y')
            {
                color(7);
                if (B[ri][ci])
                {
                    B[ri][ci]->draw();
                    cout << " ";
                }
                else
                {
                    cout << '-';
                }
            }
            else
            {
                color(7);
                if (B[ri][ci])
                {
                    B[ri][ci]->draw();
                    cout << " ";
                }
                else
                {
                    cout << '-';
                }
            }
        }
    }
    color(7);
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
    t=changeTurn(t);
    KingFinding(t, kr, kc);
    t=changeTurn(t);
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
                        if (IsValidDestination(sr, sc, er, ec) &&
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
                        if (IsValidDestination(sr, sc, er, ec) &&
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
        Ps[i] = new Player(name, i == 0 ? WHITE : BLACK);
    }

    char symbol;
    for (int ri = 0; ri < 8; ri++) 
    {
        for (int ci = 0; ci < 8; ci++) 
        {
            rdr>>symbol;
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
        //WHITE
    case 'r':
        B[ri][ci] = new Rook(WHITE, pos, this);
        break;
    case 'h':
        B[ri][ci] = new Knight(WHITE, pos, this);
        break;
    case 'b':
        B[ri][ci] = new Bishop(WHITE, pos, this);
        break;
    case 'q':
        B[ri][ci] = new Queen(WHITE, pos, this);
        break;
    case 'k':
        B[ri][ci] = new King(WHITE, pos, this);
        break;
    case 'p':
        B[ri][ci] = new Pawn(WHITE, pos, this);
        break;

        //BLACK
    case 'R':
        B[ri][ci] = new Rook(BLACK, pos, this);
        break;
    case 'H':
        B[ri][ci] = new Knight(BLACK, pos, this);
        break;
    case 'B':
        B[ri][ci] = new Bishop(BLACK, pos, this);
        break;
    case 'Q':
        B[ri][ci] = new Queen(BLACK, pos, this);
        break;
    case 'K':
        B[ri][ci] = new King(BLACK, pos, this);
        break;
    case 'P':
        B[ri][ci] = new Pawn(BLACK, pos, this);
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
    if (turn == WHITE && (sym == 'q' || sym == 'r' || sym == 'b' || sym == 'h'))
    {
        return true;
    }
    else if (turn == BLACK && (sym == 'Q' || sym == 'R' || sym == 'B' || sym == 'H'))
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

    switch (sym) {
    case 'q':
        B[r][c] = new Queen(WHITE, pos, this);
        break;
    case 'r':
        B[r][c] = new Rook(WHITE, pos, this);
        break;
    case 'b':
        B[r][c] = new Bishop(WHITE, pos, this);
        break;
    case 'h':
        B[r][c] = new Knight(WHITE, pos, this);
        break;

    case 'Q':
        B[r][c] = new Queen(BLACK, pos, this);
        break;
    case 'R':
        B[r][c] = new Rook(BLACK, pos, this);
        break;
    case 'B':
        B[r][c] = new Bishop(BLACK, pos, this);
        break;
    case 'H':
        B[r][c] = new Knight(BLACK, pos, this);
        break;
    }
}

void Board::PawnPromotion() {
    char sym = ' ';
    if (turn == WHITE) {
        for (int ci = 0; ci < 8; ci++) 
        {
            if (B[0][ci] && B[0][ci]->GetSymbol() == 'p') 
            {
                gotoRowCol(2, 30);
                cout << "Promote Pawn to:\n";
                gotoRowCol(3, 30);
                cout << "q. Queen\n";
                gotoRowCol(4, 30);
                cout << "r. Rook\n";
                gotoRowCol(5, 30);
                cout << "b. Bishop\n";
                gotoRowCol(6, 30);
                cout << "h. Horse\n";
                gotoRowCol(7, 30);
                do 
                {
                    gotoRowCol(8, 30);
                    cin >> sym;
                } while (!IsValidSym(sym));
                Promote(0, ci, WHITE, sym);
                return;
            }
        }
    }
    else if (turn == BLACK) 
    {
        for (int ci = 0; ci < 8; ci++) 
        {
            if (B[7][ci] && B[7][ci]->GetSymbol() == 'P') 
            {
                gotoRowCol(2, 30);
                cout << "Promote Pawn to:\n";
                gotoRowCol(3, 30);
                cout << "Q. Queen\n";
                gotoRowCol(4, 30);
                cout << "R. Rook\n";
                gotoRowCol(5, 30);
                cout << "B. Bishop\n";
                gotoRowCol(6, 30);
                cout << "H. Horse\n";
                gotoRowCol(7, 30);
                do 
                {
                    gotoRowCol(8, 30);
                    cin >> sym;
                } while (!IsValidSym(sym));
                Promote(7, ci, BLACK, sym);
                return;
            }
        }
    }
}


void Board::Play()
{
    int RepSize = 0;
    int choice = 0;
    Position S, D;
    system("cls");
    DrawBoard();
    hideConsoleCursor();

    do {
        gotoRowCol(9, 0);
        cout << Ps[turn]->GetName() << "'s Turn\n";
        gotoRowCol(0, 0);
        do {
            do {
                do {
                    SelectPiece(S.Pr, S.Pc);
                } while (!IsValidSelection(S.Pr, S.Pc));
                ComputeHighlight(S.Pr, S.Pc);
                PrintHighlight();
                SelectPiece(D.Pr, D.Pc);
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

        system("cls");
        DrawBoard();
        if (IsCheckmate(turn))
        {
            gotoRowCol(10, 0);
            cout << "It is a CHECKMATE!!!";
            Sleep(2000);
            break;
        }
        else if (IsCheck(turn))
        {
            turn = changeTurn(turn);
            gotoRowCol(10, 0);
            cout << Ps[turn]->GetName() << " in Check";
            turn = changeTurn(turn);
        }
        else if (IsStalemate(turn))
        {
            gotoRowCol(10, 0);
            cout << "It is a Stalemate. Match Ended as DRAW...";
            Sleep(2000);
            break;
        }

        SaveGame("Chess_Load.txt");
        turn = changeTurn(turn);
    } while (true);
}