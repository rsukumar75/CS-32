#include "newMap.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;


int main()
{
    Map a(1000);   // a can hold at most 1000 key/value pairs
    Map b(5);      // b can hold at most 5 key/value pairs
    Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
    KeyType k1[6] = { "Rishab","Shivam","Athu","Ammar","Niranjan","Malay"};
    ValueType v1  = 32.43;
    
    // No failures inserting pairs with 5 distinct keys into b
    for (int n = 0; n < 5; n++)
        assert(b.insert(k1[n], v1));
    
    // Failure if we try to insert a pair with a sixth distinct key into b
    assert(!b.insert(k1[5], v1));
    
    // When two Maps' contents are swapped, their capacities are swapped
    // as well:
    a.swap(b);
    assert(!a.insert(k1[5], v1)  &&  b.insert(k1[5], v1));

    Map m1(5);      // b can hold at most 5 key/value pairs
    Map m2;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
    assert(m1.empty());
    ValueType v = -1234.5;
    assert( !m1.get("abc", v)  &&  v == -1234.5); // v unchanged by get failure
    m1.insert("xyz", 9876.5);
    assert(m1.size() == 1);
    KeyType k = "hello";
    assert(m1.get(0, k, v)  &&  k == "xyz"  &&  v == 9876.5);
    assert(m2.empty());
    m2.insert("Cool",987.65);
    m2.insert("Boring",432.10);
    assert(m2.size()==2);
    assert(!m2.insert("Cool",532.1));
    ValueType test=12.3;
    KeyType test2="Check";
    m2.insert("Gas",test);
    assert(m2.update("Gas",121.21) && m2.get("Gas",test) && test==121.21);
    assert(m2.contains("Cool"));
    m2.swap(m1);
    assert(m1.size()==3 && m2.size()==1 && m1.contains("Cool") && m1.contains("Gas") && m1.contains("Boring") && m2.contains("xyz"));
    assert(m2.insertOrUpdate("Flatulan",32.32) && m2.get("Flatulan",test) && test==32.32);
    assert(m2.insertOrUpdate("xyz",44.44) && m2.get(0,test2,test) && test2=="xyz" && test==44.44);
    assert(!m2.erase("Updated"));
    assert(m2.erase("xyz") && m2.size()==1 && !m2.contains("xyz") && m2.get(0,test2,test) && test2=="Flatulan" && test == 32.32);
    cout << "Passed all tests" << endl;

}


