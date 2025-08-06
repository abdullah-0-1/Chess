#include "BSAI24060_Player.h"
Player::Player(std::string n, COLOR _clr)
    : Name(n), clr(_clr)
{

}

string Player::GetName() const
{
    return Name;
}

COLOR Player::GetColor() const
{
    return clr;
}