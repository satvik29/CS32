#ifndef History_h
#define History_h

#include "globals.h"

class Arena;

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
private:
    char mGrid[MAXROWS][MAXCOLS];
    int mRows;
    int mCols;
};

#endif
