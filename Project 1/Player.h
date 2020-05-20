//
//  player.h
//  project 1
//
//  Created by Urvi Shah on 1/12/20.
//  Copyright © 2020 Urvi Shah. All rights reserved.
//

#ifndef PLAYER_H
#define PLAYER_H

class Arena;
#include <iostream>
using namespace std;

class Player
{
public:
    // Constructor
    Player(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    string dropPoisonVial();
    string move(int dir);
    void   setDead();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

#endif /* PLAYER_H */
