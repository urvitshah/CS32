//
//  main.cpp
//  project 2
//
//  Created by Urvi Shah on 1/23/20.
//  Copyright © 2020 Urvi Shah. All rights reserved.
//

#include "Map.h"
#include <iostream>
using namespace std;

Map::Map() //constructs Map of size 0
{
    //points head's next and prev pointers at itself
    head = new Node;
    head->next = head;
    head->prev = head;
    m_size = 0;
}

Map::Map(const Map &src) //constructs a Map that is a copy of the map that is passed to the constructor
{
    //points head's next and prev pointers at itself
    head = new Node;
    head->next = head;
    head->prev = head;
    m_size = 0;
    
    //runs through src's linkedlist and inserts all of its Nodes into the current Map
    Node *tempSrc = src.head->next;
    for (int a = 0; a < src.m_size; a++)
    {
        insert(tempSrc->key, tempSrc->val);
        tempSrc = tempSrc->next;
    }
}

Map &Map::operator=(const Map &src) //changes current Map to have the same Nodes of the Map that is passed to the assignment operator
{
    //if the Maps equal each other, nothing is done
    if (&src == this)
        return *this;
    //deletes and deallocates the memory for all Node pointers in the current Map
    Node *temp = head->next;
    while (temp != head)
    {
        Node *n = temp->next;
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        delete temp;
        temp = n;
    }
    delete head; 
   
    //creates new current Map and inserts Nodes from src's linkedlist into current Map
    head = new Node;
    head->next = head;
    head->prev = head;
    Node *tempSrc = src.head->next;
    for (int a = 0; a < src.m_size; a++)
    {
        insert(tempSrc->key, tempSrc->val);
        tempSrc = tempSrc->next;
    }
    m_size = src.m_size;
    return *this;
}

Map::~Map() //deletes and deallocates the memory for all Node pointers in current Map
{
    Node *temp = head->next;
    
    //loops through every single Node in the Map and deletes each Node
    while (temp != head)
    {
        Node *n = temp->next;
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        delete temp;
        temp = n;
    }
    delete head;
    m_size = 0;
}

bool Map::empty() const //checks if Map is empty
{
    if (m_size==0)
        return true;
    return false;
}

int Map::size() const //returns number of Nodes in Map
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value) //adds a Node to the front of the linkedlist
{
    //does not insert a Node with a key that already exists
    if (contains(key))
        return false;
    
    //creates new Node based on parameters
    Node *p = new Node;
    p->key = key;
    p->val = value;
    
    //inserts Node into Map and adjusts prev and next pointers
    Node *temp = head->next;
    head->next = p;
    temp->prev = p;
    p->prev = head;
    p->next = temp;
    
    m_size++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value) //if the key exists in the map, this function updates the value to match the parameter
{
    //loops through every node in the Map
    Node *temp = head;
    temp = temp->next;
    while(temp != head)
    {
        //if the key is found within the map, the value of that node changes and the function exits and returns true
        if (temp->key == key)
        {
            temp->val = value;
            return true;
        }
        temp=temp->next;
    }
    //if the key is not found the function returns false
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) //if they key isn't found in the Map, insert the key into the Map and if they key is found in the Map, update the value to match the value passed in
{
    //loops through every node in the Map
    Node *temp = head;
    temp = temp->next;
    while(temp != head)
    {
        //if the key is found within the map, the value of that node changes and the function exits and returns true
        if (temp->key == key)
        {
            
            temp->val = value;
            return true;
        }
        temp=temp->next;
    }
    
    //if the key is not found the function creates a new Node based on parameters
    Node *p = new Node;
    p->key = key;
    p->val = value;
    
    //inserts Node into Map and adjusts prev and next pointers
    temp = head->next;
    head->next = p;
    temp->prev = p;
    p->prev = head;
    p->next = temp;
    m_size++;
    return true;
}

bool Map::erase(const KeyType& key) //removes given node from the Map
{
    //traverse through every node in the map
    Node *temp = head;
    temp = temp->next;
    while(temp != head)
    {
        //if the key is the found, pointers are adjusted to no longer point at current node and current node is deleted
        if (temp->key == key)
        {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            delete temp;
            m_size--;
            return true;
        }
        temp=temp->next;
    }
    //function returns false if key is not found
    return false;
}

bool Map::contains(const KeyType& key) const //returns true if key is found in one of the Nodes in the Map
{
    Node *temp = head;
    temp = temp->next;
    while(temp != head)
    {
        if (temp->key == key)
            return true;
        temp=temp->next;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const //updates number in value to match the value that corresponds with the given key
{
    //loops through nodes until finding a node with the given key and sets value equal to the corresponding value
    Node *temp = head;
    temp = temp->next;
    while(temp != head)
    {
        if (temp->key == key)
        {
            value = temp->val;
            return true;
        }
        temp=temp->next;
    }
    //returns false if key is not found in the map
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const //updates values in value and key to match the values that correspond with node at i
{
    if (i >= m_size || i < 0) //exits function if i is an invalid number
        return false;
    //loops through nodes until reaching the node at i and sets parameters equal to the values that correspond with that node
    Node *temp = head;
    for (int a = 0; a < i+1; a++)
    {
        temp = temp->next;
    }
    key = temp->key;
    value = temp->val;
    return true;
}

void Map::swap(Map& other) //swaps values in the current Map and the parameter
{
    //switches head pointers for each Map
    Node *temp = head;
    head = other.head;
    other.head = temp;
    
    //switches sizes
    int tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
}

bool combine(const Map& m1, const Map& m2, Map& result) //combines the values in m1 and m2 and adds them to result
{
    KeyType k;
    ValueType v = 0;
    bool combined = true;
    Map tempResult;
    
    //adds all values from Map m1 to temporary result Map
    for (int a = 0; a < m1.size(); a++)
    {
        m1.get(a,k,v);
        tempResult.insert(k,v);
    }
    
    //loops through values from Map m2
    for (int a = 0; a < m2.size(); a++)
    {
        m2.get(a,k,v);
        //checks if the Map already contains k
        if (tempResult.contains(k))
        {
            KeyType tempk = k;
            ValueType tempv;
            tempResult.get(tempk,tempv);
            //if the Map contains k and the two values are different, the node is deleted from the Map
            if (k == tempk && v != tempv)
            {
                tempResult.erase(k);
                combined = false;
            }
            //if the Map contains k but the two values are not different from each other, the key is left in the Map
        }
        else
            tempResult.insert(k,v); //adds value from m2 to the result Map
    }
    
    //sets final result equal to temporary result Map
    result = tempResult;
    return combined;
}

void reassign(const Map& m, Map& result)
{
    //returns result with an unchanged map if the size is 0 or 1
    if (m.size()==1 || m.size()==0)
    {
        result = m;
        return;
    }
    KeyType k1, k2;
    ValueType v1, v2;
    
    ValueType tempv;
    m.get(0,k1,tempv);
    
    //loop through nodes in the map
    for (int a = 0; a < m.size()-1; a++)
    {
        //insert each node into Map, result, with the value from the node that comes after it in Map m
        m.get(a,k1,v1);
        m.get(a+1,k2,v2);
        if (result.contains(k1))
            result.erase(k1);
        result.insert(k1,v2);
    }

    //Insert the last node into Map, result, with the value from the first node in Map, m
    if (result.contains(k2))
        result.erase(k2);
    result.insert(k2,tempv);
    
    //since result is not always empty, remove any nodes that are not present in m
    for (int a = 0; a < result.size(); a++)
    {
        result.get(a,k1,v1);
        if (!m.contains(k1))
        {
            result.erase(k1);
            a--;
        }
        
    }
}

void Map::dump() //prints out values in Map
{
    KeyType k;
    ValueType v;
    for (int a = 0; a < m_size; a++)
    {
        get(a,k,v);
        cerr << "Key: " << k << " Value: " << v << endl;
    }
}
