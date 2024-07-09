#include "Monsters.hpp"

Monsters::Monsters()
    :y(1), x(1), hp(1), type('g') {}
Monsters::Monsters(int y, int x, int hp)
    :y(y), x(x), hp(hp)
{
    type = hp > 2 ? 'G' : 'g';
}
