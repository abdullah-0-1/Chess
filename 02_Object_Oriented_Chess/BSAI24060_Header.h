#pragma once
#include<iostream>
#include<windows.h>
#include<conio.h>
#include<math.h>

using namespace std;


void getRowColbyLeftClick(int& rpos, int& cpos);

void gotoRowCol(int rpos, int cpos);

void hideConsoleCursor();

void color(int k);
enum COLOR
{
    WHITE, BLACK
};

struct Position
{
    int Pr, Pc;
};