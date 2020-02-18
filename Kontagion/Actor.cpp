#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void polarToRect(double r, double theta, double& x, double& y) {
    x = VIEW_WIDTH / 2 + r * cos(theta * M_PI / 180);
    y = VIEW_HEIGHT / 2 + r * sin(theta * M_PI / 180);
}

void rectToPolar(double x, double y, double& r, double& theta) {
    r = sqrt(pow((VIEW_WIDTH / 2 - x), 2) + pow((VIEW_HEIGHT / 2 - y), 2));
    theta = atan((VIEW_HEIGHT / 2 - y) / (VIEW_WIDTH / 2 - x)) * 180 / M_PI;
    if (x - VIEW_WIDTH / 2 < 0)
        theta += 180;
    else if (y - VIEW_HEIGHT / 2 < 0)
        theta += 360;
}

Actor::Actor(int ID, int x, int y, Direction dir, int d, bool ind) : GraphObject(ID, x, y, dir, d) { m_isAlive = ind; }

bool Actor::isAlive() { return m_isAlive; }

void Actor::setDead() { m_isAlive = false; }

Actor::~Actor() {}

Socrates::Socrates(StudentWorld* w_ptr) : Actor(IID_PLAYER, 0, VIEW_HEIGHT/2, 0, 0, true) {
    m_spray = 20;
    m_flame = 5;
    m_hp = 100;
    m_world = w_ptr;
}

void Socrates::doSomething() {
    if(!isAlive()) return;
    int ch;
    
    if(m_world->getKey(ch)) {
        double newX, newY, newR, newTheta;
        switch (ch)
        {
        case KEY_PRESS_LEFT:
                rectToPolar(getX(), getY(), newR, newTheta);
                polarToRect(newR, newTheta + 5, newX, newY);
                moveTo(newX, newY);
                break;
        case KEY_PRESS_RIGHT:
                rectToPolar(getX(), getY(), newR, newTheta);
                polarToRect(newR, newTheta - 5, newX, newY);
                moveTo(newX, newY);
                break;
        case KEY_PRESS_SPACE:
                break;
        }
    }
}

void Socrates::damage() {
    
}

int Socrates::getSpray() {
    return m_spray;
}

int Socrates::getFlame() {
    return m_flame;
}

int Socrates::getHP() {
    return m_hp;
}

Socrates::~Socrates() {};

Dirt::Dirt(int xFromCenter, int yFromCenter) : Actor(IID_DIRT, VIEW_WIDTH / 2 + xFromCenter, VIEW_WIDTH / 2 + yFromCenter, 90, 1, true) {}

void Dirt::doSomething() {}

void Dirt::damage() { setDead(); }

Dirt::~Dirt() {}

