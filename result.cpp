#include <ncurses.h>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <unistd.h>
#include "Hero.hpp"

void result(Hero &hero)
{
    initscr();
    clear();
    echo();
    curs_set(1);
    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, -1); // color Hero '@'
    init_pair(2, COLOR_RED, -1); // color Goblin 'G'
    init_pair(3, COLOR_YELLOW, -1); // color gold '$'
    init_pair(4, COLOR_BLUE, -1); // color stairs '>'
                                  
    std::ifstream file1;
    std::ofstream file2;
    file1.open("result.data",  std::ios_base::in);

    std::string str{};
    std::string name{};
    std::string my_score{};
    unsigned num_score{};
    std::multimap <unsigned, std::string> record_all;
    
    if (file1.is_open())
    {
        unsigned i = 9;
        attron(COLOR_PAIR(1) | A_BOLD);
        move (7, 25);
        printw ("THE BEST TOP 10\n");
        move (8, 20);
        attroff(COLOR_PAIR(1));
        attron(COLOR_PAIR(4) | A_BOLD);
        printw ("Name\t       Gold\n");
        attroff(COLOR_PAIR(4));
        unsigned num_score{};
        while (std::getline(file1, str))
        {
            std::istringstream record(str);
            record >> name >> my_score;
            num_score = stoi(my_score);
            record_all.emplace(num_score, name);
            move (i++, 20);
            printw("%-10s\t\t%d\n", name.c_str(), num_score); 
        }

        file1.close();
        refresh();
        
        if (num_score < hero.get_gold() || record_all.size() < 10)
        {
            mvprintw(21, 20,"What is your name??? (10 symbols)");
            mvprintw(22, 20,"Your gold: %d\n", hero.get_gold());
            refresh();
            file2.open("result.data",  std::ios_base::out);
            char my_name[11];
            mvscanw(23, 20, "%10s", my_name);
            record_all.emplace(hero.get_gold(), my_name);

            curs_set(0);

            std::multimap <unsigned, std::string>::const_reverse_iterator it_beg = record_all.crbegin(); 
            std::multimap <unsigned, std::string>::const_reverse_iterator it_end = record_all.crend(); 
            i = 10;
            unsigned count_top {};
            for (;it_beg != it_end; ++it_beg)
            {
                if (count_top == 10)
                {
                    break;
                }
                ++count_top;
                move (i++, 20);
                printw("%-10s\t\t%d\n", it_beg->second.c_str(), it_beg->first);
                file2 << it_beg->second << "\t" << it_beg->first << "\n";

            }
            refresh();
            //sleep(6);
            file2.close();
            getch();
        }
        else
        {
            mvprintw(20, 25,"Your gold: %d\n", hero.get_gold());
            refresh();
            //sleep(5);
            
            getch();
        }
       
    }
    endwin();
}
