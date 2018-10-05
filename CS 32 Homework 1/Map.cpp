#include <iostream>
#include <string>
#include "Map.h"
using namespace std;

Map :: Map()
{
    m_size=0;
}

bool Map :: empty() const
{
    return (m_size == 0);
}

int Map :: size() const
{
    return m_size;
}

bool Map :: insert(const KeyType& key, const ValueType& value)
{
    if(! contains(key) && size() < DEFAULT_MAX_ITEMS)
    {
        m_arr[size()].key = key;
        m_arr[size()].value = value;
        m_size++;
        return true;
    }
    return false;
}

bool Map :: update(const KeyType& key, const ValueType& value)
{
    if(contains(key))
    {
        for(int i=0;i<size();i++)
        {
            if(m_arr[i].key == key)
            {
                m_arr[i].value = value;
                return true;
            }
        }
    }
    return false;
}

bool Map :: insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if(contains(key))
    {
        return update(key, value);
    }
    else
        return insert(key, value);
}

bool Map :: erase(const KeyType& key)
{
    if(contains(key))
    {
        for(int i=0;i<size();i++)
        {
            if(m_arr[i].key == key)
            {
                for(int j=i;j<size()-1;j++)
                {
                    m_arr[j].key = m_arr[j+1].key;
                    m_arr[j].value = m_arr[j+1].value;
                }
                m_size--;
                return true;
            }
        }
    }
    return false;
}

bool Map :: contains(const KeyType& key) const
{
    for(int i=0;i<size();i++)
    {
        if(m_arr[i].key == key)
            return true;
    }
    return false;
}

bool Map :: get(const KeyType& key, ValueType& value) const
{
    if(contains(key))
    {
        for(int i=0;i<size();i++)
        {
            if(m_arr[i].key==key)
            {
                value=m_arr[i].value;
                return true;
            }
        }
    }
    return false;
    
}

bool Map :: get(int i, KeyType& key, ValueType& value) const
{
    if(0 <= i && i < size())
    {
        key=m_arr[i].key;
        value=m_arr[i].value;
        return true;
    }
    return false;
}

void Map :: swap(Map& other)
{
    Map* larger;
    Map* smaller;
    KeyType temp1;
    ValueType temp2;
    int temp3=0;
    if(this->size() > other.size())
    {
        larger=this;
        smaller=&other;
    }
    else
    {
        larger=&other;
        smaller=this;
    }
    temp3=smaller->size();
    smaller->m_size=larger->m_size;
    larger->m_size=temp3;
    for(int i=0;i<larger->size();i++)
    {
        temp1=smaller->m_arr[i].key;
        smaller->m_arr[i].key=larger->m_arr[i].key;
        larger->m_arr[i].key=temp1;
        temp2=smaller->m_arr[i].value;
        smaller->m_arr[i].value=larger->m_arr[i].value;
        larger->m_arr[i].value=temp2;
    }
    
    for(int j=larger->size();j<smaller->size();j++)
    {
        smaller->m_arr[j].key=larger->m_arr[j].key;
        smaller->m_arr[j].value=larger->m_arr[j].value;
    }
    
}


