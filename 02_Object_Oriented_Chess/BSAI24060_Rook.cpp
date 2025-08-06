#include "BSAI24060_Rook.h"
Rook::Rook(COLOR _clr, Position _P, Board* _B)
	:Piece(_clr, _P, _B),MoveHappened(false)
{

}
void Rook::draw()
{
	if (clr == WHITE)
	{
		cout << "r";
	}
	else
	{
		cout << "R";
	}
}

bool Rook::IsLegal(Position E)
{
	return ((IsHM(P, E) && IsHPC(B, P, E)) || (IsVM(P, E) && IsVPC(B, P, E)));
}

char Rook::GetSymbol() const
{
	return (clr == WHITE) ? 'r' : 'R';
}

bool Rook:: HasMoved() const
{
	return MoveHappened;
}
void Rook::SetHasMoved(bool moved)
{
	MoveHappened = moved;
}