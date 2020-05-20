//
//  History.cpp
//  project 1
//
//  Created by Urvi Shah on 1/12/20.
//  Copyright © 2020 Urvi Shah. All rights reserved.
//

#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
    m_row = nRows;
    m_col = nCols;
    
    for (int r = 0; r < m_row; r++)
    {
        for (int c = 0; c < m_col; c++)
        {
            m_arena[r][c] = 0;
        }
    }
}

bool History::record(int r, int c)
{
    if (r > m_row || r < 1 || c > m_col || c < 1)
        return false;
    m_arena[r-1][c-1]++;
    return true;
}

void History::display() const
{
    clearScreen();
    for (int r = 0; r < m_row; r++)
    {
        for (int c = 0; c < m_col; c++)
        {
            if (m_arena[r][c] == 0)
                cout << ".";
            else if (m_arena[r][c] >= 26)
                cout << 'Z';
            else
                cout << static_cast<char>(m_arena[r][c]+64);
        }
        cout << endl;
    }
    cout << endl;
}
