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
    
    
    for (int n = 0; n < m_nDirt; n++) {
        //check overlap here in part 2
        int x = randInt(-120, 120);
        int y = randInt(-sqrt(pow(120, 2) - pow(x, 2)), sqrt(pow(120, 2) - pow(x, 2)));
        m_list.push_back(new Dirt(x, y));
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_player->doSomething();
    for (list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); i++) {
        if((*i)->isAlive()) {
            (*i)->doSomething();
        }
        if(!m_player->isAlive()) {
            return GWSTATUS_PLAYER_DIED;
        }
        if(m_list.empty()) {
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    for (list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); ) {
        if(!(*i)->isAlive()) {
            i = m_list.erase(i);
        } else {
            i++;
        }
    }
    
    ostringstream oss;
    oss << "Score: " << setfill('0') << setw(6) << getScore() << setw(0) << " Level: " << getLevel() << " Lives: " << getLives() << " health: " << m_player->getHP() << " Sprays: " << m_player->getSpray() << " Flames: " << m_player->getFlame();
    
    //add new stuff here in part 2
    
    setGameStatText(oss.str());
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); i++) {
        delete *i;
    }
    
    delete m_player;
}

StudentWorld::~StudentWorld() { cleanUp(); }
