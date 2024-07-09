#include <random>
#include <iostream>
#include "create_dungeon.hpp"
 
extern std::mt19937 rnd; 
extern int rows, cols;
extern char **map;

struct CenterRooms
{
    int x;
    int y;
    CenterRooms(): x(0), y(0) {}
    CenterRooms & operator = (CenterRooms &other)
    {
        this->x = other.x;
        this->y = other.y;
        return *this;
    }
    
    bool operator == (CenterRooms &other)
    {
        return (this->x == other.x && this->y == other.y); 
    }
};

void create_dungeon()
{
    for (int y{}; y < rows; ++y)
    {
        for (int x{}; x < cols; ++x)
       {
            if (y == 0 || y == (rows - 1) || x == 0 || x == (cols - 1))
            {
                map[y][x] = '%';
            }
            else
            {
                map[y][x] = '#';
            }
        }
    }
    // the starting room point;
    std::uniform_int_distribution<> dist1(5, rows - 5);
    int room_y;
    std::uniform_int_distribution<> dist2 (5, cols - 5);
    int room_x;

    // size room 10 - 20;
    std::uniform_int_distribution<> dist3(5, 10);
    int size_room_y;
    std::uniform_int_distribution<> dist4(10, 20);
    int size_room_x;

    int try_counter; // number of tries for prototype
    bool collision;
    int room_num = 10; // number of rooms
                       
    CenterRooms center_room_old;
    CenterRooms center_room_new;

    while (--room_num >= 0)
    {
        // prototiping the room until no collisons
        try_counter = 0;
        do 
        {
            collision = false;
            // the starting room point;
            room_y = dist1(rnd);
            room_x = dist2(rnd);
            // size room 5 - 20;
            size_room_y = dist3(rnd);
            size_room_x = dist4(rnd);

            for (int y{room_y - 2}; y <= (room_y + size_room_y + 2); ++y)
            {
                for (int x{room_x - 2}; x <= (room_x + size_room_x + 2); ++x)
                {
                    if (map[y][x] == '%' || map[y][x] == ' ')
                    {
                        collision = true;
                        y = (room_y + size_room_y + 2);
                        break;
                    }
                }
            }
            ++try_counter;
            if (try_counter > 100)
            {
                room_y = room_x = 3;
                size_room_y = size_room_x = 5;
                break;
            }
        } while (collision == true);

        for (int y{room_y}; y <= (room_y + size_room_y); ++y)
        {
            if (y == rows)
            {
                break;
            }
            for (int x{room_x}; x <= (room_x + size_room_x); ++x)
            {
                if (map[y][x] == '%') // Wall
                {
                    break;
                }
                else
                {
                    map[y][x] = ' ';
                }
            }
        }
        center_room_new.y = room_y + (size_room_y / 2);
        center_room_new.x = room_x + (size_room_x / 2);

        if (center_room_old.x == 0 && center_room_old.y == 0)
        {
            center_room_old = center_room_new;
        }

        for (int y{center_room_new.y}; y != center_room_old.y;)
        {
            map[y][center_room_new.x] = ' ';
            if (center_room_new.y < center_room_old.y)
                ++y;
            else 
                --y;
        } 
        for (int x{center_room_old.x}; x != center_room_new.x;)
        {
            map[center_room_old.y][x] = ' ';
            if (center_room_new.x > center_room_old.x)
                ++x;
            else 
                --x;
        } 
        map[center_room_old.y][center_room_new.x] = ' ';

        center_room_old = center_room_new;
    }
    std::uniform_int_distribution<> void_y (5, rows - 5);
    std::uniform_int_distribution<> void_x(5, cols - 5);
    int stairs_y{};
    int stairs_x{};
    do 
    {
        stairs_y = void_y(rnd);
        stairs_x = void_x(rnd);
    } while (map[stairs_y][stairs_x] != ' ');

    map[stairs_y][stairs_x] = '>';
}
