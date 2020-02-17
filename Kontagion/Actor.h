#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject { //Part 1
public:
    Actor(int ID, int x, int y, Direction dir, int d, bool ind);
    virtual void doSomething() = 0;
    virtual ~Actor();
    bool isAlive();
    void setDead();
    virtual void damage() = 0;
private:
    bool m_isAlive;
};

class Socrates : public Actor { //Part 1
public:
    Socrates();
    void doSomething();
    ~Socrates();
    void damage();
private:
    int m_spray;
    int m_flame;
    int m_hp;
};

class Bacteria : public Actor {
public:
    Bacteria();
    ~Bacteria();
private:
    int m_hp;
};

class regSal : public Bacteria {
    
};

class aggSal : public Bacteria {
    
};

class eColi : public Bacteria {
    
};

class Pit : public Actor {
    
};

class Projectile : public Actor {

};

class flameProj : public Projectile {
    
};

class sprayProj : public Projectile {
    
};

class Dirt : public Actor { //Part 1
public:
    Dirt(int x, int y);
    void doSomething();
    void damage();
    ~Dirt();
private:
    
};

class Food : public Actor {
    
};

class Goody : public Actor {
    
};

class healthG : public Goody {
    
};

class flameG : public Goody {
    
};

class lifeG : public Goody {
    
};

class Fungi : public Actor {
    
};

#endif // ACTOR_H_
