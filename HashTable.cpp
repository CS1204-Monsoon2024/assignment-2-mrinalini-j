#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class HashTable {
private:
    struct Entry {
        int key;
        int value;
        bool isOccupied;
        bool isDeleted;

        Entry() : key(0), value(0), isOccupied(false), isDeleted(false) {}
    };

    vector<Entry> table;
    int m;  // Current size of the hash table
    int numElements;
    const float loadFactorThreshold = 0.8;

    int hash(int key) {
        return key % m;
    }

    void resize() {
        int oldSize = m;
        m = nextPrime(2 * m); // Resize to the next prime approximately double the size
        vector<Entry> oldTable = table;
        table.clear();
        table.resize(m);
        numElements = 0;

        // Rehash all elements from the old table
        for (int i = 0; i < oldSize; i++) {
            if (oldTable[i].isOccupied && !oldTable[i].isDeleted) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
    }

    bool isPrime(int n) {
        if (n <= 1) return false;
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) return false;
        }
        return true;
    }

    int nextPrime(int n) {
        while (!isPrime(n)) {
            n++;
        }
        return n;
    }

public:
    HashTable(int size) : m(size), numElements(0) {
        table.resize(m);
    }

    void insert(int key, int value) {
        if ((float)numElements / m >= loadFactorThreshold) {
            resize();
        }

        int index = hash(key);
        int i = 0;

        while (table[(index + i * i) % m].isOccupied && !table[(index + i * i) % m].isDeleted) {
            i++;
            if (i == m) { // Avoid infinite loop in case the table is full
                return;
            }
        }

        int finalIndex = (index + i * i) % m;
        table[finalIndex].key = key;
        table[finalIndex].value = value;
        table[finalIndex].isOccupied = true;
        table[finalIndex].isDeleted = false;
        numElements++;
    }

    int search(int key) {
        int index = hash(key);
        int i = 0;

        while (table[(index + i * i) % m].isOccupied) {
            int finalIndex = (index + i * i) % m;
            if (table[finalIndex].key == key && !table[finalIndex].isDeleted) {
                return table[finalIndex].value;
            }
            i++;
            if (i == m) {
                break;  // Avoid infinite loop
            }
        }

        return -1;  // Key not found
    }

    void erase(int key) {
        int index = hash(key);
        int i = 0;

        while (table[(index + i * i) % m].isOccupied) {
            int finalIndex = (index + i * i) % m;
            if (table[finalIndex].key == key && !table[finalIndex].isDeleted) {
                table[finalIndex].isDeleted = true;
                numElements--;
                return;
            }
            i++;
            if (i == m) {
                return;  // Avoid infinite loop
            }
        }
    }

    void printTable() {
        for (int i = 0; i < m; i++) {
            if (table[i].isOccupied && !table[i].isDeleted) {
                cout << "Index " << i << ": Key = " << table[i].key << ", Value = " << table[i].value << endl;
            } else {
                cout << "Index " << i << ": Empty" << endl;
            }
        }
    }
};


