#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    ~StudentWorld();
private:
    std::list<Actor*> m_list;
    Socrates* m_player;
    int m_nPit;
    int m_nFood;
    int m_nDirt;
};

void getValidCoords(Actor* aa, int& x, int& y);

#endif // STUDENTWORLD_H_
