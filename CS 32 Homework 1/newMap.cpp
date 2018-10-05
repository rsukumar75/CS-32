
#include "newMap.h"
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

Map :: Map(int n)
{
    if(n<0)
    {
        cout << "Capacity cannot be negative!" << endl;
        exit(1);
    }
    m_capacity=n;
    m_arr = new m_MapType[m_capacity];
    m_size = 0;
}

Map :: Map(const Map& cpy)
{
    m_capacity=cpy.m_capacity;
    m_size=cpy.m_size;
    m_arr=new m_MapType[m_capacity];
    for(int i=0;i<m_size;i++)
    {
        m_arr[i].key=cpy.m_arr[i].key;
        m_arr[i].value=cpy.m_arr[i].value;
    }
}

Map& Map :: operator=(const Map& cpy)
{
    if(&cpy==this)
        return *this;
    delete [] m_arr;
    m_capacity=cpy.m_capacity;
    m_size=cpy.m_size;
    m_arr=new m_MapType[m_capacity];
    for(int i=0;i<m_size;i++)
    {
        m_arr[i].key=cpy.m_arr[i].key;
        m_arr[i].value=cpy.m_arr[i].value;
    }
    return *this;
}

Map :: ~Map()
{
    delete [] m_arr;
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
    if(! contains(key) && size() < m_capacity)
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
    int temp=this->m_capacity;
    this->m_capacity=other.m_capacity;
    other.m_capacity=temp;
    int temp2=this->m_size;
    this->m_size=other.m_size;
    other.m_size=temp2;
    m_MapType* temp3=this->m_arr;
    this->m_arr=other.m_arr;
    other.m_arr=temp3;
}




