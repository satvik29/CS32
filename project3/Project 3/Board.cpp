#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <vector>

using namespace std;

// keeps track of the different ships on the board
class BoardShip {
public:
    int length = 0;
    int shipId;
    char shipSymbol;
};

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    
    bool allShipsDestroyed() const;
    
  private:
    char displayedBoard[MAXROWS][MAXCOLS];
    vector<BoardShip> m_Ships;
    const Game& m_game;
//    int m_shipAlive;
};

BoardImpl::BoardImpl(const Game& g):m_game(g) {
    for (int i = 0; i < m_game.rows(); i++) {
        for (int j = 0; j < m_game.cols(); j++) {
            displayedBoard[i][j] = '.';
        }
    }
}

void BoardImpl::clear() {
    for (char i = 0; i < m_game.rows(); i++) {
        for (char j = 0; j < m_game.cols(); j++) {
            displayedBoard[i][j] = '.';
        }
    }
}

void BoardImpl::block() {
      // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            if (randInt(2) == 0)
            {
                displayedBoard[r][c] = '#';
            }
}

void BoardImpl::unblock() {
    for (int r = 0; r < m_game.rows(); r++) {
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (displayedBoard[r][c] == '#')
                displayedBoard[r][c] = '.';
        }
    }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir) {
    // to check if the ID is valid
    if (shipId > m_game.nShips()) {
        return false;
    }
    
    // to check if the shipId has already been placed
    for (int i = 0; i < m_Ships.size(); i++) {
        if (m_Ships[i].shipId == shipId) {
            return false;
        }
    }
    
    if (dir == HORIZONTAL) {
        // checking if the ship is out of bounds
        if (m_game.shipLength(shipId) + topOrLeft.c - 1 > m_game.cols()) {
            return false;
        }
        
        // checking if there's a blockage or if another ship is placed there.
        for (int i = 0; i < m_game.shipLength(shipId); i++) {
            if (displayedBoard[topOrLeft.r][topOrLeft.c + i] != '.') {
                return false;
            }
        }
        
        // placing the ship
        for (int j = 0; j < m_game.shipLength(shipId); j++) {
            displayedBoard[topOrLeft.r][topOrLeft.c + j] = m_game.shipSymbol(shipId);
            BoardShip temp;
            temp.shipId = shipId;
            temp.length = m_game.shipLength(shipId);
            m_Ships.push_back(temp);
 //           m_shipAlive++;
        }
        
        return true;
    } else {
        // checking if the ship is out of bounds
        if (m_game.shipLength(shipId) + topOrLeft.r - 1 > m_game.rows()) {
            return false;
        }
    
        // checking if there's a blockage or if another ship is placed there.
        for (int i = 0; i < m_game.shipLength(shipId); i++) {
            if (displayedBoard[topOrLeft.r + i][topOrLeft.c] != '.') {
                return false;
            }
        }
    
        // placing the ship
        for (int j = 0; j < m_game.shipLength(shipId); j++) {
            displayedBoard[topOrLeft.r + j][topOrLeft.c] = m_game.shipSymbol(shipId);
            BoardShip temp;
            temp.shipId = shipId;
            temp.length = m_game.shipLength(shipId);
            temp.shipSymbol = m_game.shipSymbol(shipId);
            m_Ships.push_back(temp);
//            m_shipAlive++;
        }
    
        return true;
    }
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir) {
    if (shipId > m_game.nShips()) {
        return false;
    }
    
    if (dir == HORIZONTAL) {
        // checking if the ship is at the given location
        for (int i = 0; i < m_game.shipLength(shipId); i++) {
            if (displayedBoard[topOrLeft.r][topOrLeft.c + i] != m_game.shipSymbol(shipId)) {
                return false;
            }
        }
        
        // unplacing the ship
        for (int j = 0; j < m_game.shipLength(shipId); j++) {
            displayedBoard[topOrLeft.r][topOrLeft.c + j] = '.';
        }
        
        for (int k = 0; k < m_Ships.size(); k++) {
            if (m_Ships[k].shipId == shipId) {
                m_Ships.erase(m_Ships.begin() + k);
 //               m_shipAlive--;
            }
        }
        
        return true;
    } else {
        // checking if the ship is at the given location
        for (int i = 0; i < m_game.shipLength(shipId); i++) {
            if (displayedBoard[topOrLeft.r + i][topOrLeft.c] != m_game.shipSymbol(shipId)) {
                return false;
            }
        }
        
        // unplacing the ship
        for (int j = 0; j < m_game.shipLength(shipId); j++) {
            displayedBoard[topOrLeft.r + j][topOrLeft.c] = '.';
        }
        
        for (int k = 0; k < m_Ships.size(); k++) {
            if (m_Ships[k].shipId == shipId) {
                m_Ships.erase(m_Ships.begin() + k);
//                m_shipAlive--;
            }
        }
        
        return true;
    }
}

// displaying the board
void BoardImpl::display(bool shotsOnly) const {
    cout << "  ";
    
    for (int i = 0; i < m_game.cols(); i++) {
        cout << i;
    }
    
    cout << endl;
    
    for (int i = 0; i < m_game.rows(); i++) {
        cout << i << " ";
        for (int j = 0; j < m_game.cols(); j++) {
            if (shotsOnly == true && isalpha(displayedBoard[i][j])) { // to make sure that only the shots
                cout << '.';                                          // are shown
            } else {
                cout << displayedBoard[i][j];
            }
        }
        cout << endl;
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId) {
    // checking if the point given is on the board
    if (p.r >= m_game.rows() || p.c >= m_game.cols() || p.r < 0 || p.c < 0) {
        return false;
    }
    
    // if the point has already been hit return false
    if (displayedBoard[p.r][p.c] == 'o' || displayedBoard[p.r][p.c] == 'X') {
        shotHit = false;
        shipDestroyed = false;
        return false;
    }
    
    // incase the point has not been has and there is no ship on that point
    if (displayedBoard[p.r][p.c] == '.') {
        shotHit = false;
        shipDestroyed = false;
        displayedBoard[p.r][p.c] = 'o';
    } else if (isascii(displayedBoard[p.c][p.r]) || isprint(displayedBoard[p.c][p.r])) { // incase there is
                                                                                      //a ship on the point
        shotHit = true;
        for (int i = 0; i < m_Ships.size(); i++) {
            if (m_Ships[i].shipSymbol == displayedBoard[p.r][p.c]) {
                m_Ships[i].length--;
                if (m_Ships[i].length == 0) {
                    shipDestroyed = true;
//                    m_shipAlive--;
                }
                shipId = m_Ships[i].shipId;
            }
        }
        displayedBoard[p.r][p.c] = 'X';
    }

    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
//    int numberDestroyed = 0;
//    for (int i = 0; i < m_Ships.size(); i++) {
//        if (m_Ships[i].length == 0) {
//            numberDestroyed++;
//        }
//    }
//    
//    if (numberDestroyed == m_Ships.size()) {
//        return true;
//    }
//    
//    return false;
    
    int totalShipLength = 0;
    for (int k = 0; k < m_game.nShips() - 1; k++) {
        totalShipLength += m_game.shipLength(k);
    }
    
    int xCounter = 0;
    for (int i = 0; i < m_game.rows(); i++) {
        for (int j = 0; j < m_game.cols(); j++) {
            if (displayedBoard[i][j] == 'X')
                xCounter++;
        }
    }
    
    if (xCounter == totalShipLength) {
        return true;
    }
    
    return false;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
