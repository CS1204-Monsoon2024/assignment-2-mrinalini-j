#include <iostream>
#include <vector>
#include <cmath>

class HashTable {
private:
    struct HashEntry {
        int key;
        bool isDeleted;

        HashEntry() : key(-1), isDeleted(false) {}
    };

    std::vector<HashEntry> table;
    int size;
    int count;
    const double loadFactorThreshold = 0.8;

    int hash(int key) {
        return key % size;
    }

    int quadraticProbe(int key, int i) {
        return (hash(key) + i * i) % size;
    }

    bool isPrime(int num) {
        if (num <= 1) return false;
        if (num == 2) return true;
        if (num % 2 == 0) return false;
        for (int i = 3; i * i <= num; i += 2) {
            if (num % i == 0) return false;
        }
        return true;
    }

    int nextPrime(int num) {
        while (!isPrime(num)) {
            num++;
        }
        return num;
    }

    void resize() {
        int newSize = nextPrime(2 * size); // Find the next prime number for the new size
        std::vector<HashEntry> newTable(newSize);

        // Rehash all current keys into the new table
        for (int i = 0; i < size; i++) {
            if (table[i].key != -1 && !table[i].isDeleted) {
                int newKey = table[i].key;
                int j = 0;
                int newIndex;
                do {
                    newIndex = (newKey + j * j) % newSize;
                    j++;
                } while (newTable[newIndex].key != -1);

                newTable[newIndex].key = newKey;
                newTable[newIndex].isDeleted = false;
            }
        }

        table = newTable;
        size = newSize;
    }

public:
    HashTable(int initialSize) {
        size = nextPrime(initialSize);
        table.resize(size);
        count = 0;
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
        } while (table[index].key != -1 && !table[index].isDeleted && table[index].key != key);

        if (table[index].key == -1 || table[index].isDeleted) {
            table[index].key = key;
            table[index].isDeleted = false;
            count++;
        }
    }

    int search(int key) {
        int i = 0;
        int index;
        do {
            index = quadraticProbe(key, i);
            i++;
            if (table[index].key == -1 && !table[index].isDeleted) {
                return -1;  // Key not found
            }
        } while (table[index].key != key && i < size);

        return (table[index].key == key) ? index : -1;  // Return index if found, else -1
    }

    void remove(int key) {
        int i = 0;
        int index;
        do {
            index = quadraticProbe(key, i);
            i++;
            if (table[index].key == -1 && !table[index].isDeleted) {
                return;  // Key not found, nothing to remove
            }
        } while (table[index].key != key && i < size);

        if (table[index].key == key) {
            table[index].isDeleted = true;
            count--;
        }
    }

    void printTable() {
        for (int i = 0; i < size; i++) {
            if (table[i].key != -1 && !table[i].isDeleted) {
                std::cout << i << " -> " << table[i].key << std::endl;
            } else {
                std::cout << i << " -> " << "Empty" << std::endl;
            }
        }
        std::cout << std::endl;
    }
};


