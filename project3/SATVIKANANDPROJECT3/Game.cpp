#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

using namespace std;

class GameShip {
public:
    // constructor for special ship class
    GameShip(int length, char symbol, string name) {
        m_length = length;
        m_symbol = symbol;
        m_name = name;
    }
    
    // accessors
    int returnLength() const {
        return m_length;
    }
    
    char returnSymbol() const {
        return m_symbol;
    }
    
    string returnName() const {
        return m_name;
    }
    
private:
    int m_length;
    char m_symbol;
    string m_name;
};

class GameImpl {
public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    vector<GameShip> m_ships;
    int m_nShips;
    int m_rows;
    int m_cols;
};

void waitForEnter() {
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols):m_rows(nRows), m_cols(nCols), m_nShips(0) {
}

int GameImpl::rows() const {
    return m_rows;
}

int GameImpl::cols() const {
    return m_cols;
}

bool GameImpl::isValid(Point p) const {
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const {
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name) {
    GameShip temp(length, symbol, name);
    m_ships.push_back(temp);
    m_nShips++;
    return true;
}

int GameImpl::nShips() const {
    return m_nShips;
}

int GameImpl::shipLength(int shipId) const {
    return m_ships[shipId].returnLength();
}

char GameImpl::shipSymbol(int shipId) const {
    return m_ships[shipId].returnSymbol();
}

string GameImpl::shipName(int shipId) const {
    return m_ships[shipId].returnName();
}

bool getLineWithTwoIntegers(int& r, int& c);
void extractCoordinates(Point p, int& r, int& c) {
    r = p.r;
    c = p.c;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause) {
    
    if (!p1->placeShips(b1))
        return nullptr;
    
    if (!p2->placeShips(b2))
        return nullptr;
    
    while (!b1.allShipsDestroyed() && !b2.allShipsDestroyed()) {
        bool shotHit = false;
        bool shipDestroyed = false;
        int shipId;
        int r = 0, c = 0;
        
        // player 1's turn to attack
        cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
        
        b2.display(p1->isHuman());
        if (p1->isHuman()) {
            p1->recommendAttack();
        } else {
            extractCoordinates(p1->recommendAttack(), r, c); // replaces r and c with the coordinates
        }                                                    // returned from recommend attack
        
        Point temp1(r, c);
        b2.attack(temp1, shotHit, shipDestroyed, shipId);
        
        cout << p1->name() << " attacked (" << r << ", " << c << ") and ";
        if (!shotHit)
            cout << "missed, resulting in:" << endl;
        if (shotHit && !shipDestroyed)
            cout << "hit something, resulting in:" << endl;
        if (shotHit && shipDestroyed)
            cout << "destroyed the " << shipName(shipId) << ", resulting in:" << endl;
        
        b2.display(p1->isHuman());
        
        if (b2.allShipsDestroyed()) {
            b1.display(false);
            return p1;
        }
        
        p1->recordAttackResult(temp1, isValid(temp1), shotHit, shipDestroyed, shipId);
        
        
        
        // player 2's turn to attack
        cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;
        b1.display(p2->isHuman());
        if (p2->isHuman()) {
            p2->recommendAttack();
        } else {
            extractCoordinates(p2->recommendAttack(), r, c);
        }
        Point temp2(r, c);
        b1.attack(temp2, shotHit, shipDestroyed, shipId);
        cout << p2->name() << " attacked (" << r << ", " << c << ") and ";
        if (!shotHit)
            cout << "missed, resulting in:" << endl;
        if (shotHit && !shipDestroyed)
            cout << "hit something, resulting in:" << endl;
        if (shotHit && shipDestroyed)
            cout << "destroyed the " << shipName(shipId) << ", resulting in:" << endl;
        b1.display(p2->isHuman());
        
        if (b1.allShipsDestroyed()) {
            b2.display(false);
            return p2;
        }
        
        p2->recordAttackResult(temp2, isValid(temp1), shotHit, shipDestroyed, shipId);
        
        if (shouldPause)
            waitForEnter();
    }
    return nullptr;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

