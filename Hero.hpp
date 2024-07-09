#pragma once
#ifndef HERO_HPP
#define HERO_HPP
#include <random>
class Hero    {
    private:
        bool death; 
        int lifes;
        int gold;
        char image;
    public:
        int x;
        int y;
        Hero();

        int get_lifes();
        char view();
        int get_gold();
        bool dead();
        void add_gold(int numberGold);
        void wound();
        void new_position();
        void heal(int hp);
};
#endif
