#pragma once
#include"BSAI24060_Pawn.h"
#include"BSAI24060_Knight.h"
#include"BSAI24060_King.h"
#include"BSAI24060_Queen.h"
#include"BSAI24060_Player.h"
class Board
{
	Player* Ps[2];
	Piece* B[8][8]{};
	COLOR turn;
	char HB[8][8]{};

	void CreatePieceFromSymbol(int ri, int ci, char symbol);
public:
	Board();
	Board(const string& filename);
	COLOR changeTurn(COLOR t);
	void SelectPiece(int& Pr, int& Pc);
	void init();
	void DrawBoard();
	void Play();
	bool IsValidSelection(int r, int c);
	bool IsValidDestination(int sr, int sc, int er, int ec);
	void MovePiece(int sr, int sc, int er, int ec);
	Piece* PieceAt(Position P)const;
	void ClearHighlight();
	void ComputeHighlight(int sr, int sc);
	void PrintHighlight();
	void KingFinding(COLOR t, int& kr, int& kc);
	bool IsCheck(COLOR t);
	bool IsSelfCheck(COLOR turn, int sr, int sc, int er, int ec);
	bool IsCheckmate(COLOR t);
	bool IsStalemate(COLOR t);
	void LoadGame(const string& filename);
	void SaveGame(const string& filename);
	bool IsCasteling(Position K,Position R);
	void Casteling(Position K, Position R);
	bool IsValidSym(char sym);
	void Promote(int r, int c, COLOR pColor, char sym);
	void PawnPromotion();
};
