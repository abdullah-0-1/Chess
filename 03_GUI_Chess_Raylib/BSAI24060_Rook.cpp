#include "BSAI24060_Rook.h"
#include"raylib.h"
Rook::Rook(COLOR _clr, Position _P, Board* _B)
	: Piece(_clr, _P, _B), MoveHappened(false)
{
	texture = LoadTexture(_clr == PWHITE ? "W_Rook.png" : "B_Rook.png");
}

void Rook::draw()
{
	DrawTexture(texture, P.Pc * 100 + 15, P.Pr * 100 + 4, WHITE);
}

bool Rook::IsLegal(Position E)
{
	return ((IsHM(P, E) && IsHPC(B, P, E)) || (IsVM(P, E) && IsVPC(B, P, E)));
}

char Rook::GetSymbol() const
{
	return (clr == PWHITE) ? 'r' : 'R';
}

bool Rook:: HasMoved() const
{
	return MoveHappened;
}
void Rook::SetHasMoved(bool moved)
{
	MoveHappened = moved;
}
