#pragma once
#include"BSAI24060_Piece.h"
class Knight :public Piece
{
public:
	Knight(COLOR _clr, Position _P, Board* _B);
	void draw() override;
	bool IsLegal(Position E)override;
	char GetSymbol() const override;

};

