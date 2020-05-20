//
//  game.h
//  project 1
//
//  Created by Urvi Shah on 1/12/20.
//  Copyright © 2020 Urvi Shah. All rights reserved.
//

#ifndef GAME_H
#define GAME_H

class Arena;

#include <iostream>
using namespace std;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
    
    // Helper functions
    string takePlayerTurn();
};


#endif /* GAME_H */
