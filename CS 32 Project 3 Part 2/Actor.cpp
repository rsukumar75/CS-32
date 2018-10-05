#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor :: Actor(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* curr_world)
: GraphObject(imageID, startX, startY, startDirection, size, depth)
{
    m_alive = true;
    m_world = curr_world;
}

Actor :: ~Actor()
{}

bool Actor :: getStatus()
{
    return m_alive;
}

void Actor :: setStatus(bool x)
{
    m_alive = x;
}

StudentWorld* Actor :: getWorld()
{
    return m_world;
}

bool Actor :: collide(Actor *x)
{
    return (sqrt(((x->getX()-getX())*(x->getX()-getX())) + ((x->getY()-getY())*(x->getY()-getY()))) < (0.75 * (getRadius()+x->getRadius())));
}

bool Actor :: playerProjectile()
{
    return false;
}

bool Actor :: hasFlightPlan()
{
    return false;
}

bool Actor :: hasCabbages()
{
    return false;
}

bool Actor :: spins()
{
    return false;
}

bool Actor :: hasPathLength()
{
    return false;
}

void Actor :: sufferDamage(Actor *a)
{}

Star :: Star(int startX, int startY, StudentWorld* curr_world)
: Actor(IID_STAR, startX, startY, 0, randInt(5, 50)/100.0, 3, curr_world) 
{}

Star :: ~Star()
{}

void Star :: doSomething()
{
    moveTo(getX()-1, getY());
    if(getX()<0)
        setStatus(false);
}

NachenBlaster :: NachenBlaster(StudentWorld* curr_world)
: Actor(IID_NACHENBLASTER,0,128,0,1.0,0,curr_world)
{
    m_health = 50;
    m_cabbageEnergy = 30;
    m_torpedoes = 0;
}

NachenBlaster :: ~NachenBlaster()
{}

void NachenBlaster :: doSomething() // need to implement collisions and flatulence torpedoes
{
    if(getHealth()<=0)
    {
        setStatus(false);
        getWorld()->decLives();
        return;
    }
    if(!getStatus())
        return;
    int ch;
    if(getWorld()->getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_LEFT:
                if(getX() >= 6)
                    moveTo(getX()-6, getY());
                break;
            case KEY_PRESS_RIGHT:
                if(getX() < VIEW_WIDTH-6)
                    moveTo(getX()+6, getY());
                break;
            case KEY_PRESS_UP:
                if(getY() < VIEW_HEIGHT-6)
                    moveTo(getX(), getY()+6);
                break;
            case KEY_PRESS_DOWN:
                if(getY() >= 6)
                    moveTo(getX(), getY()-6);
                break;
            case KEY_PRESS_SPACE:
                if(m_cabbageEnergy >= 5)
                {
                    getWorld()->addActor(new Cabbage(getX()+12,getY(),getWorld()));
                    m_cabbageEnergy -= 5;
                    getWorld()->playSound(SOUND_PLAYER_SHOOT);
                }
                break;
            case KEY_PRESS_TAB: 
                if(m_torpedoes>0)
                {
                    getWorld()->addActor(new FlatulenceTorpedo(getX()+12,getY(),0,getWorld()));
                    m_torpedoes--;
                    getWorld()->playSound(SOUND_TORPEDO);
                }
                break;
        }
    }
    getWorld()->alienDamaged(this);
    if(m_cabbageEnergy<30)
        m_cabbageEnergy++;
}

int NachenBlaster :: getHealth()
{
    return m_health;
}

void NachenBlaster :: setHealth(int x)
{
    m_health = x;
}
int NachenBlaster :: cabbageEnergy()
{
    return m_cabbageEnergy;
}

int NachenBlaster :: torpedoes()
{
    return m_torpedoes;
}

void NachenBlaster :: setTorpedoes(int x)
{
    m_torpedoes = x;
}

bool NachenBlaster :: hasCabbages()
{
    return true;
}

void NachenBlaster :: sufferDamage(Actor* a)
{
    if(a->hasFlightPlan())
    {
        if(a->hasPathLength())
            m_health -= 5;
        else
            m_health -= 15;
    }
    else
    {
        if(a->spins())
            m_health -= 2;
        else
            m_health -= 8;
        getWorld()->playSound(SOUND_BLAST);
    }
}

Projectile :: Projectile(int imageID, int startX, int startY, int startDirection, StudentWorld* curr_world)
: Actor(imageID,startX,startY,startDirection,0.5,1,curr_world)
{}

Projectile :: ~Projectile()
{}

void Projectile :: doSomething()
{
    doCommonStuff();
    doDifferentiatedStuff();
}

void Projectile :: doCommonStuff()
{
    if(!getStatus())
        return;
    if(getX()>=VIEW_WIDTH || getX()<0)
    {
        setStatus(false);
        return;
    }
}

bool Projectile :: playerProjectile()
{
    return true;
}

bool Projectile :: spins()
{
    return true;
}

Cabbage :: Cabbage(int startX, int startY, StudentWorld* curr_world)
:Projectile(IID_CABBAGE, startX, startY, 0, curr_world) 
{}

Cabbage :: ~Cabbage()
{}

void Cabbage :: doDifferentiatedStuff() // need to implement collisions and damage
{
    if(getWorld()->alienDamaged(this))
    {
        setStatus(false);
        return;
    }
    moveTo(getX()+8, getY());
    setDirection(getDirection()+20);
    if(getWorld()->alienDamaged(this))
    {
        setStatus(false);
        return;
    }
}

Turnip :: Turnip(int startX, int startY, StudentWorld* curr_world)
:Projectile(IID_TURNIP,startX,startY,0,curr_world)
{}

Turnip :: ~Turnip()
{}

void Turnip :: doDifferentiatedStuff()
{
   if(getWorld()->playerDamaged(this))
   {
       setStatus(false);
       return;
   }
    moveTo(getX()-6, getY());
    setDirection(getDirection()+20);
    if(getWorld()->playerDamaged(this))
    {
        setStatus(false);
        return;
    }
}

bool Turnip :: playerProjectile()
{
    return false;
}

FlatulenceTorpedo :: FlatulenceTorpedo(int startX, int startY, int startDirection, StudentWorld* curr_world)
:Projectile(IID_TORPEDO,startX,startY,startDirection,curr_world)
{}

FlatulenceTorpedo :: ~FlatulenceTorpedo()
{}

void FlatulenceTorpedo :: doDifferentiatedStuff()
{
    if(getDirection()==0)
    {
        if(getWorld()->alienDamaged(this))
        {
            setStatus(false);
            return;
        }
    }
    else if(getDirection()==180)
    {
        if(getWorld()->playerDamaged(this))
        {
            setStatus(false);
            return;
        }
    }
    if(getDirection()==0)
        moveTo(getX()+8, getY());
    else if(getDirection()==180)
        moveTo(getX()-8, getY());
    if(getDirection()==0)
    {
        if(getWorld()->alienDamaged(this))
        {
            setStatus(false);
            return;
        }
    }
    else if(getDirection()==180)
    {
        if(getWorld()->playerDamaged(this))
        {
            setStatus(false);
            return;
        }
    }
}

bool FlatulenceTorpedo :: spins()
{
    return false;
}

Alien :: Alien(int imageID, int startX, int startY, StudentWorld* curr_world, double health, int path, double speed)
: Actor(imageID,startX,startY,0,1.5,1,curr_world)
{
    m_health=health;
    m_path=path;
    m_speed=speed;
    m_travelDirection= "DL";
}

Alien :: ~Alien()
{}

void Alien :: doSomething()
{
    doCommonStuffA();
    if(doDifferentiatedStuff())
        doCommonStuffB();
    //check collisions again
}

void Alien :: doCommonStuffA()
{
    if(!getStatus())
        return;
    if(getX()<0)
    {
        setStatus(false);
        getWorld()->setAliens(getWorld()->getAliens()-1);
        return;
    }    
}

void Alien :: doCommonStuffB()
{
    if(getPath()==0)
    {
        int a = randInt(1, 3);
        if(a==1)
            setDirection("L");
        else if(a==2)
            setDirection("UL");
        else
            setDirection("DL");
        setPath(randInt(1, 32));
    }
    if(getY()>=VIEW_HEIGHT-1)
        setDirection("DL");
    else if(getY()<=0)
        setDirection("UL");
    
    if(getDirection()=="UL")
    {
        if(getY() + getSpeed() <= VIEW_HEIGHT-1)
            moveTo(getX()-getSpeed(), getY()+getSpeed());
        else
            moveTo(getX()-getSpeed(),VIEW_HEIGHT-1);
    }
    else if(getDirection()=="DL")
    {
        if(getY() - getSpeed() >= 0)
            moveTo(getX()-getSpeed(), getY()-getSpeed());
        else
            moveTo(getX()-getSpeed(), 0);
    }
    else
        moveTo(getX()-getSpeed(), getY());
    setPath(getPath()-1);
}

bool Alien :: hasFlightPlan()
{
    return true;
}

bool Alien :: hasPathLength()
{
    return true;
}

void Alien :: sufferDamage(Actor* a)
{
    commonDamage(a);
    if(m_health<=0 || !getStatus())
        differentDamage();
    getWorld()->playSound(SOUND_BLAST);
}

void Alien :: commonDamage(Actor* a)
{
    if(a->playerProjectile() && a->spins())
        m_health-=2;
    else if(a->playerProjectile() && !a->spins())
        m_health-=8;
    else if(a->hasCabbages())
        setStatus(false);
}

int Alien :: getPath()
{
    return m_path;
}

void Alien :: setPath(int x)
{
    m_path=x;
}

std::string Alien :: getDirection()
{
    return m_travelDirection;
}

void Alien :: setDirection(std::string s)
{
    m_travelDirection = s;
}

int Alien :: getSpeed()
{
    return m_speed;
}

void Alien :: setSpeed(int x)
{
    m_speed = x;
}

Smallgon :: Smallgon(int startX, int startY, StudentWorld* curr_world)
: Alien(IID_SMALLGON,startX,startY,curr_world,(5*(1+(curr_world->getLevel()-1)*0.1)),0,2.0)
{}

Smallgon :: ~Smallgon()
{}


bool Smallgon :: doDifferentiatedStuff()
{
    if(getWorld()->getPlayerX()<getX() && ((getY()<=getWorld()->getPlayerY()+4) && (getY()>=getWorld()->getPlayerY()-4)))
    {
        int x = 20/getWorld()->getLevel() + 5;
        int p=randInt(1, x);
        if(p==1)
        {
            getWorld()->addActor(new Turnip(getX()-14,getY(),getWorld()));
            getWorld()->playSound(SOUND_ALIEN_SHOOT);
            return false;
        }
    }
    return true;
}

void Smallgon :: differentDamage()
{
    getWorld()->increaseScore(250);
    setStatus(false);
    getWorld()->playSound(SOUND_DEATH);
    getWorld()->addActor(new Explosion(getX(),getY(),getWorld()));
    getWorld()->setAliens(getWorld()->getAliens()-1);
}

Smoregon :: Smoregon(int startX, int startY, StudentWorld* curr_world)
:Alien(IID_SMOREGON,startX,startY,curr_world,(5*(1+(curr_world->getLevel()-1)*0.1)),0,2.0)
{}

Smoregon :: ~Smoregon()
{}

void Smoregon :: differentDamage()
{
    getWorld()->increaseScore(250);
    setStatus(false);
    getWorld()->playSound(SOUND_DEATH);
    getWorld()->addActor(new Explosion(getX(),getY(),getWorld()));
    getWorld()->setAliens(getWorld()->getAliens()-1);
    int p = randInt(1, 3); 
    int g = randInt(1, 2);
    if(p==1)
    {
        if(g==1)
            getWorld()->addActor(new repairGoodie(getX(),getY(),getWorld()));
        else
            getWorld()->addActor(new flatulenceTorpedoGoodie(getX(),getY(),getWorld()));
    }
}

bool Smoregon :: doDifferentiatedStuff()
{
    if(getWorld()->getPlayerX()<getX() && ((getY()<=getWorld()->getPlayerY()+4) && (getY()>=getWorld()->getPlayerY()-4)))
    {
        int x = 20/getWorld()->getLevel() + 5;
        int p=randInt(1, x);
        if(p==1)
        {
            getWorld()->addActor(new Turnip(getX()-14,getY(),getWorld()));
            getWorld()->playSound(SOUND_ALIEN_SHOOT);
            return false;
        }
        
        if(p==2)
        {
            setDirection("L");
            setPath(VIEW_WIDTH);
            setSpeed(5);
            return true;
        }
    }
    return true;
}

Snagglegon :: Snagglegon(int startX, int startY, StudentWorld* curr_world)
:Alien(IID_SNAGGLEGON,startX,startY,curr_world,(10*(1+(curr_world->getLevel()-1)*0.1)),-1,1.75)
{}

Snagglegon :: ~Snagglegon()
{}

void Snagglegon :: differentDamage()
{
    getWorld()->increaseScore(1000);
    setStatus(false);
    getWorld()->playSound(SOUND_DEATH);
    getWorld()->addActor(new Explosion(getX(),getY(),getWorld()));
    getWorld()->setAliens(getWorld()->getAliens()-1);
    int p = randInt(1, 6);
    if(p==1)
    {
        getWorld()->addActor(new extraLifeGoodie(getX(),getY(),getWorld()));

    }
}

bool Snagglegon :: doDifferentiatedStuff()
{
    if(getWorld()->getPlayerX()<getX() && ((getY()<=getWorld()->getPlayerY()+4) && (getY()>=getWorld()->getPlayerY()-4)))
    {
        int x = 15/getWorld()->getLevel() + 10;
        int p=randInt(1, x);
        if(p==1)
        {
            getWorld()->addActor(new FlatulenceTorpedo(getX()-14,getY(),180,getWorld()));
            getWorld()->playSound(SOUND_TORPEDO);
            return false;
        }
    }
    return true;
}

bool Snagglegon :: hasPathLength()
{
    return false;
}

Explosion :: Explosion(int startX, int startY, StudentWorld* curr_world)
:Actor(IID_EXPLOSION,startX,startY,0,1,0,curr_world)
{
    m_health = 4;
}

Explosion :: ~Explosion()
{}

void Explosion :: doSomething()
{
    if(m_health==0)
    {
        setStatus(false);
        return;
    }
    setSize(getSize()*1.5);
    m_health--;
}

Goodie :: Goodie(int imageID, int startX, int startY, StudentWorld* curr_world)
:Actor(imageID,startX,startY,0,0.5,1,curr_world)
{}

Goodie :: ~Goodie()
{}

void Goodie :: doSomething()
{
    if(!getStatus())
        return;
    if(getX()<0 || getY()<0)
    {
        setStatus(false);
        return;
    }
    if(getWorld()->playerGetsGoodie(this))
    {
        getWorld()->increaseScore(100);
        setStatus(false);
        getWorld()->playSound(SOUND_GOODIE);
        doDifferentiatedStuff();
        return;
    }
        moveTo(getX()-0.75, getY()-0.75);
    if(getWorld()->playerGetsGoodie(this))
    {
        getWorld()->increaseScore(100);
        setStatus(false);
        getWorld()->playSound(SOUND_GOODIE);
        doDifferentiatedStuff();
        return;
    }
}

extraLifeGoodie :: extraLifeGoodie(int startX, int startY, StudentWorld* curr_world)
:Goodie(IID_LIFE_GOODIE,startX,startY,curr_world)
{}

extraLifeGoodie :: ~extraLifeGoodie()
{}

void extraLifeGoodie :: doDifferentiatedStuff()
{
    getWorld()->incLives();
}

repairGoodie :: repairGoodie(int startX, int startY, StudentWorld* curr_world)
:Goodie(IID_REPAIR_GOODIE,startX,startY,curr_world)
{}

repairGoodie :: ~repairGoodie()
{}

void repairGoodie :: doDifferentiatedStuff()
{
    getWorld()->increasePlayerHealth(10);
}

flatulenceTorpedoGoodie :: flatulenceTorpedoGoodie(int startX, int startY, StudentWorld* curr_world)
:Goodie(IID_TORPEDO_GOODIE,startX,startY,curr_world)
{}

flatulenceTorpedoGoodie :: ~flatulenceTorpedoGoodie()
{}

void flatulenceTorpedoGoodie :: doDifferentiatedStuff()
{
    getWorld()->increaseTorpedoes(5);
}





















