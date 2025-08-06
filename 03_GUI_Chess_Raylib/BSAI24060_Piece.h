
#pragma once
#include "BSAI24060_Header.h"
#include <raylib.h>
class Board;
class Piece
{
protected:
    COLOR clr;
    Position P;
    Board* B;
    Texture2D texture;
    static bool IsHM(Position S, Position E);
    static bool IsVM(Position S, Position E);
    static bool IsDM(Position S, Position E);

    bool IsHPC(Board* B, Position S, Position E);
    bool IsVPC(Board* B, Position S, Position E);
    bool IsDPC(Board* B, Position S, Position E);
public:
    Piece(COLOR _clr, Position _P, Board* _B);
    void SetPosition(Position _P);
    virtual void draw() = 0;
    virtual bool IsLegal(Position E) = 0;
    virtual char GetSymbol() const = 0;
    virtual bool AmIKing();
    COLOR GetClr();
    Position GetPos();
    void SetTexture(Texture2D tex);
    Texture2D GetTexture() const;

    virtual ~Piece() = default;
};