#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//Helper functions
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

bool overlap(const Actor& a1, const Actor& a2) {
    double dist = sqrt(pow(a1.getX() - a2.getX(), 2) + pow(a1.getY() - a2.getY(), 2));
    
    return dist < 2 * SPRITE_RADIUS;
}

bool overlap(int x1, int y1, int x2, int y2) {
    double dist = sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
    
    return dist < 2 * SPRITE_RADIUS;
}

bool MovementOverlap(const Bacteria& b, const Dirt& d) {
    double dist = sqrt(pow(b.getX() - d.getX(), 2) + pow(b.getY() - d.getY(), 2));
    
    return dist < SPRITE_RADIUS;
}

void randPolar(int r, int& x, int& y) {
    x = randInt(-r, r);
    y = randInt(-sqrt(pow(r, 2) - pow(x, 2)), sqrt(pow(r, 2) - pow(x, 2)));
}

//Actor
Actor::Actor(int ID, int x, int y, Direction dir, int d, bool ind) : GraphObject(ID, x, y, dir, d) { m_isAlive = ind; }

bool Actor::isAlive() { return m_isAlive; }

void Actor::setDead() { m_isAlive = false; }

bool Actor::blocks() { return false; } //has to be a dirt

bool Actor::allowsOverlap() { return false; } //has to be a dirt

Actor::~Actor() {}

//Socrates
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
                newTheta += 5;
                polarToRect(newR, newTheta, newX, newY);
                moveTo(newX, newY);
                setDirection(newTheta + 180);
                break;
        case KEY_PRESS_RIGHT:
                rectToPolar(getX(), getY(), newR, newTheta);
                newTheta -= 5;
                polarToRect(newR, newTheta, newX, newY);
                moveTo(newX, newY);
                setDirection(newTheta + 180);
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

//Dirt
Dirt::Dirt(int xFromCenter, int yFromCenter) : Actor(IID_DIRT, VIEW_WIDTH / 2 + xFromCenter, VIEW_WIDTH / 2 + yFromCenter, 0, 1, true) {}

void Dirt::doSomething() {}

void Dirt::damage() { setDead(); }

Dirt::~Dirt() {}

bool Dirt::blocks() { return true; } //overrides Actor's blocks()

bool Dirt::allowsOverlap() { return true; } //overrides Actor's allowsOverlap()

//Food
Food::Food(int xFromCenter, int yFromCenter) : Actor(IID_FOOD, VIEW_WIDTH / 2 + xFromCenter, VIEW_WIDTH / 2 + yFromCenter, 90, 1, true) {}

void Food::doSomething() {}

void Food::damage(){}

Food::~Food(){}
