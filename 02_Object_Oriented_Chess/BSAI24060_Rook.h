#pragma once
#include"BSAI24060_Piece.h"
class Rook :virtual public Piece
{
	bool MoveHappened;
public:
	Rook(COLOR _clr, Position _P, Board* _B);
	void draw() override;
	bool IsLegal(Position E)override;
	char GetSymbol() const override;
	bool HasMoved() const;
	void SetHasMoved(bool moved);
};