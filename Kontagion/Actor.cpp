#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int ID, int x, int y, Direction dir, int d, bool ind) : GraphObject(ID, x, y, dir, d) { m_isAlive = ind; }

bool Actor::isAlive() { return m_isAlive; }

void Actor::setDead() { m_isAlive = false; }

Actor::~Actor() {}

Socrates::Socrates() : Actor(IID_PLAYER, 0, VIEW_HEIGHT/2, 0, 0, true) {
    m_spray = 20;
    m_flame = 5;
    m_hp = 100;
}

void Socrates::doSomething() {
//    if(!isAlive()) return;
}

void Socrates::damage() {
    
}

Socrates::~Socrates() {};

Dirt::Dirt(int x, int y) : Actor(IID_DIRT, x, y, 90, 1, true) {}

void Dirt::doSomething() {}

void Dirt::damage() { setDead(); }

Dirt::~Dirt() {}
