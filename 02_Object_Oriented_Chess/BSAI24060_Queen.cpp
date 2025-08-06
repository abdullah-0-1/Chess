#include "BSAI24060_Queen.h"
Queen::Queen(COLOR _clr, Position _P, Board* _B)
	: Piece(_clr, _P, _B), Rook(_clr, _P, _B), Bishop(_clr, _P, _B)
{

};
void Queen::draw()
{
	if (clr == WHITE)
	{
		cout << "q";
	}
	else
	{
		cout << "Q";
	}
}

bool Queen::IsLegal(Position E)
{
	return Bishop::IsLegal(E) || Rook::IsLegal(E);
}
char Queen::GetSymbol() const
{
	return (clr == WHITE) ? 'q' : 'Q';
}