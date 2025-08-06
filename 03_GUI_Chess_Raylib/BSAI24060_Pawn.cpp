#include "BSAI24060_Pawn.h"
#include"BSAI24060_Board.h"
#include"raylib.h"
Pawn::Pawn(COLOR _clr, Position _P, Board* _B)
    : Piece(_clr, _P, _B)
{
    texture = LoadTexture(_clr == PWHITE ? "W_Pawn.png" : "B_Pawn.png");
}
void Pawn::draw()
{
    DrawTexture(texture, P.Pc * 100 + 15, P.Pr * 100 + 4, WHITE);
}

bool Pawn::IsLegal(Position E)
{
    int dr = E.Pr - P.Pr;
    int dc = abs(E.Pc - P.Pc);

    if (clr == PWHITE)
    {
        // One square forward
        if (dc == 0 && dr == -1 && B->PieceAt(E) == nullptr)
        {
            return true;
        }
        // Two squares forward
        if (P.Pr == 6 && dc == 0 && dr == -2 && B->PieceAt(E) == nullptr && B->PieceAt({ P.Pr - 1, P.Pc }) == nullptr)
        {
            return true;
        }
        // Diagonal capture
        if (dc == 1 && dr == -1 && B->PieceAt(E) != nullptr && B->PieceAt(E)->GetClr() == PBLACK)
        {
            return true;
        }
    }
    else // BLACK
    {
        // One square forward
        if (dc == 0 && dr == 1 && B->PieceAt(E) == nullptr)
        {
            return true;
        }
        // Two squares forward
        if (P.Pr == 1 && dc == 0 && dr == 2 && B->PieceAt(E) == nullptr && B->PieceAt({ P.Pr + 1, P.Pc }) == nullptr)
        {
            return true;
        }
        // Diagonal capture
        if (dc == 1 && dr == 1 && B->PieceAt(E) != nullptr && B->PieceAt(E)->GetClr() == PWHITE)
        {
            return true;
        }
    }

    return false;
}

char Pawn::GetSymbol() const
{
    return (clr == PWHITE) ? 'p' : 'P';
}
