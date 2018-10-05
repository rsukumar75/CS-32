#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{}

StudentWorld :: ~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    m_destroyedAliens = 0;
    m_remainingAliens = 6 + (4*getLevel());
    m_maxAliens = 4 + (0.5*getLevel());
    m_numAliens = 0;
    m_player = new NachenBlaster(this);
    for(int i=1;i<=30;i++)
    {
        Star* s = new Star(randInt(0, VIEW_WIDTH-1), randInt(0, VIEW_HEIGHT-1), this);
        m_actors.push_back(s);
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    vector <Actor*> :: iterator it;
    it = m_actors.begin();
    while(it!= m_actors.end())
    {
        if((*it)->getStatus())
        {
            (*it)->doSomething();
        }
        it++;
    }
    m_player->doSomething();
    removeDeadGameObjects();
    int p_star = randInt(1, 15);
    if(p_star==1)
        m_actors.push_back(new Star(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
    if(m_numAliens < min(m_maxAliens,m_remainingAliens))
    {
        int S1 = 60;
        int S2 = 20 + (getLevel()*5);
        int S3 = 5 + (getLevel()*10);
        int S = S1+S2+S3;
        double small = static_cast<double>(S1)/S * 100;
        double smore = (static_cast<double>(S2)/S * 100) + small;
        int p = randInt(1,100);
        if(p<=small)
            m_actors.push_back(new Smallgon(VIEW_WIDTH-1,randInt(0,VIEW_HEIGHT-1),this));
        else if(p<=smore)
            m_actors.push_back(new Smoregon(VIEW_WIDTH-1,randInt(0, VIEW_HEIGHT-1),this));
        else
            m_actors.push_back(new Snagglegon(VIEW_WIDTH-1,randInt(0, VIEW_HEIGHT-1),this));
        m_numAliens++;
    }
    ostringstream oss;
    oss << "Lives:" << setw(countDigits(getLives())+1) << getLives();
    oss <<  setw(9) << "Health:" << setw(countDigits(static_cast<int>(round((m_player->getHealth()/50.0) * 100)))+1) << static_cast<int>(round((m_player->getHealth()/50.0) * 100)) << "%";
    oss << setw(8) << "Score:" ;
    if(getScore()>0)
        oss << setw(countDigits(getScore())+1) << getScore();
    else
        oss << setw(2) << getScore();
    oss << setw(8) << "Level:" << setw(countDigits(getLevel())+1) << getLevel();
    oss << setw(11) << "Cabbages:" << setw(countDigits((static_cast<int>(round((m_player->cabbageEnergy()/30.0)*100))))+1) << static_cast<int>(round((m_player->cabbageEnergy()/30.0)*100)) << "%";
    oss << setw(12) << "Torpedoes:" ;
    if(m_player->torpedoes()>0)
        oss << setw(countDigits(m_player->torpedoes())+1) << m_player->torpedoes();
    else
        oss << setw(2) << m_player->torpedoes();
    string s = oss.str();
    setGameStatText(s);
    if(!m_player->getStatus())
        return GWSTATUS_PLAYER_DIED;
     if(m_remainingAliens==0)
         return GWSTATUS_FINISHED_LEVEL;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_player;
    m_player=nullptr;
    vector <Actor*> :: iterator it;
    it=m_actors.begin();
    while(it!=m_actors.end())
    {
        delete (*it);
        *it=nullptr;
        it=m_actors.erase(it);
    }
}

void StudentWorld :: addActor(Actor* a) 
{
    m_actors.push_back(a);
}

int StudentWorld :: getAliens()
{
    return m_numAliens;
}

void StudentWorld :: setAliens(int x)
{
    m_numAliens=x;
}

int StudentWorld :: getPlayerX()
{
    return m_player->getX();
}

int StudentWorld :: getPlayerY()
{
    return m_player->getY();
}

void StudentWorld :: removeDeadGameObjects()
{
    vector <Actor*> :: iterator it;
    it = m_actors.begin();
    while(it!=m_actors.end())
    {
        if(!(*it)->getStatus())
        {
            delete (*it);
            *it=nullptr;
            it=m_actors.erase(it);
        }
        else
            it++;
    }
}
                            
int StudentWorld :: countDigits(int x)
{
    int cnt=0;
    while(x!=0)
    {
        cnt++;
        x/=10;
    }
    return cnt;
}

bool StudentWorld :: alienDamaged(Actor *a)
{
    vector <Actor*> :: iterator it;
    it=m_actors.begin();
    while(it!=m_actors.end())
    {
        if((*it)->hasFlightPlan() && a->collide((*it)))
        {
            (*it)->sufferDamage(a);
            if(a->hasCabbages())
                a->sufferDamage(*it);
            if(!(*it)->getStatus())
            {
                m_destroyedAliens++;
                m_remainingAliens--;
            }
            return true;
        }
        it++;
    }
    return false;
}

bool StudentWorld :: playerDamaged(Actor *a)
{
    if(a->collide(m_player))
    {
        m_player->sufferDamage(a);
        return true;
    }
    return false;
}

bool StudentWorld :: playerGetsGoodie(Actor *a) 
{
    return a->collide(m_player);
}

void StudentWorld :: increasePlayerHealth(int x)
{
    if(m_player->getHealth()+x > 50)
        m_player->setHealth(50);
    else
        m_player->setHealth(m_player->getHealth()+x);
}

void StudentWorld :: increaseTorpedoes(int x)
{
    m_player->setTorpedoes(m_player->torpedoes()+x);
}
