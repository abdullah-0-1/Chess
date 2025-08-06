#pragma once
#include"BSAI24060_Rook.h"
#include"BSAI24060_Bishop.h"
class Queen :public Rook, public Bishop
{
public:
	Queen(COLOR _clr, Position _P, Board* _B);
	void draw() override;
	bool IsLegal(Position E)override;
	char GetSymbol() const override;
};