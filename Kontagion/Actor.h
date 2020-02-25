#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject { //Part 1
public:
    Actor(int ID, int x, int y, Direction dir, int d, int hp, bool ind, StudentWorld* w_ptr);
    virtual void doSomething() = 0;
    virtual ~Actor();
    
    //indicators
    bool isAlive();
    StudentWorld* getWorld();
    virtual bool blocks();
    virtual bool allowsOverlap();
    
    //accessors
    int getHP();
    
    //mutators
    void setHP(int n);
    void setDead();
    virtual bool damage(int n); //true if damageable
    virtual bool eat(); //only food can be eaten
private:
    int m_hp;
    bool m_isAlive;
    StudentWorld* m_world;
};

class Socrates : public Actor { //Part 1
public:
    Socrates(StudentWorld* w_ptr);
    virtual void doSomething();
    virtual ~Socrates();
    void restoreHP();
    void addFlame(int n);
    int getSpray();
    int getFlame();
private:
    int m_spray;
    int m_flame;
};

class Bacteria : public Actor {
public:
    Bacteria(int x, int y, int ID, StudentWorld* w_ptr, int hp, int movementPlan, int amtDmg); //play sound
    virtual void doSomething();
    virtual void doSpecificThing() = 0;
    virtual void addBacteria(int x, int y) = 0;
    virtual bool damage(int n) = 0; //play sound, decrease the bacteria counter in m_world
    virtual ~Bacteria();
private:
    int m_planDist;
    int m_amtDmg;
    int m_moveDist;
    int m_foodEaten;
};

class Salmonella : public Bacteria {
    Salmonella(int x, int y, StudentWorld* w_ptr);
    virtual void doSpecificThing() = 0;
    virtual void addBacteria(int x, int y);
    virtual bool damage(int n); //play sound
    virtual ~Salmonella();
};

class AggSal : public Salmonella {
public:
    AggSal(int x, int y, StudentWorld* w_ptr);
    virtual void doSpecificThing() = 0;
    virtual void addBacteria(int x, int y);
    virtual ~AggSal();
};

class RegSal : public Salmonella {
public:
    RegSal(int x, int y, StudentWorld* w_ptr);
    virtual void doSomething();
    virtual void doSpecificThing() = 0;
    virtual void addBacteria(int x, int y);
    virtual ~RegSal();
};

class EColi : public Bacteria {
public:
    EColi(int x, int y, StudentWorld* w_ptr);
    virtual void doSpecificThing();
    virtual void addBacteria(int x, int y);
    virtual bool damage(int n); //play sound
    virtual ~EColi();
};

class Pit : public Actor {
    
};

class Projectile : public Actor {
public:
    Projectile(int xFromCenter, int yFromCenter, Direction dir, int ID, int max, StudentWorld* w_ptr);
    virtual void doSomething() = 0;
    int getMaxTravel();
    int getDistTravelled();
    void addDist(int n);
    virtual bool damage(int n); //not damageable! needs override
    virtual ~Projectile();
private:
    int m_maxTravel;
    int m_distTravelled;
};

class FlameProj : public Projectile {
public:
    FlameProj(int xFromCenter, int yFromCenter, Direction dir, StudentWorld* w_ptr);
    virtual void doSomething();
    virtual ~FlameProj();
};

class SprayProj : public Projectile {
public:
    SprayProj(int xFromCenter, int yFromCenter, Direction dir, StudentWorld* w_ptr);
    virtual void doSomething();
    virtual ~SprayProj();
};

class Dirt : public Actor { //Part 1
public:
    Dirt(int xFromCenter, int yFromCenter, StudentWorld* w_ptr);
    virtual void doSomething();
    virtual ~Dirt();
    bool blocks();
    bool allowsOverlap();
};

class Food : public Actor {
public:
    Food(int xFromCenter, int yFromCenter, StudentWorld* w_ptr);
    virtual void doSomething();
    virtual bool eat(); //overrides default
    virtual ~Food();
};

class Goodie : public Actor {
public:
    Goodie(int xFromCenter, int yFromCenter, int ID, int awardPt, StudentWorld* w_ptr);
    virtual void doSomething();
    virtual void doSpecificThing() = 0;
    virtual bool damage(int n);
    virtual ~Goodie();
private:
    int m_lifeTime;
    int m_award;
};

class HealthG : public Goodie {
public:
    HealthG(int xFromCenter, int yFromcenter, StudentWorld* w_ptr);
    virtual void doSpecificThing();
    virtual ~HealthG();
};

class FlameG : public Goodie {
public:
    FlameG(int xFromCenter, int yFromcenter, StudentWorld* w_ptr);
    virtual void doSpecificThing();
    virtual ~FlameG();
};

class LifeG : public Goodie {
public:
    LifeG(int xFromCenter, int yFromcenter, StudentWorld* w_ptr);
    virtual void doSpecificThing();
    virtual ~LifeG();
};

class Fungi : public Actor {
    
};

void polarToRect(double r, double theta, double& x, double& y);

void rectToPolar(double r, double theta, double& x, double& y);

void movePolar(Actor* ap, double& r, double& theta);

int calcDistance(Actor* a1, Actor* a2);

void calcNewXY(int& x, int& y);

#endif // ACTOR_H_
