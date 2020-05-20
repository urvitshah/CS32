#include "Actor.h"
#include "StudentWorld.h"

//Actor class

//Constructs Actor
Actor::Actor(int i, int x, int y, int dir, int d, StudentWorld* sw)
: GraphObject(i,x, y, dir, d)
{
    alive = true;
    m_world = sw;
}

//Returns a pointer to the current StudentWorld
StudentWorld* Actor::getWorld()
{
    return m_world;
}

//If an object does not have any hitPoints and another class tries to reduce its hitPoints, it should set the life to false. This function is virtual since any class with hitPoints needs to redefine the function.
void Actor::reduceHitPoints(int n)
{
    alive = false;
}

//These functions are both in the Socrates class, but when a Socrates is passed as an Actor pointer, these functions are necessary. However, they do nothing in the Actor class and are only redefined in the Socrates class.
void Actor::resetHitPoints(){};
void Actor::increaseFlame(int n){};

//Sets Actor to alive or dead based on parameter
void Actor::setLife(bool status)
{
    alive = status;
}

//Returns true if Actor is alive and false if Actor is dead.
bool Actor::getLife()
{
    return alive;
}

//Returns true if Actor can be damaged by a Spray
bool Actor::isDamageable()
{
    return false;
}

//Returns true if Actor is a Food object
bool Actor::isEdible()
{
    return false;
}

//Returns true if Actor blocks movement of an object of the Bacterium class
bool Actor::isObstacle()
{
    return false;
}

//Returns true if Actor is derived from the Bacterium class
bool Actor::isBacteria()
{
    return false;
}

//Socrates class

//Socrates constructor: Creates an object of Socrates and constructs the base Actor class using the image ID for Socrates along with it's intial position and direction. Initializes number of charges that Socrates has and its health.
Socrates::Socrates(StudentWorld* sw)
: Actor(IID_PLAYER, 0, VIEW_HEIGHT/2, 0, 0, sw)
{
    sprayCharge = 20;
    flameCharge = 5;
    hitPoints = 100;
}

//Returns Socrates' health. This is not virtual because no other class is derived from Socrates.
int Socrates::getHitPoints()
{
    return hitPoints;
};

//Returns number of Sprays that Socrates has left. This is not virtual because no other class is derived from Socrates.
int Socrates::getSpray()
{
    return sprayCharge;
}

//Returns number of Flames that Socrates has left. This is not virtual because no other class is derived from Socrates.
int Socrates::getFlame()
{
    return flameCharge;
}

//Socrates takes its next action during the tick. Changes position or fires charges based on the key that is entered. This function is virtual for clarity for the user to know that it is defining Actor's doSomething() function.
void Socrates::doSomething()
{
    int val;
    double currentAngle;
    double newX, newY;
    const double PI = 4 * atan(1);
    
    //Does not continue with the function if Socrates' health has run out
    if (hitPoints <= 0)
    {
        setLife(false);
        getWorld()->playSound(SOUND_PLAYER_DIE);
        return;
    }
    //Takes action based on key that user entered
    if (getWorld()->getKey(val))
    {
        switch(val)
        {
            //If user entered a space, it creates a spray in front of Socrates
            case(KEY_PRESS_SPACE):
                if (sprayCharge > 0)
                {
                    sprayCharge--;
                    getPositionInThisDirection(getDirection(), 2*SPRITE_RADIUS, newX, newY);
                    getWorld()->addSpray(newX, newY, getDirection());
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                }
                break;
            //If user entered enter, it creates flames around Socrates
            case(KEY_PRESS_ENTER):
                if (flameCharge > 0)
                {
                    flameCharge--;
                    for (int a = 0; a < 8; a++)
                    {
                        getPositionInThisDirection(getDirection() + 22*a, 2*SPRITE_RADIUS, newX, newY);
                        getWorld()->addFlame(newX, newY, getDirection() + 22*a);
                    }
                    for (int a = 1; a <= 8; a++)
                    {
                        getPositionInThisDirection(getDirection() - 22*a, 2*SPRITE_RADIUS, newX, newY);
                        getWorld()->addFlame(newX, newY, getDirection() - 22*a);
                    }
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                break;
            //If the user presses left, Socrates moves counterclockwise
            case(KEY_PRESS_LEFT):
                currentAngle = atan2(getY()-VIEW_HEIGHT/2,getX()-VIEW_WIDTH/2);
                newX = VIEW_WIDTH/2*cos(currentAngle+(5*PI/180));
                newY = VIEW_HEIGHT/2*sin(currentAngle+(5*PI/180));
                moveTo(newX + VIEW_WIDTH/2, newY + VIEW_HEIGHT/2);
                setDirection(getDirection()+5);
                break;
            //If the user presses right, Socrates moves clockwise
            case(KEY_PRESS_RIGHT):
                currentAngle = atan2(getY()-VIEW_HEIGHT/2,getX()-VIEW_WIDTH/2);
                newX = VIEW_WIDTH/2*cos(currentAngle-(5*PI/180));
                newY = VIEW_HEIGHT/2*sin(currentAngle-(5*PI/180));
                moveTo(newX + VIEW_WIDTH/2, newY + VIEW_HEIGHT/2);
                setDirection(getDirection()-5);
                break;
        }
    }
    else
    {
        //if the user has not entered a key and a tick has passed, then the spray charge is refilled as needed
        if (sprayCharge < 20)
        {
            sprayCharge++;
        }
    }
}

//Redefines Actor's reduceHitPoints function to reduce Socrates' health and play the correct sounds if Socrates gets hurt or dies. This function is virtual for clarity for the user to know that it is defining Actor's reduceHitPoints function.
void Socrates::reduceHitPoints(int n)
{
    hitPoints -= n;
    getWorld()->playSound(SOUND_PLAYER_HURT);
    if (hitPoints <= 0)
    {
        setLife(false);
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
}

//Makes Socrates' health full again. This function is virtual for clarity for the user to know that it is defining Actor's resetHitPoints function.
void Socrates::resetHitPoints()
{
    hitPoints = 100;
}

//Increases number of flames that Socrates has. This function is virtual for clarity for the user to know that it is defining Actor's increaseFlame function.
void Socrates::increaseFlame(int n)
{
    flameCharge += n;
}

//Dirt class

//Dirt constructor: Creates an object of Dirt and constructs the base Actor class using the image ID for Dirt along with it's intial position and direction.
Dirt::Dirt(int x, int y, StudentWorld* sw)
:Actor(IID_DIRT, x, y, 0, 1, sw)
{
}

//Redefinition of Actor's doSomething function that is virtual for clarity. Does nothing because Dirt does not do anything.
void Dirt::doSomething()
{
}

//Returns true for Dirt because it can be destroyed through sprays or charges. Virtual for clarity since it is a redefinition of Actor's function.
bool Dirt::isDamageable()
{   return true;    }

//Returns true for Dirt because it blocks Bacteria movement. Virtual for clarity since it is a redefinition of Actor's function.
bool Dirt::isObstacle()
{
    return true;
}

//Pit class

//Pit constructor: Creates an object of Pit and constructs the base Actor class using the image ID for Pit along with it's intial position and direction. Initializes number of bacteria that the pit holds.
Pit::Pit(int x, int y, StudentWorld* sw)
: Actor (IID_PIT, x, y, 0, 1, sw)
{
    regSalmonella = 5;
    aggSalmonella = 3;
    eColi = 2;
}

//Pit's doSomething function that is virtual for clarity since it is redefining Actor's function. Releases bacteria into the game with a 1/50 chance.
void Pit::doSomething()
{
    //Removes pit if all the bacteria have left it.
    if (regSalmonella == 0 && aggSalmonella == 0 && eColi == 0)
    {
        setLife(false);
        return;
    }
    int chance = randInt(1,50);
    bool exit = false;
    while (chance == 1 && !exit)
    {
        //Chooses a random number to decide which bacteria is released
        int type = randInt(1,3);
        if (type == 1 && regSalmonella > 0)
        {
            getWorld()->addBacterium(getX(),getY(),1);
            regSalmonella--;
            exit = true;
        }
        if (type == 1 && aggSalmonella > 0)
        {
            getWorld()->addBacterium(getX(),getY(),2);
            aggSalmonella--;
            exit = true;
        }
        if (type == 3 && eColi > 0)
        {
            getWorld()->addBacterium(getX(),getY(),3);
            eColi--;
            exit = true;
        }
    }
}

//Food class
//Food constructor: Creates an object of Food and constructs the base Actor class using the image ID for Food along with it's initial position and direction.
Food::Food(int x, int y, StudentWorld* sw)
: Actor(IID_FOOD, x, y, 90, 1, sw)
{}

//This function is virtual for clarity because Actor's doSomething function is virtual and can be redefined. This function does nothing because Food doesn't do anything.
void Food::doSomething()
{}

//Redefinition of Actor's function that is virtual for clarity. Returns true because Food objects can be eaten.
bool Food::isEdible()
{
    return true;
}

//Projectile class
//Projectile constructor: Creates an object of a Projectile and constructs the base Actor class using the image ID for Projectile along with it's initial position and direction. Initializes maximum travel distance for every projectile.
Projectile::Projectile(int i, int x, int y, int dir, int d, StudentWorld* sw, int mtd)
: Actor (i, x, y, dir, d, sw)
{
    maxTravelDistance = mtd;
}

//This functionality is common for both the Spray and Flame classes. This function checks if the Projectile object overlaps with a Dirt object or Bacterium object and kills it if it exists. Moves forward until hitting the maximum distance it can travel if it does not hit any other objects. This function is not virtual because no other class redefines it.
void Projectile::attack(int hp)
{
    if (!getLife())
        return;
    //gets the object that it overlaps with, returns nullptr if there are none
    Actor *a = getWorld()->getDamageableObject(getX(), getY());
    if (a != nullptr)
    {
        //damages the object that it hit
        a->reduceHitPoints(hp);
        setLife(false);
        return;
    }
    else
    {
        //if there is no object, it continues moving forward
        moveForward(SPRITE_RADIUS*2);
        maxTravelDistance -= SPRITE_RADIUS*2;
    }
    
    if (maxTravelDistance <= 0)
        setLife(false);
}

//Flame class
//Flame constructor: Creates an object of a Flame and constructs the base Actor class using the image ID for Flame along with it's initial position and direction.
Flame::Flame(int x, int y, int dir, StudentWorld* sw)
:Projectile(IID_FLAME, x, y, dir, 1, sw, 32)
{ }

//Flame's doSomething function that is virtual for clarity because it is redefining Actor's virtual doSomething function. This function calls the attack function in its Base class which damages an object that it hits by the number that is passed in, moves the object forward, or kills the Flame.
void Flame::doSomething()
{
    attack(5);
}

//Spray class
//Spray constructor: Creates an object of a Spray and constructs the base Actor class using the image ID for Spray along with it's initial position and direction.
Spray::Spray(int x, int y, int dir, StudentWorld* sw)
: Projectile(IID_SPRAY, x, y, dir, 1, sw, 112)
{}

//Flame's doSomething function that is virtual for clarity because it is redefining Actor's virtual doSomething function. This function calls the attack function in its Base class which damages an object that it hits by the number that is passed in, moves the object forward, or kills the Flame.
void Spray::doSomething()
{
    attack(2);
}

//Goodie class
//Goodie constructor: Creates an object of a Goodie and constructs the base Actor class using the image ID for Goodie along with it's initial position and direction. Initializes lifetime with lifetime of Goodies.
Goodie::Goodie(int i, int x, int y, StudentWorld* sw)
: Actor(i, x, y,0,1, sw)
{
    lifetime = max(rand() % (300 - 10 * sw->getLevel()), 50) ;
}

//Returns the lifetime of the Goodie. This function is not virtual because it is never redefined.
int Goodie::getLifetime()
{
    return lifetime;
}

//This function returns true because Goodies can be damaged by Projectile objects and it is virtual for clarity since it is a redefinition of Actor's function.
bool Goodie::isDamageable()
{
    return true;
}

//This function does functionality that is common to all Goodies. It increases the player's score by the parameter that is passed in. It determines if Socrates is overlapping the object and does an action according to the type of Goodie and kills the Goodie if its' lifetime is over. This function is not virtual because it is never redefined.
void Goodie::doSomethingGeneral(int points)
{
    if(!getLife())
        return;
    lifetime--;
    //Gets Socrates if the objects overlap
    Actor *a = getWorld()->getSocrates(getX(),getY(), SPRITE_RADIUS*2);
    if (a!=nullptr)
    {
        //Increases total score and calls function to do something specific to the type of goodie
        getWorld()->increaseScore(points);
        setLife(false);
        if (points > 0)
            getWorld()->playSound(SOUND_GOT_GOODIE);
        doSomethingSpecific(a);
    }
    if (lifetime <= 0)
        setLife(false);
}

//RestoreHealthGoodie class
//RestoreHealthGoodie constructor: Creates an object of a RestoreHealthGoodie and constructs the base Actor class using the image ID for RestoreHealthGoodie along with it's initial position and direction.
RestoreHealthGoodie::RestoreHealthGoodie(int x, int y, StudentWorld* sw)
: Goodie(IID_RESTORE_HEALTH_GOODIE, x, y, sw)
{
}

//Calls the base class doSomethingGeneral function with the correct number of points that the player gets if it overlaps with the Goodie.
void RestoreHealthGoodie::doSomething()
{
    doSomethingGeneral(250);
}

//Resets health points for Socrates since this is what the RestoreHealthGoodie should do when it is picked up.
void RestoreHealthGoodie::doSomethingSpecific(Actor *a)
{
    a->resetHitPoints();
}

//FlameThrowerGoodie class
//FlameThrowerGoodie constructor: Creates an object of a FlameThrowerGoodie and constructs the base Actor class using the image ID for FlameThrowerGoodie along with it's initial position and direction.
FlameThrowerGoodie::FlameThrowerGoodie(int x, int y, StudentWorld* sw)
: Goodie(IID_FLAME_THROWER_GOODIE, x, y, sw)
{
}

//Calls the base class doSomethingGeneral function with the correct number of points that the player gets if it overlaps with the Goodie.
void FlameThrowerGoodie::doSomething()
{
    doSomethingGeneral(300);
}

//Increase's Socrates' Flame charge count by five since this is what the FlameThrowerGoodie should do when it is picked up.
void FlameThrowerGoodie::doSomethingSpecific(Actor *a)
{
    a->increaseFlame(5);
}

//ExtraLifeGoodie class
//ExtraLifeGoodie constructor: Creates an object of a ExtraLifeGoodie and constructs the base Actor class using the image ID for ExtraLifeGoodie along with it's initial position and direction.
ExtraLifeGoodie::ExtraLifeGoodie(int x, int y, StudentWorld* sw)
: Goodie(IID_EXTRA_LIFE_GOODIE, x, y, sw)
{
}

//Calls the base class doSomethingGeneral function with the correct number of points that the player gets if it overlaps with the Goodie.
void ExtraLifeGoodie::doSomething()
{
    doSomethingGeneral(500);
}

//Increases the number of lives that Socrates has since this is what the ExtraLifeGoodie should do when it is picked up.
void ExtraLifeGoodie::doSomethingSpecific(Actor *a)
{
    getWorld()->incLives();
}

//Fungus class
//Fungus constructor: Creates an object of a Fungus and constructs the base Actor class using the image ID for Fungus along with it's initial position and direction.
Fungus::Fungus(int x, int y, StudentWorld* sw)
: Goodie(IID_FUNGUS, x, y, sw)
{
}

//Calls the base class doSomethingGeneral function with the correct number of points that the player gets if it overlaps with the Goodie.
void Fungus::doSomething()
{
    doSomethingGeneral(-50);
}

//Decreases Socrates' health by 20 since this is what the Fungus should do when it is picked up.
void Fungus::doSomethingSpecific(Actor *a)
{
    a->reduceHitPoints(20);
}

//Bacterium class
//Bacterium constructor: Creates an object of a Bacterium and constructs the base Actor class using the image ID for Bacterium along with it's initial position and direction. Initializes a Bacterium object's health, movement plan, and number of food eaten.
Bacterium::Bacterium(int i, int hp, int x, int y, StudentWorld* sw)
: Actor(i, x, y, 90, 0, sw)
{
    hitPoints = hp;
    movementPlanDistance = 0;
    foodCount = 0;
}

//This function returns true because Bacterium objects can be damaged by Projectile objects and it is virtual for clarity since it is a redefinition of Actor's function.
bool Bacterium::isDamageable()
{
    return true;
}

//This function returns true because Bacterium objects are bacteria and it is virtual for clarity since it is a redefinition of Actor's function.
bool Bacterium::isBacteria()
{
    return true;
}

//This function reduces the health of the bacteria by the number that is passed in and it is virtual for clarity since it is a redefinition of Actor's function.
void Bacterium::reduceHitPoints(int n)
{
    hitPoints -= n;
}

//This returns a Bacterium object's health and it is not virtual because it is not redefined.
int Bacterium::getHitPoints()
{
    return hitPoints;
}

//This function creates a new Bacterium object at a distance of the radius away from it's current value depending on the position that it is currently at. This function is not virtual because it is never redefined.
void Bacterium::createNewBacterium(int n)
{
    int newX = getX();
    if (newX < VIEW_WIDTH/2)
        newX += SPRITE_RADIUS;
    else
        newX -= SPRITE_RADIUS;
    int newY = getY();
    if (newY < VIEW_HEIGHT/2)
        newY += SPRITE_RADIUS;
    else
        newY -= SPRITE_RADIUS;
    //Calls addBacterium function in StudentWorld to add another Bacterium object
    getWorld()->addBacterium(newX, newY, n);
}

//This function increases the number of food that the object has eaten by the number passed in. This function is not virtual because it is never redefined.
void Bacterium::increaseFood(int n)
{
    foodCount += n;
}

//Returns the total food that the Bacterium object has eaten. This function is not virtual because it is never redefined.
int Bacterium::getFood()
{
    return foodCount;
}

//Increases the movement plan of the object by the number that is passed in. This function is not virtual because it is never redefined.
void Bacterium::increaseMovementPlan(int n)
{
    movementPlanDistance += n;
}

//Returns the movement plan of the Bacterium object. This function is not virtual because it is never redefined.
int Bacterium::getMovementPlan()
{
    return movementPlanDistance;
}

//Checks to see if the Bacterium object overlaps with Socrates and reduces Socrates' health by hp if it does. Also checks to see if the Bacterium object has eaten enough Food to create a new object or if it is overlapping with a Food object. This function is not virtual because it is never redefined.
void Bacterium::checkOverlap(int hp, int type) 
{
    Actor *a = getWorld()->getSocrates(getX(),getY(), SPRITE_RADIUS*2);
    if (a!=nullptr)
        a->reduceHitPoints(hp);
    else if (getFood() >= 3)
    {
        createNewBacterium(1);
        increaseFood(-getFood());
    }
    else if(getWorld()->foodOverlap(getX(), getY()))
        increaseFood(1);
}

//EColi class
//EColi constructor: Creates an object of a EColi and constructs the base Actor class using the image ID for EColi along with it's initial position and direction.
EColi::EColi(int x, int y, StudentWorld* sw)
: Bacterium (IID_ECOLI, 5, x, y, sw)
{
}

//Tries to move EColi object in the direction of the player and does not move forward if it cannot move towards the player, until the player moves and it can. This function is virtual for clarity because it is a redefinition of Actor's function.
void EColi::doSomething()
{
    if (!getLife())
        return;
    checkOverlap(4,3);
    double newX, newY;
    Actor *player = getWorld()->getSocrates(getX(),getY(), 256);
    //gets players direction and checks if it can move there
    getPositionInThisDirection(player->getDirection()+180, 2, newX, newY);
    for (int a = 1; a <= 10; a++)
    {
        if (getWorld()->canMove(newX,newY))
        {
            moveTo(newX,newY);
            return;
        }
        else
           getPositionInThisDirection(player->getDirection()+180+10*a, 2, newX, newY);
    }
}

//Reduces the health of the EColi object and kills it if it's health is below zero. There is a one in two chance that the EColi will create a Food object when it dies. This function is virtual because it is a redefnition of Bacterium's function.
void EColi::reduceHitPoints(int n)
{
    Bacterium::reduceHitPoints(n);
    getWorld()->playSound(SOUND_ECOLI_HURT);
    if (getHitPoints() <= 0)
    {
        getWorld()->playSound(SOUND_ECOLI_DIE);
        getWorld()->increaseScore(100);
        int chance = randInt(1,2);
        if (chance == 1)
            getWorld()->addFood(getX(),getY());
        setLife(false);
        
    }
}

//Salmonella class
//Salmonella constructor: Creates an object of a Salmonella and constructs the base Actor class using the image ID for Salmonella along with it's initial position and direction.
Salmonella::Salmonella(int i, int hp, int x, int y, StudentWorld* sw)
: Bacterium(i, hp, x, y, sw)
{
}

//Reduces the health of the Salmonella object and kills it if it's health is below zero. There is a one in two chance that the Salmonella will create a Food object when it dies. This function is virtual because it is a redefnition of Bacterium's function.
void Salmonella::reduceHitPoints(int n)
{
    Bacterium::reduceHitPoints(n);
    getWorld()->playSound(SOUND_SALMONELLA_HURT);
    if (getHitPoints() <= 0)
    {
        getWorld()->playSound(SOUND_SALMONELLA_DIE);
        getWorld()->increaseScore(100);
        int chance = randInt(1,2);
        if (chance == 1)
            getWorld()->addFood(getX(),getY());
        setLife(false);
    }
}

//Since all Salmonella objects will move randomly at times this function moves the object in a random direction or moves it in the direction of its movement plan, choosing a random direction if it is blocked by an object. This function also moves objects towards food if possible. This function is not virtual because it is never redefined.
void Salmonella::runMovementPlan()
{
    //Moves object in direction of its movement plan
    if (getMovementPlan() > 0)
    {
        increaseMovementPlan(-1);
        double newX, newY;
        getPositionInThisDirection(getDirection(), 3, newX, newY);
        if (getWorld()->canMove(newX, newY))
        {
            moveTo(newX,newY);
        }
        else
        {
            int angle = randInt(0,359);
            setDirection(angle);
            increaseMovementPlan(10-getMovementPlan());
        }
        return;
    }
    //Tries to move object towards nearby Food but if not Food is found then it moves in a random direction.
    else
    {
        Actor *a = getWorld()->findNearestFood(getX(), getY());
        if (a!= nullptr)
        {
            double newX, newY;
            getPositionInThisDirection(a->getDirection(), 3, newX, newY);
            if (getWorld()->canMove(newX,newY))
                moveTo(newX, newY);
            else
            {
                int angle = randInt(0,359);
                setDirection(angle);
                increaseMovementPlan(10-getMovementPlan());
                return;
            }
        }
        else
        {
            int angle = randInt(0,359);
            setDirection(angle);
            increaseMovementPlan(10-getMovementPlan());
            return;
        }
    }
}

//RegularSalmonella class
//RegularSalmonella constructor: Creates an object of a Salmonella and constructs the base Actor class using the image ID for Salmonella along with it's initial position and direction.
RegularSalmonella::RegularSalmonella(int x, int y, StudentWorld* sw)
: Salmonella(IID_SALMONELLA, 4, x, y, sw)
{
}

//RegularSalmonella objects mostly do what RegularSalmonella objects do so this function checks if the object overlaps with anything and moves according to the RegularSalmonella's runMovementPlan function
void RegularSalmonella::doSomething()
{
    if (!getLife())
        return;
    checkOverlap(1,1);
    runMovementPlan();
}

//AggressiveSalmonella class
//AggressiveSalmonella constructor: Creates an object of a AggressiveSalmonella and constructs the base Actor class using the image ID for AggressiveSalmonella along with it's initial position and direction.
AggressiveSalmonella::AggressiveSalmonella(int x, int y, StudentWorld* sw)
: Salmonella(IID_SALMONELLA, 10, x, y, sw)
{
}

//This object moves similarly to its base class but if Socrates is nearby it will move towards Socrates so this function first tries to find Socrates and move towards it and if it cannot do that it begins to move according to how a Salmonella moves
void AggressiveSalmonella::doSomething()
{
    if (!getLife())
        return;
    double newX, newY;
    Actor *a = getWorld()->getSocrates(getX(),getY(),72);
    if (a!=nullptr)
    {
        getPositionInThisDirection(a->getDirection(), 3, newX, newY);
        if (getWorld()->canMove(newX,newY))
            moveTo(newX, newY);
        checkOverlap(2,2);
        return;
    }
    checkOverlap(2,2);
    runMovementPlan();
}


