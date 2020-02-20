#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

void getValidCoords(list<Actor*> la, int& x, int& y) {
    bool overlapInd;
    do {
        overlapInd = false;
        randPolar(120, x, y);
        for(list<Actor*>::iterator i = la.begin(); i != la.end(); i++) {
            if ((*i)->allowsOverlap() && overlap((*i)->getX(), (*i)->getY(), x, y)) {
                overlapInd = true;
            }
        }
    }
    while (overlapInd);
}

bool overlap(const Actor* a1, const Actor* a2) {
    double dist = sqrt(pow(a1->getX() - a2->getX(), 2) + pow(a1->getY() - a2->getY(), 2));
    
    return dist < 2 * SPRITE_RADIUS;
}

bool overlap(int x1, int y1, int x2, int y2) {
    double dist = sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
    
    return dist < 2 * SPRITE_RADIUS;
}

bool MovementOverlap(const Bacteria& b, const Dirt& d) {
    double dist = sqrt(pow(b.getX() - d.getX(), 2) + pow(b.getY() - d.getY(), 2));
    
    return dist < SPRITE_RADIUS;
}

void randPolar(int r, int& x, int& y) {
    x = randInt(-r, r);
    y = randInt(-sqrt(pow(r, 2) - pow(x, 2)), sqrt(pow(r, 2) - pow(x, 2)));
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_player = new Socrates(this);
}

int StudentWorld::init()
{
    m_nPit = getLevel();
    m_nFood = min(getLevel() * 5, 25);
    m_nDirt = max(180 - getLevel() * 20, 20);
    
    int x, y;
    for (int n = 0; n < m_nFood; n++) {
        //check overlap here in part 2
        getValidCoords(m_list, x, y);
        m_list.push_back(new Food(x, y, this));
    }
    
    for (int n = 0; n < m_nDirt; n++) {
        //check overlap here in part 2
        getValidCoords(m_list, x, y);
        m_list.push_back(new Dirt(x, y, this));
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_player->doSomething();
    for (list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); i++) {
        if((*i)->isAlive())
            (*i)->doSomething();
        if(!m_player->isAlive())
            return GWSTATUS_PLAYER_DIED;
        if(m_list.empty())
            return GWSTATUS_FINISHED_LEVEL;
    }
    
    for (list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); ) {
        if(!(*i)->isAlive()) {
            delete *i;
            i = m_list.erase(i);
        } else
            i++;
    }
    
    ostringstream oss;
    oss << "Score: " << setfill('0') << setw(6) << getScore() << setw(0) << " Level: " << getLevel() << " Lives: " << getLives() << " Health: " << m_player->getHP() << " Sprays: " << m_player->getSpray() << " Flames: " << m_player->getFlame();
    
    //add new stuff here in part 2
    
    setGameStatText(oss.str());
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); i++) {
        delete *i;
        i = m_list.erase(i);
    }
    
    delete m_player;
}

bool StudentWorld::damageDamageable(Actor* ap, int n) {
    for (list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); i++) {
        if (overlap(ap, *i))
            return (*i)->damage(n);
    }
    return false;
}

void StudentWorld::addFlame(int x, int y, Direction dir) {
    m_list.push_back(new FlameProj(x, y, dir, this));
}

StudentWorld::~StudentWorld() { cleanUp(); }
