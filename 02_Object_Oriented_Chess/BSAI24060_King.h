#pragma once
#include"BSAI24060_Piece.h"
class King : public Piece
{
	bool MoveHappened;
public:
	King(COLOR _clr, Position _P, Board* _B);
	void draw() override;
	bool IsLegal(Position E)override;
	char GetSymbol() const override;
	bool AmIKing()override;
	bool HasMoved() const;
	void SetHasMoved(bool moved);
};