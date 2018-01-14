#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player {
public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman();
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    void setLastCellAttacked(int r, int c);
private:
    Point m_lastCellAttacked;
};

void HumanPlayer::setLastCellAttacked(int r, int c) {
    m_lastCellAttacked.r = r;
    m_lastCellAttacked.c = c;
}


HumanPlayer::HumanPlayer(string nm, const Game& g):Player(nm, g) {
    
}

bool HumanPlayer::isHuman() {
    return true;
}

bool HumanPlayer::placeShips(Board& b) {
    cout << name() << " must place 5 ships." << endl;
    
    for (int i = 0; i < game().nShips(); i++) {
        b.display(!isHuman());
        cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
        char dir;
        Direction input;
        cin >> dir;
        if (dir != 'h' && dir != 'v') {
            cout << "\nDirection must be h or v" << endl;
            i--;
            continue;
        } else {
            if (dir == 'h')
                input = HORIZONTAL;
            else input = VERTICAL;
        }
        int r = 0, c = 0;
        cout << "Enter row and column of leftmost cell (e.g. 3 5): ";
        getLineWithTwoIntegers(r, c);
        Point temp(r, c);
        if (b.placeShip(temp, i, input))
            continue;
        else {
            cout << "The ship cannot be placed there." << endl;
            i--;
            continue;
        }
    }

    return true;
}

Point HumanPlayer::recommendAttack() {
    cout << "Enter the row and column to attack (e.g, 3 5): ";
    int r = 0, c = 0;
    getLineWithTwoIntegers(r, c);
    m_lastCellAttacked.r = r;
    m_lastCellAttacked.c = c;
    return m_lastCellAttacked;
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                        bool shipDestroyed, int shipId) {
}

void HumanPlayer::recordAttackByOpponent(Point p) {
    
}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
//
class MediocrePlayer : public Player {
public:
    MediocrePlayer(string nm, const Game& g):Player(nm, g) {
        
    }
    
    virtual bool isHuman();
    
    bool recursivePlace(int shipId, Board& b);
    virtual bool placeShips(Board& b);
    
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    
    virtual void recordAttackByOpponent(Point p);
    
    
    void setState(int state) {
        m_state = state;
    }
    int state() {
        return m_state;
    }
private:
    Point m_lastCellAttacked;
    vector<Point> m_setOfAttackPoints;
    int m_state = 0;
};

bool MediocrePlayer::isHuman() {
    return false;
}

// recursive auxillary function to help place the ships
bool MediocrePlayer::recursivePlace(int shipId, Board& b) {
    if (shipId < 0)
        return true;
    
    if (shipId > game().nShips() - 1)
        return false;

    int counter = 0;
    for (int r = 0; r < game().rows(); r++) {
        for (int c = 0; c < game().cols(); c++) {
            counter++;
            Point temp(r, c);
            if (b.placeShip(temp, shipId, VERTICAL)) {
                return recursivePlace(shipId - 1, b);
            } else if (b.placeShip(temp, shipId, HORIZONTAL)) {
                return recursivePlace(shipId - 1, b);
            } else if (r == game().rows() && c == game().cols()) {
                for (int i = 0; i < game().rows(); i++) {
                    int flag = 0;
                    for (int j = 0; j < game().cols(); j++) {
                        Point temp2(i, j);
                        b.unplaceShip(temp2, shipId + 1, HORIZONTAL) || b.unplaceShip(temp2, shipId + 1, VERTICAL);
                        flag = 1;
                    }
                    if (flag != 0)
                        break;
                }
            } else
                continue;
        }
    }
    
    if (counter > 50)
        return false;
    return true;
}

bool MediocrePlayer::placeShips(Board& b) {
    b.block();
    
    bool ifPlaced = false;
    for (int i = 0; i < game().rows(); i++) {
        int flag = 0;
        for (int j = 0; j < game().cols(); j++) {
            Point temp(i, j);
            if (b.placeShip(temp, 0, HORIZONTAL) || b.placeShip(temp, 0, VERTICAL)) {
                ifPlaced = true;
                flag = 1;
            }
            if (flag != 0)
                break;
        }
    }
    
    if (!ifPlaced)
        return false;
    
    
    recursivePlace(game().nShips() - 1, b);
    
    b.unblock();
    return true;
}

// produce a random integer between -4 and 4.
Point MediocrePlayer::recommendAttack() {
    int r = 0, c = 0;
    if (state() == 0) {
        r = randInt(game().cols());
        c = randInt(game().rows());
    } else {// put a check
        bool goodValue = false;
        while (goodValue == false) {
            int dir = randInt(2);
            if (dir == 0) {
                r = m_lastCellAttacked.r;
                c = m_lastCellAttacked.c + randInt(randInt(5) - randInt(5));
            }
            
            if (dir == 1) {
                r = m_lastCellAttacked.r + randInt(randInt(5) - randInt(5));
                c = m_lastCellAttacked.c;
            }
            
            bool alreadyAttacked = false;
            for (int i = 0; i < m_setOfAttackPoints.size(); i++) {
                if (m_setOfAttackPoints[i].r == r && m_setOfAttackPoints[r].c == c) {
                    alreadyAttacked = true;
                    break;
                }
            }
            
            if (r < game().rows() && c < game().cols() && r >= 0 && c >= 0 && !alreadyAttacked)
                goodValue = true;
        }
    }
    
    Point output(r, c);
    return output;
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                        bool shipDestroyed, int shipId) {
    m_setOfAttackPoints.push_back(p);
    m_lastCellAttacked.r = p.r;
    m_lastCellAttacked.c = p.c;
    
    if (validShot && shotHit && !shipDestroyed)
        setState(1);
    
    if (validShot && shotHit && shipDestroyed)
        setState(0);
}

void MediocrePlayer::recordAttackByOpponent(Point p) {
    
}

//typedef AwfulPlayer MediocrePlayer;
// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
typedef AwfulPlayer GoodPlayer;

//class GoodPlayer : public Player {
//public:
//    GoodPlayer(string nm, const Game& g):Player(nm, g) {
//        
//    }
//    
//    virtual bool isHuman();
//    
//    bool recursivePlace(int shipId, Board& b);
//    virtual bool placeShips(Board& b);
//    
//    virtual Point recommendAttack();
//    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
//                                    bool shipDestroyed, int shipId);
//    
//    virtual void recordAttackByOpponent(Point p);
//    
//    
//    void setState(int state) {
//        m_state = state;
//    }
//    int state() {
//        return m_state;
//    }
//private:
//    Point m_lastCellAttacked;
//    vector<Point> m_setOfAttackPoints;
//    int m_state = 0;
//};
//
//bool GoodPlayer::isHuman() {
//    return false;
//}
//
//// recursive auxillary function to help place the ships
//bool GoodPlayer::recursivePlace(int shipId, Board& b) {
//    if (shipId < 0)
//        return true;
//    
//    if (shipId > game().nShips() - 1)
//        return false;
//    
//    int counter = 0;
//    for (int r = 0; r < game().rows(); r++) {
//        for (int c = 0; c < game().cols(); c++) {
//            counter++;
//            Point temp(r, c);
//            if (b.placeShip(temp, shipId, VERTICAL)) {
//                return recursivePlace(shipId - 1, b);
//            } else if (b.placeShip(temp, shipId, HORIZONTAL)) {
//                return recursivePlace(shipId - 1, b);
//            } else if (r == game().rows() && c == game().cols()) {
//                for (int i = 0; i < game().rows(); i++) {
//                    int flag = 0;
//                    for (int j = 0; j < game().cols(); j++) {
//                        Point temp2(i, j);
//                        b.unplaceShip(temp2, shipId + 1, HORIZONTAL) || b.unplaceShip(temp2, shipId + 1, VERTICAL);
//                        flag = 1;
//                    }
//                    if (flag != 0)
//                        break;
//                }
//            } else
//                continue;
//        }
//    }
//    
//    if (counter > 50)
//        return false;
//    return true;
//}
//
//bool GoodPlayer::placeShips(Board& b) {
//    b.block();
//    
//    bool ifPlaced = false;
//    for (int i = 0; i < game().rows(); i++) {
//        int flag = 0;
//        for (int j = 0; j < game().cols(); j++) {
//            Point temp(i, j);
//            if (b.placeShip(temp, 0, HORIZONTAL) || b.placeShip(temp, 0, VERTICAL)) {
//                ifPlaced = true;
//                flag = 1;
//            }
//            if (flag != 0)
//                break;
//        }
//    }
//    
//    if (!ifPlaced)
//        return false;
//    
//    
//    recursivePlace(game().nShips() - 1, b);
//    
//    b.unblock();
//    return true;
//}
//
//Point GoodPlayer::recommendAttack() {
//    int r = 0, c = 0;
//    if (state() == 0) {
//        r = randInt(game().cols());
//        c = randInt(game().rows());
//    } else {
//        
//        int possibilities = 0;
//        
//        if (m_lastCellAttacked.r + 1 < game().rows()) {
//            Point temp(m_lastCellAttacked.r + 1, m_lastCellAttacked.c);
//            m_setOfAttackPoints.push_back(temp);
//            possibilities++;
//        }
//        
//        if (m_lastCellAttacked.r - 1 >= 0) {
//            Point temp(m_lastCellAttacked.r - 1, m_lastCellAttacked.c);
//            m_setOfAttackPoints.push_back(temp);
//            possibilities++;
//        }
//        
//        if (m_lastCellAttacked.c + 1 < game().cols()) {
//            Point temp(m_lastCellAttacked.r, m_lastCellAttacked.c + 1);
//            m_setOfAttackPoints.push_back(temp);
//            possibilities++;
//        }
//        
//        if (m_lastCellAttacked.c - 1 >= 0) {
//            Point temp(m_lastCellAttacked.r, m_lastCellAttacked.c - 1);
//            m_setOfAttackPoints.push_back(temp);
//            possibilities++;
//        }
//        
//        int indexOfAttack = randInt(possibilities);
//        r = m_setOfAttackPoints[indexOfAttack].r;
//        c = m_setOfAttackPoints[indexOfAttack].c;
//    }
//    
//    Point output(r, c);
//    return output;
//}
//
//void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
//                                        bool shipDestroyed, int shipId) {
//    m_lastCellAttacked.r = p.r;
//    m_lastCellAttacked.c = p.c;
//    
//    if (validShot && shotHit && !shipDestroyed)
//        setState(1);
//    
//    if (validShot && shotHit && shipDestroyed)
//        setState(0);
//}
//
//void GoodPlayer::recordAttackByOpponent(Point p) {
//    
//}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
