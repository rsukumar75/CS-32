#include "CarMap.h"
#include <iostream>
#include <cassert>
using namespace std;

 int main()
{
    CarMap m1;
    assert(m1.fleetSize()==0);
    m1.addCar("H12345");
    assert(m1.gas("H12345")==0);
    assert(!m1.addCar("H12345"));
    assert(m1.addGas("H12345", 50));
    assert(m1.gas("H12345")==50);
    assert(!m1.addGas("L4321", 25));
    assert(!m1.addGas(("H12345"), -10));
    assert(m1.useGas("H12345",25));
    assert(m1.gas("H12345")==25);
    assert(!m1.useGas("L4321", 12));
    assert(!m1.useGas("H12345", -10));
    assert(!m1.useGas("H12345", 30));
    m1.addCar("L43210");
    m1.addGas("L43210", 100);
    m1.addCar("B78912");
    m1.useGas("L43210", 43);
    m1.print();
    cout << "Passed all tests" << endl;
} 

