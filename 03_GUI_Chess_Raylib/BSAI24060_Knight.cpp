#include "BSAI24060_Knight.h"
#include"raylib.h"
Knight::Knight(COLOR _clr, Position _P, Board* _B)
	: Piece(_clr, _P, _B)
{
	texture = LoadTexture(_clr == PWHITE ? "W_Knight.png" : "B_Knight.png");
}

void Knight::draw()
{
	DrawTexture(texture, P.Pc * 100 + 15, P.Pr * 100 + 4, WHITE);
}

bool Knight::IsLegal(Position E)
{
	int dr = E.Pr - P.Pr, dc = E.Pc - P.Pc;
	return((abs(dr) == 2 && abs(dc) == 1) || (abs(dc) == 2 && abs(dr) == 1));
}

char Knight::GetSymbol() const
{
	return (clr == PWHITE) ? 'h' : 'H';
}
