#include <iostream>
#include <vector>
#include <cmath>

struct HashEntry {
    int key;
    bool isDeleted;

    HashEntry() : key(-1), isDeleted(false) {}
};

class HashTable {
    std::vector<HashEntry> table;
    int size;
    int count;
    double loadFactorThreshold;

public:
    HashTable(int initialSize = 7, double loadFactor = 0.5) : size(initialSize), count(0), loadFactorThreshold(loadFactor) {
        table.resize(size);
    }

    int hash(int key) {
        return key % size;
    }

    int quadraticProbe(int key, int i) {
        return (hash(key) + i * i) % size;
    }

    bool isPrime(int n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6)
            if (n % i == 0 || n % (i + 2) == 0)
                return false;
        return true;
    }

    int nextPrime(int n) {
        while (!isPrime(n)) {
            n++;
        }
        return n;
    }

    void resize() {
        int newSize = nextPrime(2 * size);
        std::vector<HashEntry> newTable(newSize);

        // Rehash all current keys into the new table
        for (int i = 0; i < size; i++) {
            if (table[i].key != -1 && !table[i].isDeleted) {
                int newKey = table[i].key;
                int j = 0;
                int newIndex;

                // Use quadratic probing for inserting into newTable
                do {
                    newIndex = (hash(newKey) + j * j) % newSize;  // Adjusted to use newSize
                    j++;
                } while (newTable[newIndex].key != -1); // Stop if we find an empty slot

                newTable[newIndex].key = newKey;
                newTable[newIndex].isDeleted = false;
            }
        }

        table = newTable;
        size = newSize;
    }

    void insert(int key) {
        if ((double)count / size >= loadFactorThreshold) {
            resize();
        }

        int i = 0;
        int index;
        do {
            index = quadraticProbe(key, i);
            i++;

            // If the key already exists, return without inserting
            if (table[index].key == key && !table[index].isDeleted) {
                return; // Key already exists
            }

            // If the slot is empty or deleted, we can insert
            if (table[index].key == -1 || table[index].isDeleted) {
                table[index].key = key;
                table[index].isDeleted = false;
                count++;
                return; // Exit after inserting
            }
        } while (i < size);
    }

    int search(int key) {
        int i = 0;
        int index;
        do {
            index = quadraticProbe(key, i);
            i++;
            // Stop if we reach an empty slot that was never occupied
            if (table[index].key == -1 && !table[index].isDeleted) {
                return -1;  // Key not found
            }
            // Return the index if we find the key
        } while (table[index].key != key && i < size);

        return (table[index].key == key) ? index : -1;  // Return index if found, else -1
    }

    void remove(int key) {
        int index = search(key);
        if (index != -1) {
            table[index].isDeleted = true;
            count--;
        }
    }

    void printTable() {
        for (int i = 0; i < size; i++) {
            if (table[i].key != -1 && !table[i].isDeleted) {
                std::cout << table[i].key << " ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
};

