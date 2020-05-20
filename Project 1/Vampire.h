//
//  vampire.h
//  project 1
//
//  Created by Urvi Shah on 1/12/20.
//  Copyright © 2020 Urvi Shah. All rights reserved.
//

#ifndef vampire_h
#define vampire_h

class Arena;
#include "globals.h"
#include <iostream>
using namespace std;

class Vampire
{
public:
    // Constructor
    Vampire(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    void move();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
    int    m_idleTurnsRemaining;
};

#endif /* vampire_h */
