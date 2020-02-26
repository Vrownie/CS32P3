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

int calcDistance(Actor* a1, Actor* a2) {
    return sqrt(pow(a1->getX()-a2->getX(), 2)+pow(a1->getY()-a2->getY(), 2));
}

void calcNewXY(int& x, int& y) {
    x += (x < VIEW_WIDTH / 2) ? SPRITE_RADIUS : -SPRITE_RADIUS;
    y += (y < VIEW_HEIGHT / 2) ? SPRITE_RADIUS : -SPRITE_RADIUS;
}

bool isValidCoord(int x, int y) { return sqrt(pow(x - VIEW_WIDTH / 2, 2) + pow(y - VIEW_HEIGHT / 2, 2)) < VIEW_RADIUS; }

//Actor
Actor::Actor(int ID, int x, int y, Direction dir, int d, int hp, bool ind,  StudentWorld* w_ptr) : GraphObject(ID, x, y, dir, d) {
    m_hp = hp;
    m_isAlive = ind;
    m_world = w_ptr;
}

bool Actor::isAlive() { return m_isAlive; }

void Actor::setDead() { m_isAlive = false; }

StudentWorld* Actor::getWorld() { return m_world; }

bool Actor::blocks() { return false; } //has to be a dirt

bool Actor::allowsOverlap() { return false; } //has to be a dirt

bool Actor::edible() { return false; } //has to be a food

bool Actor::eat() { return false; }

int Actor::getHP() { return m_hp; }

void Actor::setHP(int n) { m_hp = n; }

bool Actor::damage(int n) {
    if (n <= 0) return false;
    
    m_hp -= n;
    if (m_hp <= 0) setDead();
    return true;
}

Actor::~Actor() {}

//Socrates
Socrates::Socrates(StudentWorld* w_ptr) : Actor(IID_PLAYER, 0, VIEW_HEIGHT/2, 0, 0, 100, true, w_ptr) {
    m_spray = 20;
    m_flame = 5;
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

void Socrates::restoreHP() { setHP(100); }

void Socrates::addFlame(int n) { m_flame += n; }

int Socrates::getSpray() { return m_spray; }

int Socrates::getFlame() { return m_flame; }

Socrates::~Socrates() {}

//Dirt
Dirt::Dirt(int xFromCenter, int yFromCenter, StudentWorld* w_ptr) : Actor(IID_DIRT, VIEW_WIDTH / 2 + xFromCenter, VIEW_HEIGHT / 2 + yFromCenter, 0, 1, -1, true, w_ptr) {}

void Dirt::doSomething() {}

Dirt::~Dirt() {}

bool Dirt::blocks() { return true; } //overrides Actor's blocks()

bool Dirt::allowsOverlap() { return true; } //overrides Actor's allowsOverlap()

//Food
Food::Food(int xFromCenter, int yFromCenter, StudentWorld* w_ptr) : Actor(IID_FOOD, VIEW_WIDTH / 2 + xFromCenter, VIEW_HEIGHT / 2 + yFromCenter, 90, 1, -1, true, w_ptr) {}

void Food::doSomething() {}

bool Food::edible() { return true; }

bool Food::eat() {
    setDead();
    return true;
}

Food::~Food() {}

//Projectile (virtual)
Projectile::Projectile(int xFromCenter, int yFromCenter, Direction dir, int ID, int max, StudentWorld* w_ptr) : Actor(ID, VIEW_WIDTH / 2 + xFromCenter, VIEW_HEIGHT / 2 + yFromCenter, dir, 1, -1, true, w_ptr) {
    m_maxTravel = max;
    m_distTravelled = 0;
}

bool Projectile::damage(int n) { return false; }

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
Bacteria::Bacteria(int x, int y, int ID, StudentWorld* w_ptr, int hp, int movementPlan, int amtDmg) : Actor(ID, x, y, 90, 0, hp, true, w_ptr) {
    m_planDist = movementPlan;
    m_amtDmg = amtDmg;
    m_foodEaten = 0;
    getWorld()->playSound(SOUND_BACTERIUM_BORN);
    getWorld()->incBacteriaCount();
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
            m_foodEaten++;
            break;
        }
    }
    
    doSpecificThing();
}

bool Bacteria::damage(int n) {
    Actor::damage(n);
    if(!isAlive()) {
        getWorld()->decBacteriaCount();
        playDeadSound(); //different for each bacteria
        getWorld()->increaseScore(100);
        if(randInt(0, 1) == 0)
            getWorld()->addFood(getX(), getY());
    } else
        playHurtSound();
    return true;
}

int Bacteria::getPlanDist() { return m_planDist; }

void Bacteria::setPlanDist(int newPlan) { m_planDist = newPlan; }

Bacteria::~Bacteria() {}

//EColi
EColi::EColi(int x, int y, StudentWorld* w_ptr) : Bacteria(x, y, IID_ECOLI, w_ptr, 5, 0, 4) {}

void EColi::doSpecificThing() {
    if (getWorld()->calcDistSocrates(this) <= 256) {
        Direction theta = getWorld()->calcAngleSocrates(this);;
        for (int n = 0; n < 10; n++) {
            if (getWorld()->attemptMove(this, theta, 2)) {
                moveTo(getX() + 2 * cos(theta * M_PI / 180), getY() + 2 * sin(theta * M_PI / 180));
                return;
            }
            theta += 10;
        }
    }
}

void EColi::addBacteria(int x, int y) { getWorld()->addEColi(x, y); }

void EColi::playHurtSound() { getWorld()->playSound(SOUND_ECOLI_HURT); }

void EColi::playDeadSound() { getWorld()->playSound(SOUND_ECOLI_DIE); }

EColi::~EColi() {}

//Salmonella (virtual)
Salmonella::Salmonella(int x, int y, StudentWorld* w_ptr, int hp, int amtDmg) : Bacteria(x, y, IID_SALMONELLA, w_ptr, hp, 0, amtDmg) {}

void Salmonella::doSpecificThing() {
    if(getPlanDist() > 0) {
        setPlanDist(getPlanDist()-1);
        if(getWorld()->attemptMove(this, getDirection(), 3) && isValidCoord(getX() + 3 * cos(getDirection() * M_PI / 180), getY() + 3 * sin(getDirection() * M_PI / 180)))
            moveAngle(getDirection(), 3);
        else {
            setDirection(randInt(0, 359));
            setPlanDist(10);
        }
    } else {
        Direction dir;
        if (getWorld()->findFood(this, 128, dir)) {
            if(getWorld()->attemptMove(this, dir, 3) && isValidCoord(getX() + 3 * cos(dir * M_PI / 180), getY() + 3 * sin(dir * M_PI / 180))) {
                setDirection(dir);
                moveAngle(dir, 3);
            }
            else {
                setDirection(randInt(0, 359));
                setPlanDist(10);
            }
        } else {
            setDirection(randInt(0, 359));
            setPlanDist(10);
        }
    }
}

void Salmonella::playHurtSound() { getWorld()->playSound(SOUND_SALMONELLA_HURT); }

void Salmonella::playDeadSound() { getWorld()->playSound(SOUND_SALMONELLA_DIE); }

Salmonella::~Salmonella() {}

//Regular Salmonella
RegSal::RegSal(int x, int y, StudentWorld* w_ptr) : Salmonella(x, y, w_ptr, 4, 1) {}

void RegSal::addBacteria(int x, int y) { getWorld()->addRegSal(x, y); }

RegSal::~RegSal() {}

//Aggressive Salmonella
AggSal::AggSal(int x, int y, StudentWorld* w_ptr) : Salmonella(x, y, w_ptr, 10, 2){}

void AggSal::doSomething() {
    if(!isAlive()) return;
    
    if(getWorld()->calcDistSocrates(this) <= 72) {
        if(getWorld()->attemptMove(this, getWorld()->calcAngleSocrates(this), 3)) {
            setDirection(getWorld()->calcAngleSocrates(this));
            moveAngle(getDirection(), 3);
        }
        Bacteria::doSomething();
    } else {
        Bacteria::doSomething();
        Salmonella::doSpecificThing();
    }
}

void AggSal::doSpecificThing() {}

void AggSal::addBacteria(int x, int y) { getWorld()->addAggSal(x, y); }

AggSal::~AggSal() {}

//Goodie (virtual)
Goodie::Goodie(int xFromCenter, int yFromCenter, int ID, int awardPts, StudentWorld* w_ptr) : Actor(ID, VIEW_WIDTH / 2 + xFromCenter, VIEW_HEIGHT / 2 + yFromCenter, 0, 1, 0, true, w_ptr){
    m_lifeTime = std::max(rand() % (300 - 10 * getWorld()->getLevel()), 50);
    m_award = awardPts;
}

void Goodie::doSomething() {
    if (!isAlive()) return;
    
    m_lifeTime--;
    if (getWorld()->overlapSocrates(this)) {
        getWorld()->increaseScore(m_award);
        setDead();
        playSound();
        doSpecificThing();
    }
    
    if (m_lifeTime == 0) setDead();
}

void Goodie::playSound() { getWorld()->playSound(SOUND_GOT_GOODIE); }

bool Goodie::damage(int n) { return false; }

Goodie::~Goodie() {}

//Health Goodie
HealthG::HealthG(int xFromCenter, int yFromcenter, StudentWorld* w_ptr) : Goodie(xFromCenter, yFromcenter, IID_RESTORE_HEALTH_GOODIE, 250, w_ptr) {}

void HealthG::doSpecificThing() { getWorld()->restoreSocrates(); }

HealthG::~HealthG() {}

//Flame Goodie
FlameG::FlameG(int xFromCenter, int yFromcenter, StudentWorld* w_ptr) : Goodie(xFromCenter, yFromcenter, IID_FLAME_THROWER_GOODIE, 300, w_ptr) {}

void FlameG::doSpecificThing() { getWorld()->giveFlameToSocrates(5); }

FlameG::~FlameG() {}

//Life Goodie
LifeG::LifeG(int xFromCenter, int yFromcenter, StudentWorld* w_ptr) : Goodie(xFromCenter, yFromcenter, IID_EXTRA_LIFE_GOODIE, 500, w_ptr) {}

void LifeG::doSpecificThing() { getWorld()->incLives(); }

LifeG::~LifeG() {}

//Fungus
Fungus::Fungus(int xFromCenter, int yFromcenter, StudentWorld* w_ptr) : Goodie(xFromCenter, yFromcenter, IID_FUNGUS, -50, w_ptr) {}

void Fungus::doSpecificThing() { getWorld()->damageSocrates(50); }

void Fungus::playSound() {}

Fungus::~Fungus() {}
