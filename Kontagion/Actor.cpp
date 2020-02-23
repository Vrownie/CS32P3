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

void calcNewXY(int& x, int& y) {
    x += (x < VIEW_WIDTH / 2) ? SPRITE_RADIUS : -SPRITE_RADIUS;
    y += (y < VIEW_HEIGHT / 2) ? SPRITE_RADIUS : -SPRITE_RADIUS;
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

bool Actor::eat() { return false; }

Actor::~Actor() {}

//Socrates
Socrates::Socrates(StudentWorld* w_ptr) : Actor(IID_PLAYER, 0, VIEW_HEIGHT/2, 0, 0, true, w_ptr) {
    m_spray = 20;
    m_flame = 5;
    m_hp = 100;
}

void Socrates::doSomething() {
    if(!isAlive()) return;
    
    int ch;
    if(getWorld()->getKey(ch)) {
        double thetaC, rC, xC, yC;
        switch (ch)
        {
            case KEY_PRESS_SPACE:
                if (m_spray == 0) break;
                xC = 0;
                yC = 0;
                xC = getX() + 2 * SPRITE_RADIUS * cos(getDirection() * M_PI/180) - VIEW_WIDTH/2;
                yC = getY() + 2 * SPRITE_RADIUS * sin(getDirection() * M_PI/180) - VIEW_HEIGHT/2;
                getWorld()->addSpray(xC, yC, getDirection());
                m_spray--;
                getWorld()->playSound(SOUND_PLAYER_SPRAY);
                break;
            case KEY_PRESS_ENTER:
                if (m_flame == 0) break;
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
                rC = 0;
                thetaC = 5;
                movePolar(this, rC, thetaC);
                setDirection(thetaC + 180);
                break;
            case KEY_PRESS_RIGHT:
                rC = 0;
                thetaC = -5;
                movePolar(this, rC, thetaC);
                setDirection(thetaC + 180);
                break;
        }
    } else {
        if (m_spray < 20) m_spray++;
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

bool Food::eat() {
    setDead();
    return true;
}

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

//Flame Projectile
FlameProj::FlameProj(int xFromCenter, int yFromCenter, Direction dir, StudentWorld* w_ptr) : Projectile(xFromCenter, yFromCenter, dir, IID_FLAME, 32, w_ptr) {}

void FlameProj::doSomething() {
    if (!isAlive()) return;
    
    if(getWorld()->damageDamageable(this, 5)) {
        setDead();
        return;
    }
    
    moveAngle(getDirection(), 2*SPRITE_RADIUS);
    addDist(2*SPRITE_RADIUS);
    
    if(getDistTravelled() == getMaxTravel()) {
        setDead();
        return;
    }
}

FlameProj::~FlameProj() {}

//Spray Projectile

SprayProj::SprayProj(int xFromCenter, int yFromCenter, Direction dir, StudentWorld* w_ptr) : Projectile(xFromCenter, yFromCenter, dir, IID_SPRAY, 112, w_ptr) {}

void SprayProj::doSomething() {
    if (!isAlive()) return;
    
    if(getWorld()->damageDamageable(this, 2)) {
        setDead();
        return;
    }
    
    moveAngle(getDirection(), 2*SPRITE_RADIUS);
    addDist(2*SPRITE_RADIUS);
    
    if(getDistTravelled() == getMaxTravel()) {
        setDead();
        return;
    }
}

SprayProj::~SprayProj() {}

//Bacteria (virtual)
Bacteria::Bacteria(int x, int y, int ID, StudentWorld* w_ptr, int hp, int movementPlan, int amtDmg) : Actor(ID, x, y, 90, 0, true, w_ptr) {
    m_hp = hp;
    m_planDist = movementPlan;
    m_amtDmg = amtDmg;
    m_foodEaten = 0;
    getWorld()->playSound(SOUND_BACTERIUM_BORN);
}

void Bacteria::doSomething() {
    if (!isAlive()) return;
    
    int ind;
    if (getWorld()->overlapSocrates(this))
        ind = 1;
    else if (m_foodEaten == 3)
        ind = 2;
    else if (getWorld()->attemptEat(this))
        ind = 3;
    else //to make the compiler happy...
        ind = 0;
    
    switch(ind)
    {
        case 1: {
            getWorld()->damageSocrates(m_amtDmg);
            break;
        }
        case 2: {
            int x = getX();
            int y = getY();
            calcNewXY(x, y);
            addBacteria(x, y);
            m_foodEaten = 0;
            break;
        }
        case 3: {
            break;
        }
    }
    
    doSpecificThing();
}

Bacteria::~Bacteria() {}

