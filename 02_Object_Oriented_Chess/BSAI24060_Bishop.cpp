#include "BSAI24060_Bishop.h"
Bishop::Bishop(COLOR _clr, Position _P, Board* _B)
	:Piece(_clr, _P, _B)
{

}
void Bishop::draw()
{
	if (clr == WHITE)
	{
		cout << "b";
	}
	else
	{
		cout << "B";
	}
}

bool Bishop::IsLegal(Position E)
{
	return((IsDM(P,E) && IsDPC(B,P,E)));
}

char Bishop::GetSymbol() const
{
	return (clr == WHITE) ? 'b' : 'B';
}