#pragma once
#include<string>
#include"BSAI24060_Header.h"
using namespace std;
class Player
{
    string Name;
    COLOR clr;
public:
    Player(std::string n, COLOR _clr);
    string GetName() const;
    COLOR GetColor() const;
};

