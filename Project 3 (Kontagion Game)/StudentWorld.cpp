#include "StudentWorld.h"
#include "GameConstants.h"
#include <vector>
#include <string>
#include <cmath>
using namespace std;

#include "Actor.h"

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

//Initializes all the objects that show up at the beginning of the game. Creates Pit objects, Food objects and Dirt objects, choosing the number based on the level that the player is on. Creates the player object as well.
int StudentWorld::init()
{
    m_player = new Socrates(this);
    numBacteria = getLevel() * (5+3+2);
    
    //Adds pits to the game. Creates the same number of pits as the level number.
    int numObjects = 0;
    for (int a = 0; a < getLevel(); a++)
    {
        int x = 0;
        int y = 0;
        while (!(pow(VIEW_WIDTH/2-x,2)+ pow(VIEW_WIDTH/2-y,2)<= 120*120))
        {
            x = randInt(VIEW_WIDTH/2-120, VIEW_WIDTH/2+120);
            y = randInt(VIEW_HEIGHT/2-120, VIEW_HEIGHT/2+ 120);
        }
        
        bool addPoint = true;
        for (int b = 0; b < numObjects && addPoint; b++)
        {
            if (abs(m_actors[b]->getX() -x) < 2*SPRITE_RADIUS || abs(m_actors[b]->getY() -y) < 2*SPRITE_RADIUS)
                addPoint = false;
        }
        if (addPoint)
        {
            m_actors.push_back(new Pit(x,y, this));
            numObjects++;
        }
        else
            a--;
    }
    
    //Adds Food objects to the game. Makes sure they don't overlap with any existing pits.
    for (int a = 0; a < min(5*getLevel(),25); a++)
    {
        int x = 0;
        int y = 0;
        while (!(pow(VIEW_WIDTH/2-x,2)+ pow(VIEW_WIDTH/2-y,2)<= 120*120))
        {
            x = randInt(VIEW_WIDTH/2-120, VIEW_WIDTH/2+120);
            y = randInt(VIEW_HEIGHT/2-120, VIEW_HEIGHT/2+ 120);
        }
        
        bool addPoint = true;
        for (int b = 0; b < numObjects && addPoint; b++)
        {
            if (abs(m_actors[b]->getX() -x) < 2*SPRITE_RADIUS || abs(m_actors[b]->getY() -y) < 2*SPRITE_RADIUS)
                addPoint = false;
        }
        if (addPoint)
        {
            m_actors.push_back(new Food(x,y, this));
            numObjects++;
        }
        else
            a--;
    }
    
    //Adds Dirt objects to the game and makes sure that it does not overlap with any previously added objects.
    for (int a = 0; a < max(180 - 20 * getLevel(), 20); a++)
    {
        int x = 0;
        int y = 0;
        while (!(pow(VIEW_WIDTH/2-x,2)+ pow(VIEW_WIDTH/2-y,2)<= 120*120))
        {
            x = randInt(VIEW_WIDTH/2-120, VIEW_WIDTH/2+120);
            y = randInt(VIEW_HEIGHT/2-120, VIEW_HEIGHT/2+ 120);
        }
        
        bool addPoint = true;
        for (int b = 0; b < numObjects && addPoint; b++)
        {
            if (abs(m_actors[b]->getX() -x) < 2*SPRITE_RADIUS || abs(m_actors[b]->getY() -y) < 2*SPRITE_RADIUS)
                addPoint = false;
        }
        if (addPoint)
            m_actors.push_back(new Dirt(x,y, this));
        else
            a--;
    }
    return GWSTATUS_CONTINUE_GAME;
}

//Plays one tick of the game. Checks if any actors have died and removes them from the game if they have. Calls each Actor's doSomething function and resets game statistics to reflect current values. Adds any Goodies based on the chance that they have to be added.
int StudentWorld::move()
{
    m_player->doSomething();
    for (int a = 0; a < m_actors.size(); a++)
    {
        //Checks if player has died
        if (!m_player->getLife())
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        //Checks if player has killed all bacteria
        if (numBacteria <= 0)
            return GWSTATUS_FINISHED_LEVEL;
        //Checks if any actors have died and removes them if they have
        if (!m_actors[a]->getLife())
        {
            if (m_actors[a]->isBacteria()) 
                numBacteria--;
            delete m_actors[a];
            m_actors.erase(m_actors.begin()+a);
        }
        else
            (m_actors[a])->doSomething();
        
        setGameStatText("Score: " + to_string(getScore()) + " Level: " + to_string(getLevel()) + " Lives: " + to_string(getLives()) + " Health: " + to_string(m_player->getHitPoints()) + " Sprays: " + to_string(m_player->getSpray()) + " Flames: " + to_string(m_player->getFlame()));
        
    }
    
    //Checks chance for each Goodie and then adds new Goodies to random positions
    const double PI = 4 * atan(1);
    int chanceFungus = min(510-getLevel()*10,200);
    int rand = randInt(0,chanceFungus-1);
    if (rand == 0)
    {
        int angle = randInt(0,359);
        m_actors.push_back(new Fungus(VIEW_RADIUS*cos(angle*PI/180)+128, VIEW_RADIUS*sin(angle*PI/180)+128, this));
    }
    
    int chanceGoodie = min(510-getLevel()*10,250);
    rand = randInt(0,chanceGoodie-1);
    if (rand == 0)
    {
        int angle = randInt(0,359);
        int newRand = randInt(1,10);
        if (newRand >= 1 && newRand <=6)
            m_actors.push_back(new RestoreHealthGoodie(VIEW_RADIUS*cos(angle*PI/180)+128, VIEW_RADIUS*sin(angle*PI/180)+128, this));
        else if(newRand >=7 && newRand <= 9)
            m_actors.push_back(new FlameThrowerGoodie(VIEW_RADIUS*cos(angle*PI/180)+128, VIEW_RADIUS*sin(angle*PI/180)+128, this));
        else if (newRand == 10)
            m_actors.push_back(new ExtraLifeGoodie(VIEW_RADIUS*cos(angle*PI/180)+128, VIEW_RADIUS*sin(angle*PI/180)+128, this));
    }
    return GWSTATUS_CONTINUE_GAME;
}

//Deletes player from the game if it hasn't already been deleted and deletes all the other actors from the game and the vector.
void StudentWorld::cleanUp()
{
    if (m_player != nullptr)
        delete m_player;
    m_player = nullptr;
    for (int a = 0; a < m_actors.size(); a++)
        delete m_actors[a];
    m_actors.erase(m_actors.begin(),m_actors.begin() + m_actors.size());
}


//Returns true if there is a damageable object that overlaps with the given x and y position
Actor* StudentWorld::getDamageableObject(int x, int y)
{
    for (int a = 0; a < m_actors.size(); a++)
    {
        if (m_actors[a]->isDamageable() && (pow(m_actors[a]->getX()-x,2) + pow(m_actors[a]->getY()-y,2)) <= pow(SPRITE_RADIUS*2,2))
            return m_actors[a];
    }
    
    return nullptr;
}

//Returns a pointer to Socrates if the given x and y position are less than a distance of d away from Socrates
Actor* StudentWorld::getSocrates(int x, int y, int d) 
{
    if (pow(m_player->getX()-x,2) + pow(m_player->getY()-y,2) <= pow(d,2))
        return m_player;
    return nullptr;
}

//Returns the Food object that is closest to the x and y position given and returns nullptr if it does not exist.
Actor* StudentWorld::findNearestFood(int x, int y)
{
    int a = 0;
    Actor *food = nullptr;
    //finds the first Food object in the list
    while (food == nullptr && a < m_actors.size())
    {
        if (m_actors[a]->isEdible())
            food = m_actors[a];
        a++;
    }
    
    //Runs through every Food object and checks if the position is closer to x and y than the other Food objects
    for (; a < m_actors.size(); a++)
    {
        if (m_actors[a]->isEdible() && (pow(m_actors[a]->getX()-x,2) + pow(m_actors[a]->getY()-y,2)) <= (pow(food->getX()-x,2) + pow(food->getY()-y,2)))
            food = m_actors[a];
    }
    return food;
}

//adds a Spray to the vector of actors with the given position and direction
void StudentWorld::addSpray(int x, int y, int d) 
{
    m_actors.push_back(new Spray(x,y,d,this));
}

//adds a Flame to the vector of actors with the given position and direction
void StudentWorld::addFlame(int x, int y, int d)
{
    m_actors.push_back(new Flame(x,y,d,this));
}

//adds a Bacterium to the vector of actors with the given position and type of Bacterium
void StudentWorld::addBacterium(int x, int y, int type)
{
    if (type == 1)
        m_actors.push_back(new RegularSalmonella(x,y,this));
    if (type == 2)
        m_actors.push_back(new AggressiveSalmonella(x,y,this));
    if (type == 3)
        m_actors.push_back(new EColi(x,y,this));
}

//adds a Food object to the vector of actors with the given position
void StudentWorld::addFood(int x, int y)
{
    m_actors.push_back(new Food(x,y,this));
}

//Checks if there is a Food object that overlaps with the object at the given position
bool StudentWorld::foodOverlap(int x, int y)
{
    for (int a = 0; a < m_actors.size(); a++)
    {
        if (m_actors[a]->isEdible() && (pow(m_actors[a]->getX()-x,2) + pow(m_actors[a]->getY()-y,2)) <= pow(SPRITE_RADIUS*2,2))
        {
            m_actors[a]->setLife(false);
            return true;
        }
    }
    return false; 
}

//Returns if an object can move to the given position and is not blocked by any obstacles
bool StudentWorld::canMove(int x, int y)
{
    for (int a = 0; a < m_actors.size(); a++)
    {
        if (m_actors[a]->isObstacle() && (pow(m_actors[a]->getX()-x,2) + pow(m_actors[a]->getY()-y,2)) <= pow(SPRITE_RADIUS*2,2))
            return false;
        if (pow(x-VIEW_WIDTH/2,2) +  pow(y-VIEW_HEIGHT/2,2) >= VIEW_RADIUS*VIEW_RADIUS)
            return false;
    }
    
    return true;
}

//Calls the cleanUp function to delete all pointers
StudentWorld::~StudentWorld()
{
    cleanUp();
}
