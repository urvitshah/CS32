#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

// Actor class, base class for all objects in game
class Actor : public GraphObject
{
public:
    
    Actor(int i, int x, int y, int dir, int d, StudentWorld* sw);
    
    //doSomething() function that is called when the game wants an Actor to take an action. Since an object of Actor is never created, the function is pure virtual and does nothing.
    virtual void doSomething() = 0;
    
    virtual void reduceHitPoints(int n);
    virtual void resetHitPoints();
    virtual void increaseFlame(int n);
    
    void setLife(bool status);
    bool getLife();
    
    virtual bool isDamageable();
    virtual bool isEdible();
    virtual bool isObstacle();
    virtual bool isBacteria();
    
    StudentWorld* getWorld();
    
private:
    bool alive;
    StudentWorld* m_world;

};

//Socrates class
class Socrates : public Actor
{
public:
    Socrates(StudentWorld* sw);
    virtual void doSomething();
    virtual void reduceHitPoints(int n);
    virtual void resetHitPoints();
    virtual void increaseFlame(int n);
    int getHitPoints();
    int getSpray();
    int getFlame();
private:
    int sprayCharge;
    int flameCharge;
    int hitPoints;
};

//Dirt class
class Dirt : public Actor
{
public:
    Dirt(int x, int y, StudentWorld* sw);
    virtual void doSomething();
    bool isDamageable();
    bool isObstacle();
};

//Pit class
class Pit : public Actor
{
public:
    Pit(int x, int y, StudentWorld* sw);
    virtual void doSomething();
private:
    int regSalmonella;
    int aggSalmonella;
    int eColi;
};

//Food class
class Food : public Actor
{
public:
    Food(int x, int y, StudentWorld* sw);
    virtual void doSomething();
    virtual bool isEdible();
};

//Projectile class
class Projectile : public Actor
{
public:
    Projectile(int i, int x, int y, int dir, int d, StudentWorld* sw, int mtd);
    //This function is pure virtual because an object of this class is never created and so the Projectile class never does anything by itself.
    virtual void doSomething() = 0;
    void attack(int n);
    
private:
    int maxTravelDistance;
};

//Flame class
class Flame : public Projectile
{
public:
    Flame(int x, int y, int dir, StudentWorld* sw);
    virtual void doSomething();
private:
    int maxTravelDistance;
};

//Spray class
class Spray : public Projectile
{
public:
    Spray(int x, int y, int dir, StudentWorld* sw);
    virtual void doSomething();
private:
    int maxTravelDistance;
};

//Goodie class
class Goodie : public Actor
{
public:
    Goodie(int i, int x, int y, StudentWorld* sw);
    //This function is pure virtual because an object of this class is never created and so the Goodie class never does anything by itself.
    virtual void doSomething() = 0;
    //This function is pure vritual because every class that is derived from this class implements this function for functionality that differs for each Goodie, but it is never needed by this class.
    virtual void doSomethingSpecific(Actor *a) = 0;
    virtual void doSomethingGeneral(int points);
    int getLifetime();
    virtual bool isDamageable();
    
private:
    int lifetime;
};

//RestoreHealthGoodie class
class RestoreHealthGoodie : public Goodie
{
public:
    RestoreHealthGoodie(int x, int y, StudentWorld* sw);
    virtual void doSomething();
    virtual void doSomethingSpecific(Actor *a);

};

//FlameThrowerGoodie
class FlameThrowerGoodie : public Goodie
{
public:
    FlameThrowerGoodie(int x, int y, StudentWorld* sw);
    virtual void doSomething();
    virtual void doSomethingSpecific(Actor *a);
};

//ExtraLifeGoodie
class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(int x, int y, StudentWorld* sw);
    virtual void doSomething();
    virtual void doSomethingSpecific(Actor *a);
};

//Fungus
class Fungus : public Goodie
{
public:
    Fungus(int x, int y, StudentWorld* sw);
    virtual void doSomething();
    virtual void doSomethingSpecific(Actor *a);
};

//Bacterium class
class Bacterium : public Actor
{
public:
    Bacterium(int hp, int i, int x, int y, StudentWorld* sw);
    //This function is pure virtual because an object of this class is never created and so the Bacterium class never does anything by itself.
    virtual void doSomething() = 0;
    virtual bool isDamageable();
    virtual bool isBacteria();
    virtual void reduceHitPoints(int n);
    int getHitPoints();
    void createNewBacterium(int n);
    void increaseFood(int n);
    int getFood();
    void increaseMovementPlan(int n);
    int getMovementPlan();
    
    void checkOverlap(int hp, int type);
private:
    int hitPoints;
    int movementPlanDistance;
    int foodCount;
};

//EColi class
class EColi : public Bacterium
{
public:
    EColi(int x, int y, StudentWorld* sw);
    virtual void doSomething();
    virtual void reduceHitPoints(int n);
};

//Salmonella class
class Salmonella : public Bacterium
{
public:
    Salmonella(int hp, int i, int x, int y, StudentWorld* sw);
    virtual void doSomething() = 0;
    virtual void reduceHitPoints(int n);
    void runMovementPlan();
};

class RegularSalmonella : public Salmonella
{
public:
    RegularSalmonella(int x, int y, StudentWorld* sw);
    virtual void doSomething();
};

class AggressiveSalmonella : public Salmonella
{
public:
    AggressiveSalmonella(int x, int y, StudentWorld* sw);
    virtual void doSomething();
};
#endif // ACTOR_H_
