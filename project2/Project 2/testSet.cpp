//// testSet.cpp
//
//#include "Set.h"
//#include <cassert>
//#include <iostream>
//
void test() {
//    Set s1;
//    assert(s1.size() == 0); // tests whether size() works
//    assert(s1.empty()); // tests whether empty works
//    assert(s1.insert(1)); // tests whether insert works
//    assert(s1.contains(1)); // tests whether contains works
//    assert(s1.size() == 1); // tests whether size() still works
//    assert(!s1.insert(1)); // tests whether insert returns false if the value is already in the set
//    assert(s1.insert(2));
//    assert(s1.size() == 2);
//    assert(s1.erase(1)); // tests whether erase works
//    assert(s1.size() == 1);
//    assert(!s1.contains(1)); // tests whether contains works if you've removed a value
//    
//    // tests for the get function
//    Set s2;
//    ItemType value;
//    s2.insert(4);
//    s2.insert(5);
//    assert(s2.get(0, value));
//    assert(value == 4);
//    assert(s2.get(1, value));
//    assert(value == 5);
//    
//    //tests for the swap function
//    s1.swap(s2);
//    ItemType value2;
//    assert(s2.size() == 1);
//    assert(s2.contains(2));
//    assert(s1.size() == 2);
//    assert(s1.contains(4));
//    assert(s1.contains(5));
//    // checking if the two swapped sets are in the correct order
//    assert(s2.get(0, value2));
//    assert(value2 == 2);
//    assert(s1.get(1, value2));
//    assert(value2 == 5);
//    assert(s1.get(0, value2));
//    assert(value2 == 4);
//    
//    // tests for unite
//    Set s3;
//    unite(s1, s2, s3);
//    assert(s3.size() == 3);
//    assert(s3.contains(2));
//    assert(s3.contains(4));
//    assert(s3.contains(5));
//    s1.insert(2); // checking if the same members don't get repeated
//    assert(s3.size() == 3);
//    assert(s3.contains(2));
//    assert(s3.contains(4));
//    assert(s3.contains(5));
//    
//    // tests for subtract
//    Set s4;
//    s2.insert(5);
//    subtract(s1, s2, s4);
//    assert(s4.size() == 1);
//    assert(s4.contains(4));
}




