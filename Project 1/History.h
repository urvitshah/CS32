//
//  History.h
//  project 1
//
//  Created by Urvi Shah on 1/12/20.
//  Copyright © 2020 Urvi Shah. All rights reserved.
//

#ifndef HISTORY_H
#define HISTORY_H

class Arena;
#include "globals.h"

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;

private:
    int m_row;
    int m_col;
    int m_arena[MAXROWS][MAXCOLS];
};

#endif /* HISTORY_H */
