#pragma once
#ifndef MONSTERS_HPP
#define MONSTERS_HPP

struct Monsters
{
    Monsters();
    Monsters(int y, int x, int hp);
    int y;
    int x;
    int hp;
    char type;
};

#endif




