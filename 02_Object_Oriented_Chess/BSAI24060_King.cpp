#include "BSAI24060_King.h"
King::King(COLOR _clr, Position _P, Board* _B)
	:Piece(_clr, _P, _B), MoveHappened(false)
{

}
void King::draw()
{
	if (clr == WHITE)
	{
		cout << "k";
	}
	else
	{
		cout << "K";
	}
}
bool King::IsLegal(Position E)
{
	int dr = E.Pr - P.Pr, dc = E.Pc - P.Pc;
	return(((IsDM(P, E) && IsDPC(B, P, E)) || ((IsHM(P, E) && IsHPC(B, P, E)) || (IsVM(P, E) && IsVPC(B, P, E))))
		   && (abs(dr) <= 1 && abs(dc) <= 1));
}

bool King::AmIKing()
{
	return true;
}
char King::GetSymbol() const
{
	return (clr == WHITE) ? 'k' : 'K';
}

bool King::HasMoved() const
{
	return MoveHappened;
}
void King::SetHasMoved(bool moved)
{
	MoveHappened = moved;
}