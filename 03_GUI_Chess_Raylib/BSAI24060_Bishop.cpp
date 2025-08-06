#include "BSAI24060_Bishop.h"
#include"raylib.h"
Bishop::Bishop(COLOR _clr, Position _P, Board* _B)
    : Piece(_clr, _P, _B)
{
    texture = LoadTexture(_clr == PWHITE ? "W_Bishop.png" : "B_Bishop.png");
}

void Bishop::draw()
{
    DrawTexture(texture, P.Pc * 100 + 15, P.Pr * 100 + 4, WHITE);
}

bool Bishop::IsLegal(Position E)
{
	return((IsDM(P,E) && IsDPC(B,P,E)));
}

char Bishop::GetSymbol() const
{
	return (clr == PWHITE) ? 'b' : 'B';
}

