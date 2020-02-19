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
    bool damageDamageable(Actor* ap, int n);
    void addFlame(int x, int y, Direction dir);
    ~StudentWorld();
private:
    std::list<Actor*> m_list;
    Socrates* m_player;
    int m_nPit;
    int m_nFood;
    int m_nDirt;
};

void getValidCoords(Actor* aa, int& x, int& y);

bool overlap(const Actor* a1, const Actor* a2);

bool overlap(int x1, int y1, int x2, int y2);

bool MovementOverlap(const Bacteria& b, const Dirt& d);

void randPolar(int r, int& x, int& y);

#endif // STUDENTWORLD_H_
