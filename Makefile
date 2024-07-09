CXX = g++
CXXFLAGS = -c

all: gold_search

gold_search: gold_search.o result.o create_dungeon.o Hero.o Monsters.o fight_with_goblin.o 
	$(CXX) $^ -o $@ -lncurses

gold_search.o: gold_search.cpp 
	$(CXX)  $(CXXFLAGS) $<

create_dungeon.o: create_dungeon.cpp
	$(CXX)  $(CXXFLAGS) $<

Hero.o: Hero.cpp
	$(CXX)  $(CXXFLAGS) $<

Monsters.o: Monsters.cpp
	$(CXX)  $(CXXFLAGS) $<

fight_with_goblin.o: fight_with_goblin.cpp
	$(CXX)  $(CXXFLAGS) $<

result.o: result.cpp
	$(CXX)  $(CXXFLAGS) $<

run: gold_search
	./$<

clear:
	rm -rf *.o



