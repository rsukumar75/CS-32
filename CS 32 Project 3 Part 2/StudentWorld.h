#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <vector>
#include <string>

class NachenBlaster;
class Actor;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{ //FOR COLLISIONS: CHECK FOR CLASSES OF COMMON BEHAVIOUR TO IDENTIFY TYPES OF ACTORS INSTEAD OF HAVING AN ISTYPE() METHOD
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void addActor(Actor* a);
    void setAliens(int x);
    int getAliens();
    int getPlayerX();
    int getPlayerY();
    void increasePlayerHealth(int x);
    void increaseTorpedoes(int x);
    bool alienDamaged(Actor* a);
    bool playerDamaged(Actor* a);
    bool playerGetsGoodie(Actor* a);

private:
    NachenBlaster* m_player;
    std::vector <Actor*> m_actors;
    void removeDeadGameObjects();
    int m_destroyedAliens;
    int m_maxAliens;
    int m_remainingAliens;
    int m_numAliens;
    int countDigits(int x);
};

#endif // STUDENTWORLD_H_
