#include "BSAI24060_Queen.h"
#include"raylib.h"
Queen::Queen(COLOR _clr, Position _P, Board* _B)
    : Piece(_clr, _P, _B), Rook(_clr, _P, _B), Bishop(_clr, _P, _B)
{
    texture = LoadTexture(_clr == PWHITE ? "W_Queen.png" : "B_Queen.png");
}

void Queen::draw()
{
    DrawTexture(texture, P.Pc * 100 + 15, P.Pr * 100 + 4, WHITE);
}

bool Queen::IsLegal(Position E)
{
	return Bishop::IsLegal(E) || Rook::IsLegal(E);
}
char Queen::GetSymbol() const
{
	return (clr == PWHITE) ? 'q' : 'Q';
}

