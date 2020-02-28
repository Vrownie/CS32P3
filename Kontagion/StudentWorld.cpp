#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

void getValidCoords(list<Actor*> la, double& x, double& y) {
    bool overlapInd;
    do {
        overlapInd = false;
        randPolar(120, x, y);
        for(list<Actor*>::iterator i = la.begin(); i != la.end(); i++) {
            if (!(*i)->allowsOverlap() && overlap((*i)->getX(), (*i)->getY(), x + VIEW_WIDTH / 2, y + VIEW_HEIGHT / 2)) { //needs conversion
                overlapInd = true;
            }
        }
    }
    while (overlapInd);
}

bool overlap(const Actor* a1, const Actor* a2) {
    double dist = sqrt(pow(a1->getX() - a2->getX(), 2) + pow(a1->getY() - a2->getY(), 2));
    return dist <= SPRITE_WIDTH;
}

bool overlap(double x1, double y1, double x2, double y2) {
    double dist = sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
    return dist <= SPRITE_WIDTH;
}

bool movementOverlap(const Bacteria* b, const Actor* a) {
    double dist = sqrt(pow(b->getX() - a->getX(), 2) + pow(b->getY() - a->getY(), 2));
    return dist <= SPRITE_WIDTH / 2;
}

bool movementOverlap(double x, double y, const Actor* a) {
    double dist = sqrt(pow(x - a->getX(), 2) + pow(y - a->getY(), 2));
    return dist <= SPRITE_WIDTH / 2;
}

void randPolar(double r, double& x, double& y) {
    x = randInt(-r, r);
    y = randInt(-sqrt(pow(r, 2) - pow(x, 2)), sqrt(pow(r, 2) - pow(x, 2)));
}

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath) {}

int StudentWorld::init()
{
    m_player = new Socrates(this);
    m_nPit = getLevel();
    m_nFood = min(getLevel() * 5, 25);
    m_nDirt = max(180 - getLevel() * 20, 20);
    m_nBacteria = 0;
    m_chanceFungus = max(510 - getLevel() * 10, 200);
    m_chanceGoodie = max(510 - getLevel() * 10, 250);
    
    double x, y;
    
    for (int n = 0; n < m_nPit; n++) {
        getValidCoords(m_list, x, y);
        m_list.push_back(new Pit(x, y, this));
    }

    for (int n = 0; n < m_nFood; n++) {
        getValidCoords(m_list, x, y);
        m_list.push_back(new Food(x, y, this));
    }

    for (int n = 0; n < m_nDirt; n++) {
        getValidCoords(m_list, x, y);
        m_list.push_back(new Dirt(x, y, this));
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_player->doSomething();
    for (list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); i++) {
        if((*i)->isAlive())
            (*i)->doSomething();
        if(!m_player->isAlive()) {
            decLives();
            playSound(SOUND_PLAYER_DIE);
            return GWSTATUS_PLAYER_DIED;
        }
        if(m_nBacteria == 0 && m_nPit == 0) {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    for (list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); ) {
        if(*i != nullptr && !(*i)->isAlive()) {
            delete *i;
            i = m_list.erase(i);
        } else
            i++;
    }
    
    ostringstream oss;
    
    oss << "Score: ";
    if (getScore() < 0)
        oss << "-" << setfill('0') << setw(5) << -getScore();
    else
        oss << setfill('0') << setw(6) << getScore();
        
    oss << setw(0) << "  Level: " << getLevel() << "  Lives: " << getLives() << "  Health: " << m_player->getHP() << "  Sprays: " << m_player->getSpray() << "  Flames: " << m_player->getFlame();
    
    setGameStatText(oss.str());
    
    int r1 = randInt(0, m_chanceFungus - 1);
    if (r1 == 0) {
        int rTheta = randInt(0, 359);
        m_list.push_back(new Fungus(VIEW_RADIUS * cos(rTheta), VIEW_RADIUS * sin(rTheta), this));
        return GWSTATUS_CONTINUE_GAME;
    }
    int r2 = randInt(0, m_chanceGoodie - 2);
    if (r2 == 0) {
        int rTheta = randInt(0, 359);
        int a = randInt(0, 9);
        switch (a) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    m_list.push_back(new HealthG(VIEW_RADIUS * cos(rTheta), VIEW_RADIUS * sin(rTheta), this));
                    break;
                case 6:
                case 7:
                case 8:
                    m_list.push_back(new FlameG(VIEW_RADIUS * cos(rTheta), VIEW_RADIUS * sin(rTheta), this));
                    break;
                case 9:
                    m_list.push_back(new LifeG(VIEW_RADIUS * cos(rTheta), VIEW_RADIUS * sin(rTheta), this));
                    break;
        }
        return GWSTATUS_CONTINUE_GAME;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); ) {
        if (*i != nullptr) {
            delete *i;
            *i = nullptr;
        }
        i = m_list.erase(i);
    }
    
    if (m_player != nullptr) {
        delete m_player;
        m_player = nullptr;
    }
}

bool StudentWorld::damageDamageable(Actor* ap, int n) {
    for (list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); i++) {
        if (overlap(ap, *i))
            if ((*i)->damage(n)) return true;
    }
    return false;
}

void StudentWorld::addFlame(double x, double y, Direction dir) { m_list.push_back(new FlameProj(x, y, dir, this)); }

void StudentWorld::addSpray(double x, double y, Direction dir) { m_list.push_back(new SprayProj(x, y, dir, this)); }

void StudentWorld::addRegSal(double x, double y) { m_list.push_back(new RegSal(x, y, this)); }

void StudentWorld::addAggSal(double x, double y) { m_list.push_back(new AggSal(x, y, this)); }

void StudentWorld::addEColi(double x, double y) { m_list.push_back(new EColi(x, y, this)); }

void StudentWorld::addFood(double x, double y) { m_list.push_back(new Food(x - VIEW_WIDTH / 2, y - VIEW_HEIGHT / 2, this)); } //converts to center coordinates

void StudentWorld::restoreSocrates() { m_player->restoreHP(); }

void StudentWorld::damageSocrates(int n) { m_player->damage(n); }

void StudentWorld::giveFlameToSocrates(int n) { m_player->addFlame(n); }

bool StudentWorld::overlapSocrates(Actor* ap) { return overlap(ap, m_player); }

double StudentWorld::calcDistSocrates(Actor* ap) { return calcDistance(ap, m_player); }

Direction StudentWorld::calcAngleSocrates(Actor* ap) {
    Direction theta = atan((m_player->getY() - ap->getY()) / (m_player->getX() - ap->getX())) * 180 / M_PI;
    if (m_player->getX() - ap->getX() < 0)
        theta += 180;
    else if (m_player->getY() - ap->getY() < 0)
        theta += 360;
    return theta;
}

bool StudentWorld::findFood(Actor* ap, double dist, Direction& results) {
    int currentDist = 999;
    int minDist = 999;
    for(list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); i++) {
        if((*i)->edible() && calcDistance(*i, ap) <= dist) {
            currentDist = calcDistance(*i, ap);
            if(currentDist < minDist) {
                minDist = currentDist;
                results = atan(((*i)->getY() - ap->getY()) / ((*i)->getX() - ap->getX())) * 180 / M_PI;
                if ((*i)->getX() - ap->getX() < 0)
                    results += 180;
                else if ((*i)->getY() - ap->getY() < 0)
                    results += 360;
            }
        }
    }
    return (minDist != -1);
}

bool StudentWorld::attemptEat(Actor* ap) {
    for(list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); i++) {
        if (overlap(ap, *i) && (*i)->eat())
            return true;
    }
    return false;
}

bool StudentWorld::attemptMove(Bacteria* bp, double dist, Direction dir) {
    for(list<Actor*>::iterator i = m_list.begin(); i != m_list.end(); i++) {
        if ((*i)->blocks() && movementOverlap(bp->getX() + dist * cos(dir * M_PI / 180), bp->getY() + dist * sin(dir * M_PI / 180), *i))
            return false;
    }
    return true;
}

void StudentWorld::incBacteriaCount() { m_nBacteria++; }

void StudentWorld::decBacteriaCount() { m_nBacteria--; }

void StudentWorld::decPitCount() { m_nPit--; }

StudentWorld::~StudentWorld() { cleanUp(); }
