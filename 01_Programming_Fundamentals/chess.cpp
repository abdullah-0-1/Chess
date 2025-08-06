#include<iostream>
#include<windows.h>
#include<conio.h>
#include<math.h>
#include<string>
#include<time.h>
#include <iomanip>
#include <fstream>
using namespace std;

void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT |
        ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState ==

            FROM_LEFT_1ST_BUTTON_PRESSED)

        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}

void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}

void color(int k)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, k);
}

void hideConsoleCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
// ***********************************************
enum COLOR
{
    WHITE, BLACK
};

struct Pieces
{
    char sym;
    COLOR CLR;
    bool(*fptr)(Pieces* Board[][8], int, int, int, int);
};

struct Position
{
    int sr, sc;
    int er, ec;
};

bool Rook(Pieces* Board[][8], int sr, int sc, int er, int ec);
bool Bishop(Pieces* Board[][8], int sr, int sc, int er, int ec);
bool Horse(Pieces* Board[][8], int sr, int sc, int er, int ec);
bool Queen(Pieces* Board[][8], int sr, int sc, int er, int ec);
bool King(Pieces* Board[][8], int sr, int sc, int er, int ec);
bool Pawn(Pieces* Board[][8], int sr, int sc, int er, int ec);

void init(Pieces* Board[][8], int& turn, string Names[2])
{
    //BLACK
    Board[0][0] = new Pieces{ 'R',BLACK,Rook }, Board[0][7] = new Pieces{ 'R',BLACK,Rook };
    Board[0][1] = new Pieces{ 'H',BLACK,Horse }, Board[0][6] = new Pieces{ 'H',BLACK,Horse };
    Board[0][2] = new Pieces{ 'B',BLACK,Bishop }, Board[0][5] = new Pieces{ 'B',BLACK,Bishop };
    Board[0][3] = new Pieces{ 'Q',BLACK,Queen }, Board[0][4] = new Pieces{ 'K',BLACK,King };
    //WHITE
    Board[7][0] = new Pieces{ 'r',WHITE,Rook }, Board[7][7] = new Pieces{ 'r',WHITE,Rook };
    Board[7][1] = new Pieces{ 'h',WHITE,Horse }, Board[7][6] = new Pieces{ 'h',WHITE,Horse };
    Board[7][2] = new Pieces{ 'b',WHITE,Bishop }, Board[7][5] = new Pieces{ 'b',WHITE,Bishop };
    Board[7][3] = new Pieces{ 'q',WHITE,Queen }, Board[7][4] = new Pieces{ 'k',WHITE,King };

    for (int ri = 1; ri < 7; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (ri == 1)
            {
                Board[ri][ci] = new Pieces{ 'P',BLACK,Pawn };
            }
            else if (ri == 6)
            {
                Board[ri][ci] = new Pieces{ 'p',WHITE,Pawn };
            }
            else
            {
                Board[ri][ci] = new Pieces{ '-' };
            }
        }
    }
    turn = WHITE;
    cout << "Enter Name of first player (White):";
    cin >> Names[0];
    cout << "Enter Name of Second player (Black):";
    cin >> Names[1];
}

void WhichPiece(Pieces*& pos, char Asym)
{
    switch (Asym)
    {
    case 'R':
    {
        pos = new Pieces{ 'R',BLACK,Rook };
    }
    break;

    case 'H':
    {
        pos = new Pieces{ 'H',BLACK,Horse };
    }
    break;

    case 'B':
    {
        pos = new Pieces{ 'B',BLACK,Bishop };
    }
    break;

    case 'Q':
    {
        pos = new Pieces{ 'Q',BLACK,Queen };
    }
    break;

    case 'K':
    {
        pos = new Pieces{ 'K',BLACK,King };
    }
    break;

    case 'P':
    {
        pos = new Pieces{ 'P',BLACK,Pawn };;
    }
    break;

    case 'p':
    {
        pos = new Pieces{ 'p',WHITE,Pawn };
    }
    break;

    case 'r':
    {
        pos = new Pieces{ 'r',WHITE,Rook };
    }
    break;

    case 'h':
    {
        pos = new Pieces{ 'h',WHITE,Horse };;
    }
    break;

    case 'b':
    {
        pos = new Pieces{ 'b',WHITE,Bishop };
    }
    break;

    case 'q':
    {
        pos = new Pieces{ 'q',WHITE,Queen };
    }
    break;

    case 'k':
    {
        pos = new Pieces{ 'k',WHITE,King };
    }
    break;

    default:
    {
        pos = new Pieces{ '-' };
    }
    }
}

void init_2(Pieces* Board[][8], int& turn, string Names[2])
{
    char v;
    ifstream rdr("Chess_Load.txt");
    rdr >> Names[0];
    rdr >> Names[1];
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            rdr >> v;
            WhichPiece(Board[ri][ci], v);
        }
    }

    rdr >> turn;


}

void TurnDisplay(string Aname)
{
    gotoRowCol(8, 0);
    cout << Aname << " Take your turn...";
}

void PrintBoard(Pieces* Board[][8])
{
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            cout << setw(2) << Board[ri][ci]->sym;
        }
        cout << endl;
    }
}

void SelectPiece(int& sr, int& sc)
{
    getRowColbyLeftClick(sr, sc);
    sc = sc / 2;
}

bool IsValidSelect(Pieces* Board[][8], int turn, int sr, int sc)
{
    if (sr < 0 || sr>7 || sc < 0 || sc>7)
    {
        return false;
    }
    if (turn == BLACK && (Board[sr][sc]->sym >= 'A' && Board[sr][sc]->sym <= 'Z'))
    {
        return true;
    }
    else if (turn == WHITE && (Board[sr][sc]->sym >= 'a' && Board[sr][sc]->sym <= 'z'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SelectPieceDes(int& er, int& ec)
{
    getRowColbyLeftClick(er, ec);
    ec = ec / 2;
}

bool IsValidSelectDes(Pieces* Board[][8], int turn, int er, int ec)
{
    if (er < 0 || er>7 || ec < 0 || ec>7)
    {
        return false;
    }
    if (turn == BLACK && (Board[er][ec]->sym == '-' || !(Board[er][ec]->sym >= 'A' && Board[er][ec]->sym <= 'Z')))
    {
        return true;
    }
    else if (turn == WHITE && (Board[er][ec]->sym == '-' || !(Board[er][ec]->sym >= 'a' && Board[er][ec]->sym <= 'z')))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MoveOnBoard(Pieces* Board[][8], int sr, int sc, int er, int ec)
{
    Board[er][ec]->sym = Board[sr][sc]->sym;
    Board[sr][sc]->sym = '-';
    Board[er][ec]->CLR = Board[sr][sc]->CLR;
}

void TurnChange(int& turn)
{
    turn = (turn + 1) % 2;
}

bool IsHM(int sr, int sc, int er, int ec)
{
    return (sr == er);
}

bool IsVM(int sr, int sc, int er, int ec)
{
    return (sc == ec);
}

bool IsDM(int sr, int sc, int er, int ec)
{
    int dr = er - sr, dc = ec - sc;
    return (abs(dr) == abs(dc));
}

bool IsHPC(Pieces* Board[][8], int sr, int sc, int er, int ec)
{
    if (ec > sc)
    {
        for (int c = sc + 1; c < ec; c++)
        {
            if (Board[sr][c]->sym != '-')
            {
                return false;
            }
        }
    }

    else
    {
        for (int c = ec + 1; c < sc; c++)
        {
            if (Board[sr][c]->sym != '-')
            {
                return false;
            }
        }
    }

    return true;
}

bool IsVPC(Pieces* Board[][8], int sr, int sc, int er, int ec)
{
    if (er > sr)
    {
        for (int r = sr + 1; r < er; r++)
        {
            if (Board[r][sc]->sym != '-')
            {
                return false;
            }
        }
    }

    else
    {
        for (int r = er + 1; r < sr; r++)
        {
            if (Board[r][sc]->sym != '-')
            {
                return false;
            }
        }
    }

    return true;
}

bool IsDPC(Pieces* Board[][8], int sr, int sc, int er, int ec)
{
    int r, c;

    if (er > sr && ec > sc)
    {
        r = sr + 1;
        c = sc + 1;
        for (; r < er && c < ec; r++, c++)
        {
            if (Board[r][c]->sym != '-')
            {
                return false;
            }
        }
    }
    else if (er > sr && ec < sc)
    {
        r = sr + 1;
        c = sc - 1;
        for (; r < er && c > ec; r++, c--)
        {
            if (Board[r][c]->sym != '-')
            {
                return false;
            }
        }
    }
    else if (er<sr && ec>sc)
    {
        r = sr - 1;
        c = sc + 1;
        for (; r > er && c < ec; r--, c++)
        {
            if (Board[r][c]->sym != '-')
            {
                return false;
            }
        }
    }
    else if (er < sr&& ec < sc)
    {
        r = sr - 1;
        c = sc - 1;
        for (; r > er && c > ec; r--, c--)
        {
            if (Board[r][c]->sym != '-')
            {
                return false;
            }
        }
    }
    return true;
}

bool Rook(Pieces* Board[][8], int sr, int sc, int er, int ec)
{
    return((IsHM(sr, sc, er, ec) && IsHPC(Board, sr, sc, er, ec)) || (IsVM(sr, sc, er, ec) && IsVPC(Board, sr, sc, er, ec)));
}

bool Bishop(Pieces* Board[][8], int sr, int sc, int er, int ec)
{
    return((IsDM(sr, sc, er, ec) && IsDPC(Board, sr, sc, er, ec)));
}

bool Queen(Pieces* Board[][8], int sr, int sc, int er, int ec)
{
    return(Rook(Board, sr, sc, er, ec) || Bishop(Board, sr, sc, er, ec));
}

bool King(Pieces* Board[][8], int sr, int sc, int er, int ec)
{
    int dr = er - sr, dc = ec - sc;
    return(Queen(Board, sr, sc, er, ec) && (abs(dr) <= 1 && abs(dc) <= 1));
}

bool Horse(Pieces* Board[][8], int sr, int sc, int er, int ec)
{
    int dr = er - sr, dc = ec - sc;
    return((abs(dr) == 2 && abs(dc) == 1) || (abs(dc) == 2 && abs(dr) == 1));
}

bool Pawn(Pieces* Board[][8], int sr, int sc, int er, int ec)
{
    if (Board[sr][sc]->CLR == WHITE)
    {
        if (ec == sc && er == sr - 1 && Board[er][ec]->sym == '-')
        {
            return true;
        }

        if (sr == 6 && ec == sc && er == sr - 2 && Board[er][ec]->sym == '-' && Board[sr - 1][sc]->sym == '-')
        {
            return true;
        }

        if (abs(ec - sc) == 1 && er == sr - 1 && Board[er][ec]->sym != '-' && Board[er][ec]->CLR == BLACK)
        {
            return true;
        }
    }

    else if (Board[sr][sc]->CLR == BLACK)
    {
        if (ec == sc && er == sr + 1 && Board[er][ec]->sym == '-')
        {
            return true;
        }

        if (sr == 1 && ec == sc && er == sr + 2 && Board[er][ec]->sym == '-' && Board[sr + 1][sc]->sym == '-')
        {
            return true;
        }

        if (abs(ec - sc) == 1 && er == sr + 1 && Board[er][ec]->sym != '-' && Board[er][ec]->CLR == WHITE)
        {
            return true;
        }
    }

    return false;
}

bool IsLegalMove(Pieces* Board[][8], int sr, int sc, int er, int ec)
{
    switch (Board[sr][sc]->sym)
    {
    case'r':
    case'R':
        return Rook(Board, sr, sc, er, ec);
        break;
    case'b':
    case'B':
        return Bishop(Board, sr, sc, er, ec);
        break;
    case'h':
    case'H':
        return Horse(Board, sr, sc, er, ec);
        break;
    case'q':
    case'Q':
        return Queen(Board, sr, sc, er, ec);
        break;
    case'k':
    case'K':
        return King(Board, sr, sc, er, ec);
        break;
    case'p':
    case'P':
        return Pawn(Board, sr, sc, er, ec);
        break;
    }

}

void CompHighlight(Pieces* Board[][8], char HB[][8], int sr, int sc, int turn)
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            int er = r;
            int ec = c;
            if (IsValidSelectDes(Board, turn, er, ec) && IsLegalMove(Board, sr, sc, er, ec))
            {
                if (Board[er][ec]->sym != '-' && Board[er][ec]->sym != turn)
                {
                    HB[r][c] = 'k';
                }
                else if (Board[er][ec]->sym != '-' && Board[er][ec]->sym == turn)
                {
                    HB[r][c] = 'y';
                }
                else
                {
                    HB[r][c] = 'x';
                }
            }
            else
            {
                HB[r][c] = '-';
            }
        }
    }
}

void PrintHighlight(Pieces* Board[][8], char HB[][8])
{
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (HB[ri][ci] == 'x')
            {
                color(1);
                gotoRowCol(ri, ci * 2);
                cout << setw(2) << 'x';

            }
            else if (HB[ri][ci] == 'k')
            {
                color(4);
                gotoRowCol(ri, ci * 2);
                cout << setw(2) << Board[ri][ci]->sym;

            }
            else if (HB[ri][ci] == 'y')
            {
                color(7);
                gotoRowCol(ri, ci * 2);
                cout << setw(2) << Board[ri][ci]->sym;

            }
            else
            {
                color(7);
                gotoRowCol(ri, ci * 2);
                cout << setw(2) << Board[ri][ci]->sym;
            }
        }
    }
    color(7);
}

void KingFinding(Pieces* Board[][8], int turn, int& kr, int& kc)
{
    char ksym;
    if (turn == BLACK)
    {
        ksym = 'K';
    }
    else
    {
        ksym = 'k';
    }

    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (Board[ri][ci]->sym == ksym)
            {
                kr = ri;
                kc = ci;
                return;
            }
        }
    }
}

bool IsCheck(Pieces* Board[][8], int turn)
{
    int kr, kc;
    TurnChange(turn);
    KingFinding(Board, turn, kr, kc);
    TurnChange(turn);
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (IsValidSelect(Board, turn, ri, ci) && IsLegalMove(Board, ri, ci, kr, kc))
            {
                return true;
            }
        }
    }
    return false;
}

bool IsSelfCheck(Pieces* Board[][8], int turn, int sr, int sc, int er, int ec)
{
    Pieces* TempB[8][8]{};
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            TempB[ri][ci] = new Pieces;
            TempB[ri][ci]->sym = Board[ri][ci]->sym;
            TempB[ri][ci]->fptr = Board[ri][ci]->fptr;
            TempB[ri][ci]->CLR = Board[ri][ci]->CLR;
        }
    }
    MoveOnBoard(TempB, sr, sc, er, ec);
    TurnChange(turn);
    bool res = IsCheck(TempB, turn);
    TurnChange(turn);

    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            delete[] TempB[ri][ci];
        }
    }
    return res;
}

bool IsCheckmate(Pieces* Board[][8], int turn)
{
    Pieces* TempB[8][8]{};
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            TempB[ri][ci] = new Pieces;
            TempB[ri][ci]->sym = Board[ri][ci]->sym;
            TempB[ri][ci]->fptr = Board[ri][ci]->fptr;
            TempB[ri][ci]->CLR = Board[ri][ci]->CLR;
        }
    }

    if (!IsCheck(TempB, turn))
    {
        for (int ri = 0; ri < 8; ri++)
        {
            for (int ci = 0; ci < 8; ci++)
            {
                delete TempB[ri][ci];
            }
        }
        return false;
    }

    int orTurn = turn;
    TurnChange(turn);

    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (IsValidSelect(TempB, turn, ri, ci))
            {
                for (int r = 0; r < 8; r++)
                {
                    for (int c = 0; c < 8; c++)
                    {
                        if (IsValidSelectDes(TempB, turn, r, c) &&
                            IsLegalMove(TempB, ri, ci, r, c) &&
                            !IsSelfCheck(TempB, turn, ri, ci, r, c))
                        {
                            for (int rd = 0; rd < 8; rd++)
                            {
                                for (int cd = 0; cd < 8; cd++)
                                {
                                    delete TempB[rd][cd];
                                }
                            }
                            turn = orTurn;
                            return false;
                        }
                    }
                }
            }
        }
    }
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            delete TempB[ri][ci];
        }
    }

    turn = orTurn;
    return true;
}

bool IsStalemate(Pieces* Board[][8], int turn)
{
    Pieces* TempB[8][8]{};
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            TempB[ri][ci] = new Pieces;
            TempB[ri][ci]->sym = Board[ri][ci]->sym;
            TempB[ri][ci]->fptr = Board[ri][ci]->fptr;
            TempB[ri][ci]->CLR = Board[ri][ci]->CLR;
        }
    }

    if (IsCheck(TempB, turn))
    {
        for (int ri = 0; ri < 8; ri++)
        {
            for (int ci = 0; ci < 8; ci++)
            {
                delete TempB[ri][ci];
            }
        }
        return false;
    }

    int orTurn = turn;
    TurnChange(turn);

    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (IsValidSelect(TempB, turn, ri, ci))
            {
                for (int r = 0; r < 8; r++)
                {
                    for (int c = 0; c < 8; c++)
                    {
                        if (IsValidSelectDes(TempB, turn, r, c) &&
                            IsLegalMove(TempB, ri, ci, r, c) &&
                            !IsSelfCheck(TempB, turn, ri, ci, r, c))
                        {
                            for (int rd = 0; rd < 8; rd++)
                            {
                                for (int cd = 0; cd < 8; cd++)
                                {
                                    delete TempB[rd][cd];
                                }
                            }
                            turn = orTurn;
                            return false;
                        }
                    }
                }
            }
        }
    }
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            delete TempB[ri][ci];
        }
    }

    turn = orTurn;
    return true;
}

void Promote(Pieces* Board[][8], int r, int c, int turn, char Asym)
{
    if (turn == WHITE)
    {
        switch (Asym)
        {
        case 'q': case'Q':
        {
            Board[r][c]->sym = 'q';
            Board[r][c]->fptr = Queen;
        }
        break;
        case 'r': case'R':
        {
            Board[r][c]->sym = 'r';
            Board[r][c]->fptr = Rook;
        }
        break;
        case 'b': case'B':
        {
            Board[r][c]->sym = 'b';
            Board[r][c]->fptr = Bishop;
        }
        break;
        case 'h': case'H':
        {
            Board[r][c]->sym = 'h';
            Board[r][c]->fptr = Horse;
        }
        break;
        default:
            return;
        }
    }

    if (turn == BLACK)
    {
        switch (Asym)
        {
        case 'q': case'Q':
        {
            Board[r][c]->sym = 'Q';
            Board[r][c]->fptr = Queen;
        }
        break;
        case 'r': case'R':
        {
            Board[r][c]->sym = 'R';
            Board[r][c]->fptr = Rook;
        }
        break;
        case 'b': case'B':
        {
            Board[r][c]->sym = 'B';
            Board[r][c]->fptr = Bishop;
        }
        break;
        case 'h': case'H':
        {
            Board[r][c]->sym = 'H';
            Board[r][c]->fptr = Horse;
        }
        break;
        default:
            return;
        }
    }
}

bool IsValidSym(char Asym)
{
    switch (Asym)
    {
    case 'q': case'Q':
        return true;
        break;
    case 'r': case'R':
        return true;
        break;
    case 'b': case'B':
        return true;
        break;
    case 'h': case'H':
        return true;
        break;
    default:
        return false;
    }
}

void PawnPromotion(Pieces* Board[][8], int turn)
{
    char sym = ' ';
    if (turn == WHITE)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (Board[0][ci]->sym == 'p')
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
                Promote(Board, 0, ci, turn, sym);
                return;
            }
        }
    }

    else if (turn == BLACK)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            if (Board[7][ci]->sym == 'P')
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
                Promote(Board, 0, ci, turn, sym);
                return;
            }
        }
    }
}

bool IsCastling(Pieces* Board[][8], int Kr, int Kc, int Rr, int Rc, int turn)
{
    if (turn == WHITE && Board[Kr][Kc]->sym != 'k' || turn == BLACK && Board[Kr][Kc]->sym != 'K')
    {
        return false;
    }

    if (turn == WHITE && Board[Rr][Rc]->sym != 'r' || turn == BLACK && Board[Rr][Rc]->sym != 'R')
    {
        return false;
    }

    if (Kr != Rr || !(IsHPC(Board, Kr, Kc, Rr, Rc)))
    {
        return false;
    }

    if (Kc < Rc)
    {
        for (int i = 0; i <= 2; i++)
        {
            if (IsSelfCheck(Board, turn, Kr, Kc, Kr, Kc + i))
            {
                return false;
            }
        }
    }
    else if (Kc > Rc)
    {
        for (int i = 0; i <= 2; i++)
        {
            if (IsSelfCheck(Board, turn, Kr, Kc, Kr, Kc - i))
            {
                return false;
            }
        }
    }

    return true;

}

void Castle(Pieces* Board[][8], int turn, int Kr, int Rc)
{
    int newKc = 4;
    int newRc = 4;
    if (Rc < 4)
    {
        newKc = 2;
        newRc = 3;
    }
    else if (Rc > 4)
    {
        newKc = 6;
        newRc = 5;
    }
    MoveOnBoard(Board, Kr, 4, Kr, newKc);
    MoveOnBoard(Board, Kr, Rc, Kr, newRc);

}

void ChessSave(Pieces* Board[][8], int turn, string Names[2])
{
    ofstream wtr("Chess_Load.txt");
    wtr << Names[0];
    wtr << endl;
    wtr << Names[1];
    wtr << endl;
    for (int ri = 0; ri < 8; ri++)
    {
        for (int ci = 0; ci < 8; ci++)
        {
            wtr << Board[ri][ci]->sym;
        }
        wtr << endl;
    }

    wtr << endl;
    wtr << turn;
}

void Rules()
{
    cout << "RULES for CHESS:\n"
        << "=> Use click to select the piece and destination.\n"
        << "=> After Every move there is a 2 second time if you want to undo or redo your move.\n"
        << "=> Press \"U\" for Undo and \"R\" for Redo.\n"
        << "=> At the end of the game you will be given an option for replay.\n"
        << "=> Your Game is Being Autosaved so you can close compiler anytime.\n";
}

void Choice(int& choice)
{
    cout << "Enter your choice\n"
        << "1) New Game\n"
        << "2) Load Game\n"
        << "0) Exit Game\n";
    cin >> choice;
}

void Choice2(int& choice)
{
    cout << "Enter your choice\n"
        << "1) Watch Replay\n"
        << "0) Exit Game\n";
    cin >> choice;
}

void SaveForReplay(char***& RepBoard, int& RepSize, Pieces* Board[][8])
{
    char*** NRB = new char** [RepSize + 1];
    for (int i = 0; i < RepSize; i++) {
        NRB[i] = RepBoard[i];
    }

    NRB[RepSize] = new char* [8];
    for (int ri = 0; ri < 8; ri++) {
        NRB[RepSize][ri] = new char[8];
        for (int ci = 0; ci < 8; ci++) {
            NRB[RepSize][ri][ci] = Board[ri][ci]->sym;
        }
    }
    delete[] RepBoard;
    RepSize++;
    RepBoard = NRB;
}

void ShowReplay(char*** RepBoard, int RepSize)
{
    for (int fi = 0; fi < RepSize; fi++)
    {
        for (int ri = 0; ri < 8; ri++)
        {
            for (int ci = 0; ci < 8; ci++)
            {
                cout << setw(2) << RepBoard[fi][ri][ci];
            }
            cout << endl;
        }
        Sleep(2000);
        system("cls");
    }
}

void DeleteMemory(Pieces* Board[][8], char*** RepBoard, int RepSize)
{
    for (int ri = 0; ri < 8; ri++) {
        for (int ci = 0; ci < 8; ci++) {
            if (Board[ri][ci] != nullptr) {
                delete Board[ri][ci];
                Board[ri][ci] = nullptr;
            }
        }
    }


    for (int fi = 0; fi < RepSize; fi++) {
        if (RepBoard[fi] != nullptr) {
            for (int ri = 0; ri < 8; ri++) {
                delete[] RepBoard[fi][ri];
            }
            delete[] RepBoard[fi];
        }
    }
    delete[] RepBoard;
}

// ***************************************MAIN**********************
int main()
{
    int RepSize = 0;
    int choice = 0;
    int choice2 = 0;
    Position Redo, Undo;
    string Names[2];
    int turn = 0;
    Pieces* Board[8][8]{};
    char*** RepBoard{};
    char HB[8][8]{};
    int sr, sc, er, ec;
    Rules();
    _getch();
    system("cls");
    Choice(choice);
    switch (choice)
    {
    case 1:
        init(Board, turn, Names);
        break;
    case 2:
        init_2(Board, turn, Names);
        break;
    case 0:
    {
        cout << "Game Ended";
        exit(1);
    }
    default:
    {
        cout << "Wrong Input...";
        exit(1);
    }
    }
    system("cls");
    PrintBoard(Board);
    SaveForReplay(RepBoard, RepSize, Board);
    hideConsoleCursor();
    do
    {
        bool UndoHappen = false;
        TurnDisplay(Names[turn]);

        do
        {
            do {
                do
                {
                    SelectPiece(sr, sc);
                } while (!IsValidSelect(Board, turn, sr, sc));
                CompHighlight(Board, HB, sr, sc, turn);
                PrintHighlight(Board, HB);
                SelectPieceDes(er, ec);
            } while (!IsValidSelectDes(Board, turn, er, ec));
        } while ((!IsLegalMove(Board, sr, sc, er, ec) && !IsCastling(Board, sr, sc, sr, er, turn)) || IsSelfCheck(Board, turn, sr, sc, er, ec));



        if (IsCastling(Board, sr, sc, sr, er, turn))
        {
            Castle(Board, turn, sr, er);
        }
        else
        {
            MoveOnBoard(Board, sr, sc, er, ec);
        }

        system("cls");
        PrintBoard(Board);
        if (IsCheckmate(Board, turn))
        {
            gotoRowCol(10, 0);
            cout << "It is a CHECKMATE!!!";
            SaveForReplay(RepBoard, RepSize, Board);
            Sleep(2000);
          //  DeleteMemory(Board, RepBoard, RepSize);
            break;
        }
        else if (IsCheck(Board, turn))
        {
            TurnChange(turn);
            gotoRowCol(10, 0);
            cout << Names[turn] << " in Check";
            TurnChange(turn);
        }
        else if (IsStalemate(Board, turn))
        {
            gotoRowCol(10, 0);
            cout << "It is a Stalemate. Match Ended as DRAW...";
            SaveForReplay(RepBoard, RepSize, Board);
            Sleep(2000);
          //  DeleteMemory(Board, RepBoard, RepSize);
            break;
        }
        PawnPromotion(Board, turn);

        int t = 0;
        SaveForReplay(RepBoard, RepSize, Board);

        Undo.sr = er, Undo.sc = ec, Undo.er = sr, Undo.ec = sc;
        do
        {
            t++;

            if (_kbhit())
            {
                char k = _getch();
                if (k == 'u' || k == 'U')
                {
                    MoveOnBoard(Board, Undo.sr, Undo.sc, Undo.er, Undo.ec);
                    system("cls");
                    PrintBoard(Board);
                    UndoHappen = true;
                    TurnChange(turn);
                    ChessSave(Board, turn, Names);
                    SaveForReplay(RepBoard, RepSize, Board);
                }
            }
            Redo.sr = Undo.er, Redo.sc = Undo.ec, Redo.er = Undo.sr, Redo.ec = Undo.sc;
            if (_kbhit())
            {
                char k = _getch();
                if ((k == 'r' || k == 'R') && UndoHappen == true)
                {
                    MoveOnBoard(Board, Redo.sr, Redo.sc, Redo.er, Redo.ec);
                    system("cls");
                    PrintBoard(Board);
                    TurnChange(turn);
                    SaveForReplay(RepBoard, RepSize, Board);
                    break;
                }
            }

        } while (t != 50000);

        TurnChange(turn);

        ChessSave(Board, turn, Names);

    } while (true);
    system("cls");
    Choice2(choice2);
    switch (choice2)
    {
    case 1:
    {
        {
            system("cls");
            ShowReplay(RepBoard, RepSize);
        }
    }
    break;

    case 0:
    {
        cout << "Thanks for Playing";
       // DeleteMemory(Board, RepBoard, RepSize);
        exit(1);
    }
    break;

    default:
        cout << "Wrong Input....";
      //  DeleteMemory(Board, RepBoard, RepSize);
        exit(1);
    }
   // DeleteMemory(Board, RepBoard, RepSize);

    return 0;
}
