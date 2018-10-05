#include "History.h"
#include "City.h"
#include <iostream>
using namespace std;
History :: History(int nRows, int nCol)
: m_rows(nRows),m_cols(nCol)
{
    for(int i=0;i<m_rows;i++)
    {
        for(int j=0;j<m_cols;j++)
        {
            grid[i][j]='.';
        }
    }
}

bool History :: record(int r, int c)
{
    if(r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols)
    {
        switch(grid[r-1][c-1])
        {
            case '.':
                grid[r-1][c-1]='A';
                break;
            case 'Z':
                break;
            default:
                grid[r-1][c-1]++;
                break;
        }
        return true; //to be changed
    }
    return false;
}

void History :: display() const
{
    clearScreen();
    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
}
