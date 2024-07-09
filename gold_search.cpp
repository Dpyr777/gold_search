#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <random>
#include "Hero.hpp"
#include "Monsters.hpp"
#include "create_dungeon.hpp"
#include "result.hpp"
#include "fight_with_goblin.hpp"
//#include <chrono>

std::random_device rd;
std::mt19937 rnd (rd());
int rows, cols;
char **map;
namespace {
    int numberMonsters {3};
    bool open_next_lvl {false};
    bool newDungeon{true};
    int lvlDungeon{};
}


enum color 
{
    green = 1,
    red,
    yellow,
    blue,
    magenta
};

void draw(Hero &hero)
{
    for (int y{}; y < rows; ++y)
    {
        for (int x{}; x < cols; ++x)
        {
            if (y == hero.y && x == hero.x)
                mvaddch(y, x, hero.view() | A_BOLD | COLOR_PAIR(green));
            else if (map[y][x] == 'G' || map[y][x] == 'g')
                mvaddch(y, x, map[y][x] | A_BOLD | COLOR_PAIR(magenta));
            else if (map[y][x] == '$')
                mvaddch(y, x, map[y][x] | A_BOLD | COLOR_PAIR(yellow));
            else if (map[y][x] == '>' && open_next_lvl)
                mvaddch(y, x, map[y][x] | A_BOLD | COLOR_PAIR(blue));
            else if (map[y][x] == 'B')
                mvaddch(y, x, map[y][x] | A_BOLD | COLOR_PAIR(red));
            else if (map[y][x] == '&')
                mvaddch(y, x, map[y][x] | A_BOLD | COLOR_PAIR(red));
            else 
            {
                mvaddch(y, x, map[y][x]);
            }
        }
    }
    std::string str_hp(hero.get_lifes(), '|');
    attron(COLOR_PAIR(green) | A_BOLD); 
    //mvprintw(rows + 1, 0, "Hero hp: %d", hero.get_lifes());
    mvprintw(rows + 1, 0, "Hero hp: %s", str_hp.c_str());
    attroff(COLOR_PAIR(green) | A_BOLD); 
    attron(COLOR_PAIR(yellow) | A_BOLD); 
    mvprintw(rows + 1, 30, "Gold: %d", hero.get_gold());
    attroff(COLOR_PAIR(yellow) | A_BOLD); 
    attron(A_BOLD); 
    mvprintw(rows, cols / 2 - 5, "LVL %d", lvlDungeon);
    attroff(A_BOLD); 
    refresh();
}

void battle(int y, int x, Hero& hero, Monsters*& ms)
{
    int powerGoblin {};
    for (int i{}; i < numberMonsters; ++i)
    {
      if (y == ms[i].y && x == ms[i].x)
      {
        fight_with_goblin(hero, ms[i]);
         if (ms[i].hp == 0)
         {
             if (i == numberMonsters - 1)
             {
                 map[y][x] = '&'; 
             }
             else 
             {
                 map[y][x] = '$'; 
             }
             mvprintw(rows + 1, 30, "             "); 
             for (int i{}; i < numberMonsters; ++i)
             {
                 if (ms[i].hp > 0)
                     return;
             }
             open_next_lvl = true;
         }
         else
         {
             mvprintw(rows + 1, 40, "GOBLIN HP: %d ", ms[i].hp); 
         }
         return;
      }
    }
}

void create_goblins(Monsters *&monsters)
{
    std::uniform_int_distribution<> coord_y(5, rows - 5);
    std::uniform_int_distribution<> coord_x (5, cols - 5);
    std::uniform_int_distribution<> hp (lvlDungeon,  2 + lvlDungeon);
    std::uniform_int_distribution<> luck (1, 4);
    int y;
    int x;
    for (int i{}; i < numberMonsters; ++i)
    {
        do
        {
            x = coord_x(rnd);
            y = coord_y(rnd);
        
        } while (map[y][x] != ' ');
        
        int hitPoints{};
        int event = luck(rnd);
        if (i == numberMonsters - 1) 
        {
            hitPoints += 2 * lvlDungeon;
            map[y][x] = 'B';
        }
        else if (event == 3)
        {
            hitPoints += lvlDungeon;
            map[y][x] = 'G';
        }
        else
        {
            map[y][x] = 'G';
        }
        Monsters temp(y, x, hp(rnd) + hitPoints);
        monsters[i] = temp;
    }
}

void movement (int &button, Hero &h, Monsters *&ms)
{
    button = getch();
    int temp_y = h.y;
    int temp_x = h.x;

    switch (button)
    {
        case KEY_UP:
            --temp_y;
            break;
        case KEY_DOWN:
            ++temp_y;
            break;
        case KEY_LEFT:
            --temp_x;
            break;
        case KEY_RIGHT:
            ++temp_x;
            break;
        case '>':
            if (map[h.y][h.x] == '>' && open_next_lvl)
            {
               newDungeon = true; 
            }
            break;
    }
    if (map[temp_y][temp_x] == ' ' || map[temp_y][temp_x] == '>')
    {
        h.y = temp_y;
        h.x = temp_x;
    }
    else if (map[temp_y][temp_x] == 'G' || map[temp_y][temp_x] == 'B')
    {
        battle(temp_y, temp_x, h, ms);
    }
    else if (map[temp_y][temp_x] == '$')
    {
        std::uniform_int_distribution<> gold(lvlDungeon, 2 * lvlDungeon);
        h.add_gold(gold(rnd));
        h.y = temp_y;
        h.x = temp_x;
        map[temp_y][temp_x] = ' ';
    }
    else if (map[temp_y][temp_x] == '&')
    {
        h.heal(lvlDungeon);
        h.y = temp_y;
        h.x = temp_x;
        map[temp_y][temp_x] = ' ';
    }
        
}



int main()
{
    initscr(); // <ncurses.h>
    noecho(); // don't display input
    curs_set(0);
    keypad(stdscr, true); // allow arrows

    start_color();
    if (has_colors() == false)
    {
        endwin();
        std::cout << "Ploblem with the color!" << std::endl;
        return 3;
    }
    use_default_colors();
    init_pair(1, COLOR_GREEN, -1); // color Hero '@'
    init_pair(2, COLOR_RED, -1); // color Goblin 'B' and '&'
    init_pair(3, COLOR_YELLOW, -1); // color gold '$'
    init_pair(4, COLOR_BLUE, -1); // color stairs '>'
    init_pair(5, COLOR_MAGENTA, -1); // color stairs 'G'

    getmaxyx(stdscr, rows, cols);
    rows -= 2;
    --cols;
    Monsters *monsters;

    try 
    {
        map = new char*[rows + 1]; 
        for (int i{}; i < rows; ++i)
        {
            map[i] = new char[cols + 1];
        }
        monsters = new Monsters[numberMonsters];
    }
    catch (std::bad_alloc & err)
    {
        endwin();
        std::cout << "Bad alloc " << err.what() <<  std::endl;
        return 1;
    }
    catch (...)
    {
        endwin();
        std::cout << "ALL ERRORS" << std::endl;
        return 2;
    }
    Hero hero;

    int button{};
    
    do
    {
        if (newDungeon)
        {
            ++lvlDungeon;
            open_next_lvl = false;
            create_dungeon();
            hero.new_position();
            create_goblins(monsters);
            newDungeon = false;
        }
        draw(hero);
        movement(button,hero, monsters);

    } while (!(hero.dead()) && button != 27); // 27 - ESC
                                                          
    result(hero);

    for (int i{}; i < rows; ++i)
    {
        delete [] map[i];
    }
    delete [] map;
    delete [] monsters;
    endwin();
    return 0;
}
