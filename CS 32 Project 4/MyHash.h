//MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
//member functions; we have implemented the eighth.
#ifndef MYHASH_INCLUDED
#define MYHASH_INCLUDED
#include <iostream>

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;
    
private:
    struct Node
    {
        Node()
        :next(nullptr)
        {}
        ValueType val;
        KeyType key;
        Node* next;
    };
    
    Node** map;
    double maxLF;
    int items;
    int numBuckets;
    unsigned int getBucketNumber(const KeyType& key) const
    {
        unsigned int hash(const KeyType& k);
        return (hash(key) % numBuckets);
    }
};

template<typename KeyType, typename ValueType>
MyHash<KeyType,ValueType> :: MyHash(double maxLoadFactor)
:maxLF(maxLoadFactor), map(nullptr)
{
    if(maxLF <= 0)
        maxLF = 0.5;
    else if(maxLF > 2)
        maxLF = 2.0;
    map = new Node*[100];
    numBuckets = 100;
    items=0;
    for(int i=0;i<numBuckets;i++)
        map[i]=nullptr;
}

template<typename KeyType, typename ValueType>
MyHash<KeyType,ValueType> :: ~MyHash() // CHECK FOR O(B) COMPLEXITY
{
    for(int i=0;i<numBuckets;i++) //O(BN)
    { //O(N)
        while(map[i]!=nullptr) //O(N)
        {
            Node* temp = map[i]->next; //O(1)
            delete map[i]; //O(1)
            map[i] = temp; //O(1)
        }
    }
    delete [] map; //O(1)
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType,ValueType> :: reset() // CHECK FOR O(B) COMPLEXITY
{
    for(int i=0;i<numBuckets;i++)
    {
        while(map[i]!=nullptr)
        {
            Node* temp = map[i]->next;
            delete map[i];
            map[i] = temp;
        }
    }
    delete [] map;
    map = new Node*[100];
    numBuckets=100;
    for(int i=0;i<numBuckets;i++)
        map[i]=nullptr;
    items=0;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType,ValueType> :: associate(const KeyType& key, const ValueType& value) // CHECK FOR O(B) COMPLEXITY
{
    int index = getBucketNumber(key);
    Node* insert = new Node;
    insert->key=key;
    insert->val=value;
    insert->next=nullptr;
    Node* curr=map[index];
    if(map[index]==nullptr)
    {
        map[index]=insert;
        items++;
    }
    else
    {
        bool differentKey=true;
        while(curr->next!=nullptr)
        {
            if(curr->key==insert->key)
            {
                curr->val=insert->val;
                differentKey=false;
                break;
            }
            curr=curr->next;
        }
        if(curr->key==insert->key)
        {
            curr->val=insert->val;
            differentKey=false;
        }
        if(differentKey)
        {
            curr->next=insert;
            items++;
        }
        else
            delete insert;
    }

    if(getLoadFactor() > maxLF)
    {
        numBuckets *= 2; //O(1)
        Node** map2 = new Node*[numBuckets]; //O(1)
        for(int i=0;i<numBuckets;i++) //O(B)
            map2[i]=nullptr;
        // insert all items in map1 in map2 using hash function
        for(int i=0;i<numBuckets/2;i++)//O(BXY)
        {
            Node* current = map[i];
            while(current!=nullptr) //O(XY)
            { //O(X)
                int index = getBucketNumber(map[i]->key); //O(1)
                Node* insert = new Node;//O(1)
                insert->key=current->key;//O(1)
                insert->val=current->val;//O(1)
                insert->next=nullptr;//O(1)
                Node* curr = map2[index];//O(1)
                if(map2[index]==nullptr)
                    map2[index]=insert;//O(1)
                else
                {
                    bool differentKey=true;
                    while(curr->next!=nullptr) //O(X)
                    {//O(1)
                        if(curr->key==insert->key)
                        {
                            curr->val=insert->val;//O(1)
                            differentKey=false;//O(1)
                            break;//O(1)
                        }
                        curr=curr->next;//O(1)
                    }
                    if(curr->key==insert->key)
                    {
                        curr->val=insert->val;//O(1)
                        differentKey=false; //O(1)
                    }
                    if(differentKey)
                        curr->next=insert; //O(1)
                    else
                        delete insert; //O(1)
                }
                current=current->next; //O(1)
            }
        }
        for(int i=0;i<numBuckets/2;i++) //O(B)
        { //O(X)
            while(map[i]!=nullptr) //O(X)
            {
                Node* temp = map[i]->next; //O(1)
                delete map[i]; //O(1)
                map[i] = temp; //O(1)
            }
        }
        delete [] map; //O(1)
        map = map2; //(1)
    }
}

template<typename KeyType, typename ValueType>
int MyHash<KeyType,ValueType> :: getNumItems() const
{
    return items;
}

template<typename KeyType, typename ValueType>
double MyHash<KeyType,ValueType> :: getLoadFactor() const
{
    return static_cast<double>(getNumItems())/numBuckets;
}

template<typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType,ValueType> :: find(const KeyType& key) const
{
    int index = getBucketNumber(key);
    Node* curr = map[index];
    ValueType* value = nullptr;
    while(curr!=nullptr)
    {
        if(curr->key==key)
        {
            value=&(curr->val);
            break;
        }
        curr=curr->next;
    }
    return value;
}

#endif
