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
    ~StudentWorld();
    
    //main functions
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    //adding objects
    void addFlame(double x, double y, Direction dir);
    void addSpray(double x, double y, Direction dir);
    void addRegSal(double x, double y);
    void addAggSal(double x, double y);
    void addEColi(double x, double y);
    void addFood(double x, double y);
    
    //involving Socrates
    void restoreSocrates();
    void damageSocrates(int n);
    void giveFlameToSocrates(int n);
    bool overlapSocrates(Actor* ap);
    double calcDistSocrates(Actor* ap);
    Direction calcAngleSocrates(Actor* ap);
    
    //counter modifiers
    void incBacteriaCount();
    void decBacteriaCount();
    void decPitCount();
    
    //other modifiers & checkers
    bool damageDamageable(Actor* ap, int n);
    bool findFood(Actor* ap, double dist, Direction& results);
    bool attemptEat(Actor* ap);
    bool attemptMove(Bacteria* bp, double dist, Direction dir);
private:
    //Objects
    std::list<Actor*> m_list;
    Socrates* m_player;
    
    //Counters
    int m_nPit;
    int m_nFood;
    int m_nDirt;
    int m_nBacteria;
    int m_chanceFungus;
    int m_chanceGoodie;
};

void getValidCoords(Actor* aa, double& x, double& y);

bool overlap(const Actor* a1, const Actor* a2);

bool overlap(double x1, double y1, double x2, double y2);

bool movementOverlap(const Bacteria* b, const Dirt* d);

void randPolar(double r, double& x, double& y);

#endif // STUDENTWORLD_H_
