#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject { //Part 1
public:
    Actor(int ID, int x, int y, Direction dir, int d, bool ind, StudentWorld* w_ptr);
    virtual void doSomething() = 0;
    virtual ~Actor();
    bool isAlive();
    void setDead();
    StudentWorld* getWorld();
    virtual bool damage(int n) = 0; //true if damageable
    virtual bool eat(); //only food can be eaten
    virtual bool blocks();
    virtual bool allowsOverlap();
private:
    bool m_isAlive;
    StudentWorld* m_world;
};

class Socrates : public Actor { //Part 1
public:
    Socrates(StudentWorld* w_ptr);
    virtual void doSomething();
    ~Socrates();
    bool damage(int n);
    int getSpray();
    int getFlame();
    int getHP();
private:
    int m_spray;
    int m_flame;
    int m_hp;
};

class Bacteria : public Actor {
public:
    Bacteria(int x, int y, int ID, StudentWorld* w_ptr, int hp, int movementPlan, int amtDmg); //play sound
    virtual void doSomething();
    virtual void doSpecificThing() = 0;
    virtual void addBacteria(int x, int y) = 0;
    virtual bool damage(int n) = 0; //play sound
    ~Bacteria();
private:
    int m_hp;
    int m_planDist;
    int m_amtDmg;
    int m_foodEaten;
};

class Salmonella : public Bacteria {
    Salmonella(int x, int y);
    virtual void doSomething();
    virtual void doSpecificThing() = 0;
    virtual void addBacteria(int x, int y);
    virtual bool damage(int n); //play sound
    ~Salmonella();
};

class AggSal : public Salmonella {
    AggSal(int x, int y);
    virtual void doSomething();
    virtual void doSpecificThing() = 0;
    virtual void addBacteria(int x, int y);
    ~AggSal();
};

class RegSal : public Salmonella {
    RegSal(int x, int y);
    virtual void doSomething();
    virtual void doSpecificThing() = 0;
    virtual void addBacteria(int x, int y);
    ~RegSal();
};

class EColi : public Bacteria {
    EColi(int x, int y);
    virtual void doSomething();
    virtual void doSpecificThing() = 0;
    virtual void addBacteria(int x, int y);
    virtual bool damage(int n); //play sound
    ~EColi();
};

class Pit : public Actor {
    
};

class Projectile : public Actor {
public:
    Projectile(int xFromCenter, int yFromCenter, Direction dir, int ID, int max, StudentWorld* w_ptr);
    bool damage(int n);
    virtual void doSomething() = 0;
    int getMaxTravel();
    int getDistTravelled();
    void addDist(int n);
    virtual ~Projectile();
private:
    int m_maxTravel;
    int m_distTravelled;
};

class FlameProj : public Projectile {
public:
    FlameProj(int xFromCenter, int yFromCenter, Direction dir, StudentWorld* w_ptr);
    virtual void doSomething();
    ~FlameProj();
};

class SprayProj : public Projectile {
public:
    SprayProj(int xFromCenter, int yFromCenter, Direction dir, StudentWorld* w_ptr);
    virtual void doSomething();
    ~SprayProj();
};

class Dirt : public Actor { //Part 1
public:
    Dirt(int xFromCenter, int yFromCenter, StudentWorld* w_ptr);
    virtual void doSomething();
    bool damage(int n);
    ~Dirt();
    bool blocks();
    bool allowsOverlap();
};

class Food : public Actor {
public:
    Food(int xFromCenter, int yFromCenter, StudentWorld* w_ptr);
    virtual void doSomething();
    virtual bool eat(); //overrides default
    bool damage(int n);
    ~Food();
};

class Goodie : public Actor {
    
};

class HealthG : public Goodie {
    
};

class FlameG : public Goodie {
    
};

class LifeG : public Goodie {
    
};

class Fungi : public Actor {
    
};

void polarToRect(double r, double theta, double& x, double& y);

void rectToPolar(double r, double theta, double& x, double& y);

void movePolar(Actor* ap, double& r, double& theta);

void calcNewXY(int& x, int& y);

#endif // ACTOR_H_
