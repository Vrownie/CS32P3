#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    m_player = new Socrates;
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    for (list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); i++) {
        (*i)->doSomething();
    }
    m_player->doSomething();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); i++) {
        delete *i;
    }
}

StudentWorld::~StudentWorld() { cleanUp(); }
