#ifndef HASHTABLE_H
#define HASHTABLE_H

class HashTable {
public:
    HashTable(int size);    // Constructor
    void insert(int key, int value);    // Insert key-value pair
    void printTable() const;            // Print table
private:
    int* table;    // Pointer to the hash table
    int size;
    const float loadFactorThreshold = 0.75;  // Threshold for resizing
};

#endif // HASHTABLE_H

#include "HashTable.h"
#include <iostream>

// Constructor to initialize the hash table with given size
HashTable::HashTable(int size) : size(size) {
    table = new int[size];
    for (int i = 0; i < size; i++) {
        table[i] = -1;    // Initialize with -1 (empty slots)
    }
}

// Insert a key-value pair into the hash table
void HashTable::insert(int key, int value) {
    int index = key % size;
    table[index] = value;
}

// Print the hash table
void HashTable::printTable() const {
    for (int i = 0; i < size; i++) {
        if (table[i] != -1) {
            std::cout << "Index " << i << ": " << table[i] << std::endl;
        } else {
            std::cout << "Index " << i << ": " << "Empty" << std::endl;
        }
    }
}
