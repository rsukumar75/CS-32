#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* curr_world);
    virtual ~Actor();
    bool getStatus();
    void setStatus(bool x);
    StudentWorld* getWorld();
    virtual void doSomething()=0;
    bool collide(Actor* x);
    virtual bool hasFlightPlan();
    virtual bool hasCabbages();
    virtual bool spins();
    virtual bool playerProjectile();
    virtual bool hasPathLength();
    virtual void sufferDamage(Actor* a);
private:
    bool m_alive;
    StudentWorld* m_world;
};

class Star : public Actor
{
public:
    Star(int startX, int startY, StudentWorld* curr_world);
    virtual ~Star();
    void doSomething();
};

class NachenBlaster : public Actor
{
public:
    NachenBlaster(StudentWorld* curr_world);
    virtual ~NachenBlaster();
    void doSomething();
    int getHealth();
    void setHealth(int x);
    int cabbageEnergy();
    int torpedoes();
    void setTorpedoes(int x);
    void sufferDamage(Actor* a);
    bool hasCabbages();
private:
    int m_health;
    int m_cabbageEnergy;
    int m_torpedoes;
};

class Alien : public Actor //Add functions to do common stuff and do differentiated stuff
{
public:
    Alien(int imageID, int startX, int startY, StudentWorld* curr_world, double health, int path, double speed);
    virtual ~Alien();
    int getPath();
    void setPath(int x);
    std::string getDirection();
    void setDirection(std::string);
    int getSpeed();
    void setSpeed(int x);
    void doSomething();
    void sufferDamage(Actor* a);
    bool hasFlightPlan();
    virtual bool hasPathLength();
private:
    double m_health;
    int m_path;
    double m_speed;
    std::string m_travelDirection;
    virtual bool doDifferentiatedStuff() = 0;
    void doCommonStuffA();
    void doCommonStuffB();
    void commonDamage(Actor* a);
    virtual void differentDamage()=0;
};

class Smoregon : public Alien
{
public:
    Smoregon(int startX, int StartY, StudentWorld* curr_world);
    virtual ~Smoregon();
private:
    virtual bool doDifferentiatedStuff();
    void differentDamage();
};

class Snagglegon : public Alien
{
public:
    Snagglegon(int startX, int StartY, StudentWorld* curr_world);
    virtual ~Snagglegon();
    bool hasPathLength();
private:
    virtual bool doDifferentiatedStuff();
    void differentDamage();
};

class Smallgon : public Alien
{
public:
    Smallgon(int startX, int startY, StudentWorld* curr_world);
    virtual ~Smallgon();
private:
    virtual bool doDifferentiatedStuff();
    void differentDamage();
};

class Projectile : public Actor
{
public:
    Projectile(int imageID, int startX, int startY, int startDirection, StudentWorld* curr_world);
    virtual ~Projectile();
    void doSomething();
    virtual bool spins();
    virtual bool playerProjectile();
private:
    virtual void doDifferentiatedStuff()=0;
    void doCommonStuff();
};

class Cabbage : public Projectile
{
public:
    Cabbage(int startX, int startY, StudentWorld* curr_world);
    virtual ~Cabbage();
private:
    void doDifferentiatedStuff();
};

class Turnip : public Projectile
{
public:
    Turnip(int startX, int startY, StudentWorld* curr_world);
    virtual ~Turnip();
    bool playerProjectile();
private:
    void doDifferentiatedStuff();
};

class FlatulenceTorpedo : public Projectile
{
public:
    FlatulenceTorpedo(int startX, int startY, int startDirection, StudentWorld* curr_world);
    virtual ~FlatulenceTorpedo();
    bool spins();
private:
    void doDifferentiatedStuff();
};

class Explosion : public Actor
{
public:
    Explosion(int startX,int startY,StudentWorld* curr_world);
    virtual ~Explosion();
    void doSomething();
private:
    int m_health;
};

class Goodie : public Actor
{
public:
    Goodie(int imageID, int startX, int startY, StudentWorld* curr_world);
    virtual ~Goodie();
    virtual void doSomething();
private:
    virtual void doDifferentiatedStuff()=0;
};

class extraLifeGoodie : public Goodie
{
public:
    extraLifeGoodie(int startX, int startY, StudentWorld* curr_world);
    virtual ~extraLifeGoodie();
private:
    void doDifferentiatedStuff();
};

class repairGoodie : public Goodie
{
public:
    repairGoodie(int startX, int startY, StudentWorld* curr_world);
    virtual ~repairGoodie();
private:
    void doDifferentiatedStuff();
};

class flatulenceTorpedoGoodie : public Goodie
{
public:
    flatulenceTorpedoGoodie(int startX, int startY, StudentWorld* curr_world);
    virtual ~flatulenceTorpedoGoodie();
private:
    void doDifferentiatedStuff();
};
#endif // ACTOR_H_
