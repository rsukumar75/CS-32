#include "Map.h"
#include <iostream>
using namespace std;

Map :: Map() //Default constructor
{
    head=nullptr; //creates Map with no key/value pairs
}

Map :: Map(const Map& cpy) //Copy constructor
{
    Node* p=cpy.head; //Pointer to head of Map that is being copied
    while(p!=nullptr) //linked list traversal
    {
        this->insert(p->key, p->value); //inserts every node of Map cpy in current map being created
        p=p->next;
    }
}

Map :: ~Map() //destructor
{
    while(size()>0) //erase nodes of the current list until it becomes empty
    {
        erase(head->key); //erase head node
    }
}

Map& Map :: operator=(const Map& cpy) //Assignment function
{
    if(&cpy==this) //if Map being assigned is the same as the current Map simply return the current Map
        return *this;
    while(size()!=0) //delete all nodes that may exist in the current Map before assignment until it is empty
    {
        erase(head->key); //erase head node
    }
    Node* p=cpy.head; //Pointer to head of Map that is being assigned to current Map
    while(p!=nullptr) //linked list traversal
    {
        this->insert(p->key, p->value); //inserts every node of Map cpy in current map being created
        p=p->next;
    }
    return *this; //return current Map
}

bool Map :: empty() const //constant member function does not modify the data members of Map
{
    return (head==nullptr);  // Return true if the map is empty, otherwise false.
}

int Map :: size() const //constant member function does not modify the data members of Map
{  // Return the number of key/value pairs in the map.
    int size=0;
    for(Node* ptr=head;ptr!=nullptr;ptr=ptr->next)
    {//linked list traversal until pointer goes past the end of the list
        size++; //increment size variable
    }
    return size;
}

bool Map :: contains(const KeyType& key) const
{ //constant member function does not modify the data members of Map
    // Return true if key is equal to a key currently in the map, otherwise false.
    for(Node* p=head;p!=nullptr;p=p->next)
    { //linked list traversal until pointer goes past the end of the list
        if(p->key==key)
            return true;
    }
    return false;
}

bool Map :: erase(const KeyType &key)
{ // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.
    if(empty()) //return false if Map is empty
        return false;
    if(contains(key)) //If Map contains the key parameter
    {
        if(head->key==key && head->next==nullptr)
        { //if there is only one node in the list and its key value is equal to the key parameter
            delete head;
            head=nullptr; //assign head pointer to be the nullptr (indication that list is empty)
            return true;
        }
        else if(head->key==key) //If the first node's key value is equal to the key parameter
        {
            Node* p=head; //create a temporary pointer to point to the head node
            Node* c=p->next; //create a temporary pointer to point to what is being pointed by the first node's next pointer
            c->prev=nullptr; //set the next node's previous pointer to the nullptr
            head=c; //make the second node the head
            delete p; //delete the first node
            return true;
        }
        else //if node whose key value is equal to the key parameter is in the middle or end of the list
        {
            Node* p=head; //create a pointer pointing to the head
            while(p!=nullptr) //traverse through linked list until the pointer goes past the end of the list
            {
                if(p->next!=nullptr) //if the node is not the last node
                {
                    if(p->next->key==key)
                    { //if the node following the node being pointed to has the same key as the parameter
                        Node* temp=p->next; //create a temporary pointer pointing to the node to be erased
                        p->next=temp->next; //current node's next pointer points to the next pointer of node to be erased
                        delete temp; //erase the node
                        if(p->next!=nullptr) //if the node following the node erased is not the nullptr
                            p->next->prev=p; //the node following the node erased will be linked to the node preceding the node erased via the previous pointer
                        return true;
                    }
                }
                p=p->next;
            }
        }
    }
    return false;
}

bool Map :: insert(const KeyType& key, const ValueType& value)
{ // If key is not equal to any key currently in the map, and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that either the key is already in the map, or the map has a fixed
    // capacity and is full).
    if(!contains(key)) //if map does not contain the key passed as an argument
    {
        if(head==nullptr) //if list is empty
        {
            Node* ptr=new Node; //create a new node and assign its key and value as per parameters
            ptr->key=key;
            ptr->value=value;
            head=ptr; //make head point to this new node and initialise its next and previous pointers to nullptr
            head->prev=nullptr;
            head->next=nullptr;
            return true;
        }
        //otherwise, for every other case
        Node* ptr; //create a new node and assign its key and value as per parameters
        ptr=new Node;
        ptr->key=key;
        ptr->value=value;
        Node* p=head; //create pointer to head of current map and traverse through the list until it reaches the last node
        while(p->next!=nullptr)
        {
            p=p->next;
        }
        p->next=ptr; //make the last node's next pointer point to the new node
        ptr->prev=p; //make the new node's previous pointer point to the last node
        ptr->next=nullptr; //make the new node's next pointer the nullptr (end of list)
        return true;
    }
    return false;
}

bool Map :: update(const KeyType &key, const ValueType &value)
{ // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // Otherwise, make no change to the map and return false.
    if(contains(key)) //Map contains key parameter
    {
        Node* p=head;
        while(p!=nullptr) //traverse through list until pointer goes past the end of the list
        {
            if(p->key==key)
            {
                p->value=value; //change value mapped to by key parameter to the value in the function parameter
                return true;
            }
            p=p->next;
        }
    }
    return false;
}

bool Map :: insertOrUpdate(const KeyType &key, const ValueType &value)
{ // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // If key is not equal to any key currently in the map and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that the key is not already in the map and the map has a fixed
    // capacity and is full).
    if(contains(key))
        return update(key,value);
    else
        return insert(key,value);
}

bool Map :: get(const KeyType &key, ValueType &value) const
{ //constant member function does not modify the data members of Map
    // If key is equal to a key currently in the map, set value to the
    // value in the map that that key maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.
    if(contains(key))
    {
        Node* p=head;
        while(p!=nullptr) //traverse through list until pointer goes past the end of the list
        {
            if(p->key==key)
            {
                value=p->value; //change value in the function's parameter to value mapped to by the given key
                return true;
            }
            p=p->next;
        }
    }
    return false;
}

bool Map :: get(int i, KeyType &key, ValueType &value) const
{ //constant member function does not modify the data members of Map
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of one of the key/value pairs in the map and return
    // true.  Otherwise, leave the key and value parameters unchanged and
    // return false.
    if(i>=0 && i<size())
    {
        Node* p=head;
        int c=i; //keeps track of which node we are on
        while(p!=nullptr && c>0) //traverse through list until you reach the i-th node or the pointer goes past the end of the list
        {
            p=p->next;
            c--;
        }
        if(p!=nullptr)
        { //change key and value parameters to the corresponding values of the i-th node
            key=p->key;
            value=p->value;
            return true;
        }
    }
    return false;
}

void Map :: swap(Map &other)
{ // Exchange the contents of this map with the other one.
    Node* p1=this->head; //create a pointer pointing to the head of this Map
    this->head=other.head; //make head of this Map point to head of other Map
    other.head=p1; //make head of other Map point to head of this Map
}

bool combine(const Map& m1, const Map& m2, Map& result) //Non member function
{ //Combine the 2 maps and store the combination in result Map regardless of what was in result before
    //If a key appears in exactly one of m1 and m2, then result must contain a pair consisting of that key and its corresponding value.
    //If a key appears in both m1 and m2, with the same corresponding value in both, then result must contain exactly one pair with that key and value.
    //If there exists a key that appears in both m1 and m2, but with different corresponding values, then this function returns false; if there is no key like this, the function returns true.
    //CHECK FOR ALIASING: If result refers to Map m1 or Map m2, modify said Map through result such that we get the correct result
   
    Map copy; //create a temporary Map
    bool r=true; //boolean variable to tell us if there exists a key that appears in both m1 and m2, but with different corresponding values
    int j=0;
    while(j<m1.size()) //traverse through every node of the list m1
    {
        KeyType k;
        ValueType v;
        m1.get(j,k,v); //get key and value of j-th node in variables k and v
        if(!copy.contains(k)) //if the resultant map doesn't already contain the current key
            copy.insert(k,v); //insert node
        else //if the resultant map already contains the current key
        {
            ValueType v1;
            copy.get(k,v1); //get the value being mapped to by current key in variable v1
            if(v1==v) //if v1 is the same as v (value earlier being mapped to by already existing key)
            {
                j++; //skip node without inserting
                continue;
            }
            else //if values of nodes with the same key are different
            {
                copy.erase(k); //delete old node with key being referred to and skip without inserting current node
                r=false; //set r to false
            }
        }
        j++;
    }
    int l=0;
    while(l<m2.size()) //traverse through every node of the list m2
    {
        KeyType k;
        ValueType v;
        m2.get(l,k,v); //get key and value of l-th node in variables k and v
        if(!copy.contains(k)) //if the resultant map doesn't already contain the current key
            copy.insert(k,v); //insert node
        else //if the resultant map already contains the current key
        {
            ValueType v1;
            copy.get(k,v1); //get the value being mapped to by current key in variable v1
            if(v1==v) //if v1 is the same as v (value earlier being mapped to by already existing key)
            {
                l++; //skip node without inserting
                continue;
            }
            else //if values of nodes with the same key are different
            {
                copy.erase(k); //delete old node with key being referred to and skip without inserting current node
                r=false; //set r to false
            }
        }
        l++;
    }
    result=copy; //assign contents of copy Map to the result Map parameter
    return r;
}

void subtract(const Map& m1, const Map& m2, Map& result) //Non-member function
{ //When this function returns, result must contain one copy of all the pairs in m1 whose keys don't appear in m2; it must not contain any other pairs. This must work regardless of what was in result before
    //CHECK FOR ALIASING: If result refers to Map m1 or Map m2, modify said Map through result such that we get the correct result
    Map copy; //Create temporary Map
    int j=0;
    while(j<m1.size()) //traverse through every node of the list m1
    {
        KeyType k;
        ValueType v;
        m1.get(j,k,v); //get key and value of j-th node in variables k and v
        if(!m2.contains(k)) //if m2 does not contain current key
            copy.insert(k,v); //insert current key/value pair to the temporary Map
        j++;
    }
    result=copy; //assign contents of copy Map to the result Map parameter
}
