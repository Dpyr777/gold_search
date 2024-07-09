#include <random>
#include "Hero.hpp"

extern std::mt19937 rnd;
extern char **map;
extern int rows, cols;


Hero::Hero() : death(false), lifes(7), gold(0), image('@'), x(1), y(1) {}

int Hero::get_lifes() { return lifes;}

char Hero::view() { return image;}

int Hero::get_gold() { return gold;}

bool Hero::dead() { return death;}

void Hero::add_gold(int numberGold)
{
    gold += numberGold;
}
void Hero::wound()
{
    if (lifes == 1)
    {
        death = true;
    }
    --lifes;
    return;
} 
void Hero::new_position()
{
    std::uniform_int_distribution<> dist1 (5, rows - 5);
    std::uniform_int_distribution<> dist2 (5, cols - 5);
    do
    {
        x = dist2(rnd);
        y = dist1(rnd);
    } while (map[y][x] != ' ');
}

void Hero::heal(int hp)
{
    lifes += hp;
}


