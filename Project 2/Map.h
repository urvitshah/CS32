//
//  Map.h
//  project 2
//
//  Created by Urvi Shah on 1/23/20.
//  Copyright © 2020 Urvi Shah. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <string>

using KeyType = std::string;
using ValueType = double;

class Map
{
public:
    Map();
    Map(const Map &src);
    Map &operator=(const Map &src);
    ~Map();
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    void dump();

private:
    int m_size;
    struct Node
    {
        KeyType key;
        ValueType val;
        Node *next;
        Node *prev;
    };
    Node *head;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void reassign(const Map& m, Map& result);

#endif /* Map_h */
