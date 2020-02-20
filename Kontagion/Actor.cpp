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

void movePolar(Actor* ap, double& r, double& theta) {
    double newX, newY, newR, newTheta;
    rectToPolar(ap->getX(), ap->getY(), newR, newTheta);
    newR += r;
    newTheta += theta;
    polarToRect(newR, newTheta, newX, newY);
    ap->moveTo(newX, newY);
    r = newR;
    theta = newTheta;
}

//Actor
Actor::Actor(int ID, int x, int y, Direction dir, int d, bool ind, StudentWorld* w_ptr) : GraphObject(ID, x, y, dir, d) {
    m_isAlive = ind;
    m_world = w_ptr;
}

bool Actor::isAlive() { return m_isAlive; }

void Actor::setDead() { m_isAlive = false; }

StudentWorld* Actor::getWorld() { return m_world; }

bool Actor::blocks() { return false; } //has to be a dirt

bool Actor::allowsOverlap() { return false; } //has to be a dirt

Actor::~Actor() {}

//Socrates
Socrates::Socrates(StudentWorld* w_ptr) : Actor(IID_PLAYER, 0, VIEW_HEIGHT/2, 0, 0, true, w_ptr) {
    m_spray = 100;
    m_flame = 100;
    m_hp = 100;
}

void Socrates::doSomething() {
    if(!isAlive()) return;
    int ch;
    
    if(getWorld()->getKey(ch)) {
        double newR, newTheta;
        switch (ch)
        {
            case KEY_PRESS_SPACE:
                //Add here
                break;
            case KEY_PRESS_ENTER:
                if (m_flame == 0) break;
                double thetaC, xC, yC;
                for (int i = 0; i < 16; i++) {
                    xC = 0;
                    yC = 0;
                    thetaC = (getDirection() + 22 * i) % 360;
                    xC = getX() + 2 * SPRITE_RADIUS * cos(thetaC * M_PI/180) - VIEW_WIDTH/2;
                    yC = getY() + 2 * SPRITE_RADIUS * sin(thetaC * M_PI/180) - VIEW_HEIGHT/2;
                    getWorld()->addFlame(xC, yC, thetaC);
                }
                m_flame--;
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                break;
            case KEY_PRESS_LEFT:
                newR = 0;
                newTheta = 5;
                movePolar(this, newR, newTheta);
                setDirection(newTheta + 180);
                break;
            case KEY_PRESS_RIGHT:
                newR = 0;
                newTheta = -5;
                movePolar(this, newR, newTheta);
                setDirection(newTheta + 180);
                break;
            default:
                if (m_spray < 20) m_spray++;
        }
    }
}

bool Socrates::damage(int n) {
    m_hp -= n;
    return true;
} //temporary

int Socrates::getSpray() { return m_spray; }

int Socrates::getFlame() { return m_flame; }

int Socrates::getHP() { return m_hp; }

Socrates::~Socrates() {}

//Dirt
Dirt::Dirt(int xFromCenter, int yFromCenter, StudentWorld* w_ptr) : Actor(IID_DIRT, VIEW_WIDTH / 2 + xFromCenter, VIEW_HEIGHT / 2 + yFromCenter, 0, 1, true, w_ptr) {}

void Dirt::doSomething() {}

bool Dirt::damage(int n) {
    setDead();
    return true;
}

Dirt::~Dirt() {}

bool Dirt::blocks() { return true; } //overrides Actor's blocks()

bool Dirt::allowsOverlap() { return true; } //overrides Actor's allowsOverlap()

//Food
Food::Food(int xFromCenter, int yFromCenter, StudentWorld* w_ptr) : Actor(IID_FOOD, VIEW_WIDTH / 2 + xFromCenter, VIEW_HEIGHT / 2 + yFromCenter, 90, 1, true, w_ptr) {}

void Food::doSomething() {}

bool Food::damage(int n) { return false; }

Food::~Food() {}

//Projectile (virtual)
Projectile::Projectile(int xFromCenter, int yFromCenter, Direction dir, int ID, int max, StudentWorld* w_ptr) : Actor(ID, VIEW_WIDTH / 2 + xFromCenter, VIEW_HEIGHT / 2 + yFromCenter, dir, 1, true, w_ptr) {
    m_maxTravel = max;
    m_distTravelled = 0;
}

bool Projectile::damage(int n) {
    return false;
}

int Projectile::getMaxTravel() { return m_maxTravel; }

int Projectile::getDistTravelled() { return m_distTravelled; }

void Projectile::addDist(int n) { m_distTravelled += n; }

Projectile::~Projectile() {}

//Flame
FlameProj::FlameProj(int xFromCenter, int yFromCenter, Direction dir, StudentWorld* w_ptr) : Projectile(xFromCenter, yFromCenter, dir, IID_FLAME, 32, w_ptr) {}

void FlameProj::doSomething() {
    if (!isAlive()) return;
    
    
    if(getWorld()->damageDamageable(this, 5)) {
        setDead();
        return;
    }
    
//    double x, y;
//    x = getX() + 2 * SPRITE_RADIUS * cos(getDirection()*M_PI/180);
//    y = getY() + 2 * SPRITE_RADIUS * sin(getDirection()*M_PI/180);
//    polarToRect(2 * SPRITE_RADIUS, getDirection(), x, y);
//    x += getX() - VIEW_WIDTH / 2;
//    y += getY() - VIEW_HEIGHT / 2;
    
    moveAngle(getDirection(), 2*SPRITE_RADIUS);
    addDist(2*SPRITE_RADIUS);
    
    if(getDistTravelled() == getMaxTravel()) {
        setDead();
        return;
    }
}

FlameProj::~FlameProj() {}
