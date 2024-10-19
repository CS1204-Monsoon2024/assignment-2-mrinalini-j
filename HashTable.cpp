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

    // Hash function to map keys
    int hash(int key) {
        return key % size;
    }

    // Quadratic probing function
    int quadraticProbe(int key, int i) {
        return (hash(key) + i * i) % size;
    }

    // Check if a number is prime
    bool isPrime(int num) {
        if (num <= 1)
            return false;
        if (num == 2)
            return true;
        if (num % 2 == 0)
            return false;
        for (int i = 3; i * i <= num; i += 2) {
            if (num % i == 0)
                return false;
        }
        return true;
    }

    // Get the next prime number greater than the current number
    int nextPrime(int num) {
        while (!isPrime(num)) {
            num++;
        }
        return num;
    }

    // Resize and rehash the hash table
    void resize() {
        int newSize = nextPrime(2 * size);
        std::vector<HashEntry> newTable(newSize);

        // Rehash all current keys into the new table
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
    // Constructor for initializing the hash table
    HashTable(int initialSize) : size(nextPrime(initialSize)), count(0) {
        table.resize(size);
    }

    // Insert function using quadratic probing and handling deleted slots
    void insert(int key) {
        // Check for duplicate key
        if (search(key) != -1) {
            std::cout << "Duplicate key insertion is not allowed" << std::endl;
            return;
        }

        // Resize if necessary
        if ((double)(count + 1) / size >= loadFactorThreshold) {
            resize();
        }

        int i = 0;
        int index;
        int firstAvailableIndex = -1;

        while (i < size) {
            index = quadraticProbe(key, i);

            if (table[index].key == -1 && !table[index].isDeleted) {
                // Empty slot found
                if (firstAvailableIndex == -1) {
                    firstAvailableIndex = index;
                }
                break;  // No need to probe further
            } else if (table[index].isDeleted) {
                // Deleted slot found
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

    // Search function using quadratic probing
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

    // Remove function marking a key as deleted
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

    // Print the contents of the hash table
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
