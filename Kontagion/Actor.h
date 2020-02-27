#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(int ID, double x, double y, Direction dir, int d, int hp, bool ind, StudentWorld* w_ptr);
    virtual ~Actor();
    virtual void doSomething() = 0;
    
    //non-virtual functions
    int getHP();
    StudentWorld* getWorld();
    void setHP(int n);
    void setDead();
    bool isAlive();
    
    //indicators
    virtual bool blocks();
    virtual bool allowsOverlap();
    virtual bool edible();
    
    //mutators
    virtual bool damage(int n); //true if damageable
    virtual bool eat(); //only food can be eaten
private:
    int m_hp;
    bool m_isAlive;
    StudentWorld* m_world;
};

class Socrates : public Actor {
public:
    Socrates(StudentWorld* w_ptr);
    virtual ~Socrates();
    virtual void doSomething();
    virtual bool damage(int n);
    
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
    Bacteria(double x, double y, int ID, StudentWorld* w_ptr, int hp, int movementPlan, int amtDmg); //play sound
    virtual ~Bacteria();
    virtual void doSomething();
    virtual void doSpecificThing() = 0;
    virtual bool damage(int n); //play sound, decrease the bacteria counter in m_world
    
    virtual void playHurtSound() = 0;
    virtual void playDeadSound() = 0;
    virtual void addBacteria(int x, int y) = 0;
    
    int getPlanDist();
    void setPlanDist(int newPlan);
private:
    int m_planDist;
    int m_amtDmg;
    int m_moveDist;
    int m_foodEaten;
};

class Salmonella : public Bacteria {
public:
    Salmonella(double x, double y, StudentWorld* w_ptr, int hp, int amtdmg);
    virtual ~Salmonella();
    virtual void doSpecificThing();
    
    virtual void addBacteria(int x, int y) = 0;
    virtual void playHurtSound();
    virtual void playDeadSound();
};

class RegSal : public Salmonella {
public:
    RegSal(double x, double y, StudentWorld* w_ptr);
    virtual ~RegSal();
    
    virtual void addBacteria(int x, int y);
};

class AggSal : public Salmonella {
public:
    AggSal(double x, double y, StudentWorld* w_ptr);
    virtual ~AggSal();
    
    virtual void addBacteria(int x, int y);
    virtual void doSomething(); //overrides doSomething, adds an extra step
    virtual void doSpecificThing(); //overrides Salmonella's, when needed
};

class EColi : public Bacteria {
public:
    EColi(double x, double y, StudentWorld* w_ptr);
    virtual ~EColi();
    virtual void doSpecificThing();
    
    virtual void addBacteria(int x, int y);
    virtual void playHurtSound();
    virtual void playDeadSound();
};

class Pit : public Actor {
public:
    Pit(double x, double y, StudentWorld* w_ptr);
    virtual ~Pit();
    virtual void doSomething();
    virtual bool damage(int n); //overrides, cannot be damaged
private:
    int m_nEColi;
    int m_nRegSal;
    int m_nAggSal;
};

class Projectile : public Actor {
public:
    Projectile(double x, double y, Direction dir, int ID, int max, int dmg, StudentWorld* w_ptr);
    virtual ~Projectile();
    virtual void doSomething();
    virtual bool damage(int n); //overrides, cannot be damaged
    
    int getMaxTravel();
    int getDistTravelled();
    void addDist(int n);
private:
    int m_amtDmg;
    int m_maxTravel;
    int m_distTravelled;
};

class FlameProj : public Projectile {
public:
    FlameProj(double x, double y, Direction dir, StudentWorld* w_ptr);
    virtual ~FlameProj();
};

class SprayProj : public Projectile {
public:
    SprayProj(double x, double y, Direction dir, StudentWorld* w_ptr);
    virtual ~SprayProj();
};

class Dirt : public Actor {
public:
    Dirt(double xFromCenter, double yFromCenter, StudentWorld* w_ptr);
    virtual ~Dirt();
    virtual void doSomething();
    
    virtual bool blocks();
    virtual bool allowsOverlap();
};

class Food : public Actor {
public:
    Food(double xFromCenter, double yFromCenter, StudentWorld* w_ptr);
    virtual ~Food();
    virtual void doSomething();
    
    virtual bool damage(int n); //overrides default
    virtual bool edible(); //overrides default
    virtual bool eat(); //overrides default
};

class Goodie : public Actor { //also includes Fungus
public:
    Goodie(double xFromCenter, double yFromCenter, int ID, int awardPt, StudentWorld* w_ptr);
    virtual ~Goodie();
    virtual void doSomething();
    virtual void doSpecificThing() = 0;
    
    virtual void playSound();
    virtual bool damage(int n); //overrides, cannot be damaged
private:
    int m_lifeTime;
    int m_award;
};

class HealthG : public Goodie {
public:
    HealthG(double xFromCenter, double yFromCenter, StudentWorld* w_ptr);
    virtual void doSpecificThing();
    virtual ~HealthG();
};

class FlameG : public Goodie {
public:
    FlameG(double xFromCenter, double yFromCenter, StudentWorld* w_ptr);
    virtual ~FlameG();
    
    virtual void doSpecificThing();
};

class LifeG : public Goodie {
public:
    LifeG(double xFromCenter, double yFromCenter, StudentWorld* w_ptr);
    virtual ~LifeG();
    
    virtual void doSpecificThing();
};

class Fungus : public Goodie {
public:
    Fungus(double xFromCenter, double yFromCenter, StudentWorld* w_ptr);
    virtual void doSpecificThing();
    virtual ~Fungus();
    
    virtual void playSound(); //overrides Goodie's
    virtual bool damage(int n); //overrides Actor's
};

void polarToRect(double r, Direction theta, double& x, double& y);

void rectToPolar(double r, double theta, double& x, Direction& y);

void movePolar(Actor* ap, double& r, double& theta);

double calcDistance(Actor* a1, Actor* a2);

void calcNewXY(double& x, double& y);

bool isValidCoord(double x, double y);

#endif // ACTOR_H_
