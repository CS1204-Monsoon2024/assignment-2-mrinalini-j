#include "HashTable.cpp"
#include <iostream>
#include <vector>

class HashTable {
private:
    std::vector<int> table;
    int currentSize;
    int capacity;
    std::vector<bool> deleted;

    int hashFunction(int key) {
        return key % capacity;
    }

    void resizeTable() {
        int oldCapacity = capacity;
        capacity *= 2; // Double the size
        std::vector<int> oldTable = table;
        std::vector<bool> oldDeleted = deleted;

        table.clear();
        table.resize(capacity, -1);
        deleted.clear();
        deleted.resize(capacity, false);

        currentSize = 0; // Reset size

        for (int i = 0; i < oldCapacity; ++i) {
            if (oldTable[i] != -1 && !oldDeleted[i]) {
                insert(oldTable[i]); // Rehash existing keys
            }
        }
    }

public:
    HashTable(int size) {
        capacity = size;
        currentSize = 0;
        table.resize(capacity, -1); // Initialize table with -1 (indicating empty slots)
        deleted.resize(capacity, false); // Keep track of deleted slots
    }

    void insert(int key) {
        if ((float)currentSize / capacity >= 0.8) {
            resizeTable();
        }

        int index = hashFunction(key);
        int i = 0;

        // Quadratic probing
        while (table[(index + i * i) % capacity] != -1 && !deleted[(index + i * i) % capacity]) {
            i++;
        }

        index = (index + i * i) % capacity;
        table[index] = key;
        deleted[index] = false; // Mark slot as not deleted
        currentSize++;

        std::cout << "Insert " << key << ":\n";
        printTable();
    }

    int search(int key) {
        int index = hashFunction(key);
        int i = 0;

        while (table[(index + i * i) % capacity] != -1) {
            int probeIndex = (index + i * i) % capacity;
            if (table[probeIndex] == key && !deleted[probeIndex]) {
                return probeIndex; // Key found
            }
            i++;
        }

        return -1; // Key not found
    }

    void remove(int key) {
        int index = search(key);
        if (index != -1) {
            deleted[index] = true;
            currentSize--;
            std::cout << "Remove " << key << ":\n";
            printTable();
        }
    }

    void printTable() {
        for (int i = 0; i < capacity; ++i) {
            std::cout << "[" << i << "]: ";
            if (table[i] == -1) {
                std::cout << "(empty)";
            } else if (deleted[i]) {
                std::cout << "(deleted)";
            } else {
                std::cout << table[i];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

// Test the HashTable
int main() {
    int initialSize = 7; 
    HashTable ht(initialSize);

    // Example test case
    ht.insert(1);    // Insert 1
    ht.insert(6);    // Insert 6
    ht.insert(15);   // Insert 15
    ht.insert(25);   // Insert 25
    ht.remove(15);   // Remove 15
    ht.insert(29);   // Insert 29

    int find = ht.search(22); // Search for 22
    std::cout << "Search for 22:\n";
    std::cout << "Found at: " << find << std::endl;

    return 0;
}

