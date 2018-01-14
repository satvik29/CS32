#include <iostream>
#include "History.h"
#include "globals.h"

History::History(int nRows, int nCols) {
    mRows = nRows;
    mCols = nCols;
    for (int i = 1; i <= mRows; i++) {
        for (int j = 1; j <= mCols; j++) {
            mGrid[i][j] = '.';
        }
    }
}

bool History::record(int r, int c) {
    bool record = true;
    
    if (!(r >= 1  &&  r <= mRows  &&  c >= 1  &&  c <= mCols)) {
        record = false;
    } else {
        if (mGrid[r][c] <= 'Z' && mGrid[r][c] >= 'A') {
            mGrid[r][c] += 1;
        } else {
            mGrid[r][c] = 'A';
        }
    }
    
    return record;
}

void History::display() const {
    
    clearScreen();
    
    for (int i = 1; i <= mRows; i++) {
        for (int j = 1; j <= mCols; j++) {
            std::cout << mGrid[i][j];
        }
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
}
