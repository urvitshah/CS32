#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
using namespace std;
class Socrates;
class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    StudentWorld* getWorld();
    Actor* getDamageableObject(int x, int y);
    Actor* getSocrates(int x, int y, int d);
    Actor* findNearestFood(int x, int y);
    
    void addSpray(int x, int y, int d);
    void addFlame(int x, int y, int d);
    void addBacterium(int x, int y, int type);
    void addFood(int x, int y);
    
    bool foodOverlap(int x, int y);
    bool canMove(int x, int y);
    
    ~StudentWorld();

private:
    Socrates *m_player;
    vector<Actor*> m_actors;
    int numBacteria;
};

#endif // STUDENTWORLD_H_
