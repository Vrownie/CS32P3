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
    virtual bool blocks();
    virtual bool allowsOverlap();
private:
    bool m_isAlive;
    StudentWorld* m_world;
};

class Socrates : public Actor { //Part 1
public:
    Socrates(StudentWorld* w_ptr);
    void doSomething();
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
    Bacteria();
    ~Bacteria();
private:
    int m_hp;
};

class RegSal : public Bacteria {
    
};

class AggSal : public Bacteria {
    
};

class EColi : public Bacteria {
    
};

class Pit : public Actor {
    
};

class Projectile : public Actor {
public:
    Projectile(int xFromCenter, int yFromCenter, Direction dir, int ID, int max, StudentWorld* w_ptr);
    bool damage(int n);
    virtual void doSomething() = 0;
    virtual bool getMaxTravel();
    virtual ~Projectile();
private:
    int m_maxTravel;
};

class FlameProj : public Projectile {
public:
    FlameProj(int xFromCenter, int yFromCenter, Direction dir, StudentWorld* w_ptr);
    virtual void doSomething();
    ~FlameProj();
private:
    int m_distTravelled;
};

class SprayProj : public Projectile {
public:
    SprayProj(int xFromCenter, int yFromCenter, Direction dir, StudentWorld* w_ptr);
    virtual void doSomething();
    ~SprayProj();
private:
    int m_distTravelled;
};

class Dirt : public Actor { //Part 1
public:
    Dirt(int xFromCenter, int yFromCenter, StudentWorld* w_ptr);
    void doSomething();
    bool damage(int n);
    ~Dirt();
    bool blocks();
    bool allowsOverlap();
};

class Food : public Actor {
public:
    Food(int xFromCenter, int yFromCenter, StudentWorld* w_ptr);
    void doSomething();
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

#endif // ACTOR_H_
