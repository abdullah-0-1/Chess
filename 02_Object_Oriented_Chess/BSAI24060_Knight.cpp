#include "BSAI24060_Knight.h"
Knight::Knight(COLOR _clr, Position _P, Board* _B)
	:Piece(_clr, _P, _B)
{

}
void Knight::draw()
{
	if (clr == WHITE)
	{
		cout << "h";
	}
	else
	{
		cout << "H";
	}
}

bool Knight::IsLegal(Position E)
{
	int dr = E.Pr - P.Pr, dc = E.Pc - P.Pc;
	return((abs(dr) == 2 && abs(dc) == 1) || (abs(dc) == 2 && abs(dr) == 1));
}

char Knight::GetSymbol() const
{
	return (clr == WHITE) ? 'h' : 'H';
}
