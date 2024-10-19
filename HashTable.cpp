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
    double loadFactorThreshold = 0.8;

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
        int newSize = nextPrime(2 * size);
        std::vector<HashEntry> newTable(newSize);

        for (int i = 0; i < size; i++) {
            if (table[i].key != -1 && !table[i].isDeleted) {
                int newKey = table[i].key;
                int j = 0;
                int newIndex;
                do {
                    newIndex = (newKey % newSize + j * j) % newSize;
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
    HashTable(int initialSize) : size(nextPrime(initialSize)), count(0) {
        table.resize(size);
    }

    void insert(int key) {
        if (search(key) != -1) {
            std::cout << "Duplicate key insertion is not allowed" << std::endl;
            return;
        }

        if ((double)(count + 1) / size >= loadFactorThreshold) {
            resize();
        }

        int i = 0;
        int index;
        int firstAvailableIndex = -1;

        while (i < size) {
            index = quadraticProbe(key, i);

            if (table[index].key == -1 && !table[index].isDeleted) {
                if (firstAvailableIndex == -1) {
                    firstAvailableIndex = index;
                }
                break;  
            } else if (table[index].isDeleted) {
                if (firstAvailableIndex == -1) {
                    firstAvailableIndex = index;
                }
            }
            i++;
        }

        if (firstAvailableIndex != -1) {
            table[firstAvailableIndex].key = key;
            table[firstAvailableIndex].isDeleted = false;
            count++;
        } else {
            std::cout << "Max probing limit reached!" << std::endl;
        }
    }

    int search(int key) {
        int i = 0;
        int index;

        while (i < size) {
            index = quadraticProbe(key, i);

            if (table[index].key == -1 && !table[index].isDeleted) {
                return -1;  // Key not found
            } else if (table[index].key == key && !table[index].isDeleted) {
                return index;  // Key found
            }
            i++;
        }
        return -1;  // Key not found after full probing
    }

    void remove(int key) {
        int i = 0;
        int index;
        bool found = false;

        while (i < size) {
            index = quadraticProbe(key, i);

            if (table[index].key == -1 && !table[index].isDeleted) {
                break;  // Key not found
            } else if (table[index].key == key && !table[index].isDeleted) {
                table[index].isDeleted = true;
                count--;
                found = true;
                break;
            }
            i++;
        }

        if (!found) {
            std::cout << "Element not found" << std::endl;
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


