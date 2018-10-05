#include "CarMap.h"
#include <iostream>
using namespace std;
CarMap :: CarMap()
{
   
}

bool CarMap :: addCar(std::string license)
{
    if(!m_carMap.contains(license) && m_carMap.size()<DEFAULT_MAX_ITEMS)
    {
        return m_carMap.insert(license, 0);
    }
    return false;
}

double CarMap :: gas(std::string license) const
{
    ValueType v;
    if(m_carMap.contains(license))
    {
        m_carMap.get(license,v);
        return v;
    }
    return -1;
}

bool CarMap :: addGas(std::string license, double gallons)
{
    if(! m_carMap.contains(license) || (gallons<0))
        return false;
    else
    {
        m_carMap.update(license,(gas(license)+gallons));
        return true;
    }
}

bool CarMap :: useGas(std::string license, double gallons)
{
    if(! m_carMap.contains(license) || (gallons<0) || (gas(license)<gallons))
        return false;
    else
    {
        m_carMap.update(license,(gas(license)-gallons));
        return true;
    }
}

int CarMap :: fleetSize() const
{
    return m_carMap.size();
}

void CarMap :: print() const
{
    KeyType k;
    ValueType v;
    for(int i=0;i<m_carMap.size();i++)
    {
        m_carMap.get(i,k,v);
        cout << k << " " << v << endl;
    }
}

