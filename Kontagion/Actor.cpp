#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//Helper functions
void polarToRect(double r, Direction theta, double& x, double& y) {
    x = VIEW_WIDTH / 2 + r * cos(theta * M_PI / 180);
    y = VIEW_HEIGHT / 2 + r * sin(theta * M_PI / 180);
}

void rectToPolar(double x, double y, double& r, Direction& theta) {
    r = sqrt(pow((VIEW_WIDTH / 2 - x), 2) + pow((VIEW_HEIGHT / 2 - y), 2));
    theta = atan((VIEW_HEIGHT / 2 - y) / (VIEW_WIDTH / 2 - x)) * 180 / M_PI;
    if (x - VIEW_WIDTH / 2 < 0)
        theta += 180;
    else if (y - VIEW_HEIGHT / 2 < 0)
        theta += 360;
    theta %= 360;
}

void movePolar(Actor* ap, double& r, Direction& theta) {
    double newX, newY, newR;
    Direction newTheta;
    rectToPolar(ap->getX(), ap->getY(), newR, newTheta);
    newR += r;
    newTheta += theta;
    newTheta %= 360;
    polarToRect(newR, newTheta, newX, newY);
    ap->moveTo(newX, newY);
    r = newR;
    theta = newTheta;
}

double calcDistance(Actor* a1, Actor* a2) {
    return sqrt(pow(a1->getX()-a2->getX(), 2)+pow(a1->getY()-a2->getY(), 2));
}

void calcNewXY(double& x, double& y) {
    x += (x < VIEW_WIDTH / 2) ? SPRITE_WIDTH / 2 : -SPRITE_WIDTH / 2;
    y += (y < VIEW_HEIGHT / 2) ? SPRITE_WIDTH / 2 : -SPRITE_WIDTH / 2;
}

bool isValidCoord(double x, double y) { return sqrt(pow(x - VIEW_WIDTH / 2, 2) + pow(y - VIEW_HEIGHT / 2, 2)) < VIEW_RADIUS; }

//Actor
Actor::Actor(int ID, double x, double y, Direction dir, int d, int hp, bool ind, StudentWorld* w_ptr) : GraphObject(ID, x, y, dir, d) {
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
    m_hp -= n;
    if (m_hp <= 0) setDead();
    return true;
}

Actor::~Actor() {}

//Socrates
Socrates::Socrates(StudentWorld* w_ptr) : Actor(IID_PLAYER, 0, VIEW_HEIGHT / 2, 0, 0, 100, true, w_ptr) {
    m_spray = 20;
    m_flame = 5;
}

void Socrates::doSomething() {
    if(!isAlive()) return;
    
    int ch;
    if(getWorld()->getKey(ch)) {
        double rC = 0;
        double xC = 0;
        double yC = 0;
        Direction thetaC = 0;
        switch (ch)
        {
            case KEY_PRESS_SPACE:
                if (m_spray == 0) break;
                xC = getX() + SPRITE_WIDTH * cos(getDirection() * M_PI/180);
                yC = getY() + SPRITE_WIDTH * sin(getDirection() * M_PI/180);
                getWorld()->addSpray(xC, yC, getDirection());
                m_spray--;
                getWorld()->playSound(SOUND_PLAYER_SPRAY);
                break;
            case KEY_PRESS_ENTER:
                if (m_flame == 0) break;
                for (int i = 0; i < 16; i++) {
                    thetaC = (getDirection() + 22 * i) % 360;
                    xC = getX() + SPRITE_WIDTH * cos(thetaC * M_PI/180);
                    yC = getY() + SPRITE_WIDTH * sin(thetaC * M_PI/180);
                    getWorld()->addFlame(xC, yC, thetaC);
                }
                m_flame--;
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                break;
            case KEY_PRESS_LEFT:
                thetaC = 5;
                movePolar(this, rC, thetaC);
                setDirection(thetaC + 180);
                break;
            case KEY_PRESS_RIGHT:
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

bool Socrates::damage(int n) {
    Actor::damage(n);
    if (isAlive()) getWorld()->playSound(SOUND_PLAYER_HURT);
    else getWorld()->playSound(SOUND_PLAYER_DIE);
    return true;
}

Socrates::~Socrates() {}

//Dirt
Dirt::Dirt(double xFromCenter, double yFromCenter, StudentWorld* w_ptr) : Actor(IID_DIRT, VIEW_WIDTH / 2 + xFromCenter, VIEW_HEIGHT / 2 + yFromCenter, 0, 1, -1, true, w_ptr) {}

void Dirt::doSomething() {}

Dirt::~Dirt() {}

bool Dirt::blocks() { return true; } //overrides Actor's blocks()

bool Dirt::allowsOverlap() { return true; } //overrides Actor's allowsOverlap()

//Food
Food::Food(double xFromCenter, double yFromCenter, StudentWorld* w_ptr) : Actor(IID_FOOD, VIEW_WIDTH / 2 + xFromCenter, VIEW_HEIGHT / 2 + yFromCenter, 90, 1, -1, true, w_ptr) {}

void Food::doSomething() {}

bool Food::edible() { return true; }

bool Food::eat() {
    setDead();
    return true;
}

bool Food::damage(int n) { return false; }

Food::~Food() {}

//Projectile (virtual)
Projectile::Projectile(double x, double y, Direction dir, int ID, int max, int dmg,  StudentWorld* w_ptr) : Actor(ID, x, y, dir, 1, -1, true, w_ptr) {
    m_maxTravel = max;
    m_distTravelled = 0;
    m_amtDmg = dmg;
}

void Projectile::doSomething() {
    if (!isAlive()) return;
    
    if(getWorld()->damageDamageable(this, m_amtDmg)) {
        setDead();
        return;
    }
    
    moveAngle(getDirection(), SPRITE_WIDTH);
    addDist(SPRITE_WIDTH);
    
    if(getDistTravelled() >= getMaxTravel()) {
        setDead();
    }
}

bool Projectile::damage(int n) { return false; }

int Projectile::getMaxTravel() { return m_maxTravel; }

int Projectile::getDistTravelled() { return m_distTravelled; }

void Projectile::addDist(int n) { m_distTravelled += n; }

Projectile::~Projectile() {}

//Flame Projectile
FlameProj::FlameProj(double x, double y, Direction dir, StudentWorld* w_ptr) : Projectile(x, y, dir, IID_FLAME, 32, 5, w_ptr) {}

FlameProj::~FlameProj() {}

//Spray Projectile

SprayProj::SprayProj(double x, double y, Direction dir, StudentWorld* w_ptr) : Projectile(x, y, dir, IID_SPRAY, 112, 2, w_ptr) {}

SprayProj::~SprayProj() {}

//Bacteria (virtual)
Bacteria::Bacteria(double x, double y, int ID, StudentWorld* w_ptr, int hp, int movementPlan, int amtDmg) : Actor(ID, x, y, 90, 0, hp, true, w_ptr) {
    m_planDist = movementPlan;
    m_amtDmg = amtDmg;
    m_foodEaten = 0;
    getWorld()->playSound(SOUND_BACTERIUM_BORN);
    getWorld()->incBacteriaCount();
}

void Bacteria::doSomething() {
    if (!isAlive()) return;
    if (getWorld()->overlapSocrates(this))
        getWorld()->damageSocrates(m_amtDmg);
    else if (m_foodEaten == 3) {
        double x = getX();
        double y = getY();
        calcNewXY(x, y);
        addBacteria(x, y);
        m_foodEaten = 0;
    }
    else if (getWorld()->attemptEat(this))
        m_foodEaten++;
    doSpecificThing();
}

bool Bacteria::damage(int n) {
    Actor::damage(n);
    if(!isAlive()) {
        getWorld()->increaseScore(100);
        playDeadSound();
        if(randInt(0, 1) == 0)
            getWorld()->addFood(getX(), getY());
    } else
        playHurtSound();
    return true;
}

int Bacteria::getPlanDist() { return m_planDist; }

void Bacteria::setPlanDist(int newPlan) { m_planDist = newPlan; }

Bacteria::~Bacteria() { getWorld()->decBacteriaCount(); } //different for each bacteria

//EColi
EColi::EColi(double x, double y, StudentWorld* w_ptr) : Bacteria(x, y, IID_ECOLI, w_ptr, 5, 0, 4) {}

void EColi::doSpecificThing() {
    if (getWorld()->calcDistSocrates(this) <= 256) {
        Direction theta = getWorld()->calcAngleSocrates(this);;
        for (int n = 0; n < 10; n++) {
            if (getWorld()->attemptMove(this, 2, theta)) {
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
Salmonella::Salmonella(double x, double y, StudentWorld* w_ptr, int hp, int amtDmg) : Bacteria(x, y, IID_SALMONELLA, w_ptr, hp, 0, amtDmg) {}

void Salmonella::doSpecificThing() {
    if(getPlanDist() > 0) {
        setPlanDist(getPlanDist()-1);
        if(getWorld()->attemptMove(this, 3, getDirection()) && isValidCoord(getX() + 3 * cos(getDirection() * M_PI / 180), getY() + 3 * sin(getDirection() * M_PI / 180)))
            moveAngle(getDirection(), 3);
        else {
            setDirection(randInt(0, 359));
            setPlanDist(10);
        }
    } else {
        Direction dir;
        if (getWorld()->findFood(this, 128, dir)) {
            if(getWorld()->attemptMove(this, 3, dir) && isValidCoord(getX() + 3 * cos(dir * M_PI / 180), getY() + 3 * sin(dir * M_PI / 180))) {
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
RegSal::RegSal(double x, double y, StudentWorld* w_ptr) : Salmonella(x, y, w_ptr, 4, 1) {}

void RegSal::addBacteria(int x, int y) { getWorld()->addRegSal(x, y); }

RegSal::~RegSal() {}

//Aggressive Salmonella
AggSal::AggSal(double x, double y, StudentWorld* w_ptr) : Salmonella(x, y, w_ptr, 10, 2){}

void AggSal::doSomething() {
    if(!isAlive()) return;
    
    if(getWorld()->calcDistSocrates(this) <= 72) {
        if(getWorld()->attemptMove(this, 3, getWorld()->calcAngleSocrates(this))) {
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
Goodie::Goodie(double xFromCenter, double yFromCenter, int ID, int awardPts, StudentWorld* w_ptr) : Actor(ID, VIEW_WIDTH / 2 + xFromCenter, VIEW_HEIGHT / 2 + yFromCenter, 0, 1, -1, true, w_ptr){
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
HealthG::HealthG(double xFromCenter, double yFromCenter, StudentWorld* w_ptr) : Goodie(xFromCenter, yFromCenter, IID_RESTORE_HEALTH_GOODIE, 250, w_ptr) {}

void HealthG::doSpecificThing() { getWorld()->restoreSocrates(); }

HealthG::~HealthG() {}

//Flame Goodie
FlameG::FlameG(double xFromCenter, double yFromCenter, StudentWorld* w_ptr) : Goodie(xFromCenter, yFromCenter, IID_FLAME_THROWER_GOODIE, 300, w_ptr) {}

void FlameG::doSpecificThing() { getWorld()->giveFlameToSocrates(5); }

FlameG::~FlameG() {}

//Life Goodie
LifeG::LifeG(double xFromCenter, double yFromCenter, StudentWorld* w_ptr) : Goodie(xFromCenter, yFromCenter, IID_EXTRA_LIFE_GOODIE, 500, w_ptr) {}

void LifeG::doSpecificThing() { getWorld()->incLives(); }

LifeG::~LifeG() {}

//Fungus
Fungus::Fungus(double xFromCenter, double yFromCenter, StudentWorld* w_ptr) : Goodie(xFromCenter, yFromCenter, IID_FUNGUS, -50, w_ptr) {}

void Fungus::doSpecificThing() { getWorld()->damageSocrates(50); }

void Fungus::playSound() {}

bool Fungus::damage(int n) {
    Actor::damage(n);
    return true;
}

Fungus::~Fungus() {}

//Pit
Pit::Pit(double xFromCenter, double yFromCenter, StudentWorld* w_ptr) : Actor(IID_PIT, VIEW_WIDTH / 2 + xFromCenter, VIEW_HEIGHT / 2 + yFromCenter, 0, 1, -1, true, w_ptr) {
    m_nRegSal = 5;
    m_nAggSal = 3;
    m_nEColi = 2;
}

void Pit::doSomething() {
    if (m_nAggSal == 0 && m_nRegSal == 0 && m_nEColi == 0) {
        setDead();
        return;
    }
    
    if (randInt(0, 49) == 0) {
        if (m_nRegSal != 0 && m_nAggSal == 0 && m_nEColi == 0) {
            getWorld()->addRegSal(getX(), getY());
            m_nRegSal--;
        } else if (m_nRegSal == 0 && m_nAggSal != 0 && m_nEColi == 0) {
            getWorld()->addAggSal(getX(), getY());
            m_nAggSal--;
        } else if (m_nRegSal == 0 && m_nAggSal == 0 && m_nEColi != 0) {
            getWorld()->addEColi(getX(), getY());
            m_nEColi--;
        } else if (m_nRegSal == 0 && m_nAggSal != 0 && m_nEColi != 0) {
            if (randInt(0,1) == 0) {
                getWorld()->addAggSal(getX(), getY());
                m_nAggSal--;
            } else {
                getWorld()->addEColi(getX(), getY());
                m_nEColi--;
            }
        } else if (m_nRegSal != 0 && m_nAggSal == 0 && m_nEColi != 0) {
            if (randInt(0,1) == 0) {
                getWorld()->addRegSal(getX(), getY());
                m_nRegSal--;
            } else {
                getWorld()->addEColi(getX(), getY());
                m_nEColi--;
            }
        } else if (m_nRegSal != 0 && m_nAggSal != 0 && m_nEColi == 0) {
            if (randInt(0,1) == 0) {
                getWorld()->addRegSal(getX(), getY());
                m_nRegSal--;
            } else {
                getWorld()->addAggSal(getX(), getY());
                m_nAggSal--;
            }
        } else {
            int rand = randInt(0, 2);
            switch (rand) {
                case 0: {
                    getWorld()->addRegSal(getX(), getY());
                    m_nRegSal--;
                    break;
                }
                case 1: {
                    getWorld()->addAggSal(getX(), getY());
                    m_nAggSal--;
                    break;
                }
                case 2: {
                    getWorld()->addEColi(getX(), getY());
                    m_nEColi--;
                    break;
                }
            }
        }
    }
}

bool Pit::damage(int n) { return false; }

Pit::~Pit() { getWorld()->decPitCount(); }
