#include "BSAI24060_Piece.h"
#include"BSAI24060_Board.h"

Piece::Piece(COLOR _clr, Position _P, Board* _B)
	:clr{ _clr }, P{ _P }, B{ _B }
{

}
void Piece::SetPosition(Position _P)
{
	P = _P;
}

COLOR Piece::GetClr()
{
	return clr;
}
Position Piece::GetPos()
{
	return P;
}
bool Piece::IsHM(Position S, Position E)
{
	return (S.Pr == E.Pr);
}
bool Piece::IsVM(Position S, Position E)
{
	return (S.Pc == E.Pc);
}
bool Piece::IsDM(Position S, Position E)
{
	int dr = E.Pr - S.Pr, dc = E.Pc - S.Pc;
	return (abs(dr) == abs(dc));
}

bool Piece::IsHPC(Board* B, Position S, Position E)
{
    if (E.Pc > S.Pc)
    {
        for (int c = S.Pc + 1; c < E.Pc; c++)
        {
            if (B->PieceAt({S.Pr,c})!=nullptr)
            {
                return false;
            }
        }
    }

    else
    {
        for (int c = E.Pc + 1; c < S.Pc; c++)
        {
            if (B->PieceAt({ S.Pr,c }) != nullptr)
            {
                return false;
            }
        }
    }

    return true;
}
bool Piece::IsVPC(Board* B, Position S, Position E)
{
    if (E.Pr > S.Pr)
    {
        for (int r = S.Pr + 1; r < E.Pr; r++)
        {
            if (B->PieceAt({ r,S.Pc }) != nullptr)
            {
                return false;
            }
        }
    }

    else
    {
        for (int r = E.Pr + 1; r < S.Pr; r++)
        {
            if (B->PieceAt({ r,S.Pc }) != nullptr)
            {
                return false;
            }
        }
    }

    return true;
}
bool Piece::IsDPC(Board* B, Position S, Position E)
{
    int r, c;

    if (E.Pr > S.Pr && E.Pc > S.Pc)
    {
        r = S.Pr + 1;
        c = S.Pc + 1;
        for (; r < E.Pr && c < E.Pc; r++, c++)
        {
            if (B->PieceAt({ r,c }) != nullptr)
            {
                return false;
            }
        }
    }
    else if (E.Pr > S.Pr && E.Pc < S.Pc)
    {
        r = S.Pr + 1;
        c = S.Pc - 1;
        for (; r < E.Pr && c > E.Pc; r++, c--)
        {
            if (B->PieceAt({ r,c }) != nullptr)
            {
                return false;
            }
        }
    }
    else if (E.Pr<S.Pr && E.Pc>S.Pc)
    {
        r = S.Pr - 1;
        c = S.Pc + 1;
        for (; r > E.Pr && c < E.Pc; r--, c++)
        {
            if (B->PieceAt({ r,c }) != nullptr)
            {
                return false;
            }
        }
    }
    else if (E.Pr < S.Pr&& E.Pc < S.Pc)
    {
        r = S.Pr - 1;
        c = S.Pc - 1;
        for (; r > E.Pr && c > E.Pc; r--, c--)
        {
            if (B->PieceAt({ r,c }) != nullptr)
            {
                return false;
            }
        }
    }
    return true;
}

bool Piece:: AmIKing()
{
    return false;
}