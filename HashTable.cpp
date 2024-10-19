#include <iostream>
#include <vector>
#include <cmath>

class HashTable {
private:
    struct HashEntry {
        int key;
        int value;
        bool isOccupied;
        bool isDeleted;

        HashEntry() : key(0), value(0), isOccupied(false), isDeleted(false) {}
    };

    std::vector<HashEntry> table;
    int size;
    int count;
    const double loadFactorThreshold = 0.8;

    // Hash function to map keys
    int hash(int key) const {
        return key % size;
    }

    // Quadratic probing function
    int quadraticProbe(int key, int i) const {
        return (hash(key) + i * i) % size;
    }

    // Check if a number is prime
    bool isPrime(int num) const {
        if (num <= 1) return false;
        if (num == 2) return true;
        if (num % 2 == 0) return false;
        for (int i = 3; i * i <= num; i += 2) {
            if (num % i == 0) return false;
        }
        return true;
    }

    // Get the next prime number greater than the current number
    int nextPrime(int num) const {
        while (!isPrime(num)) {
            num++;
        }
        return num;
    }

    // Resize and rehash the hash table
    void resize() {
        int oldSize = size;
        int newSize = nextPrime(2 * size);
        std::vector<HashEntry> oldTable = table;

        // Initialize the new table
        table = std::vector<HashEntry>(newSize);
        size = newSize;
        count = 0;

        // Rehash all current keys into the new table
        for (int i = 0; i < oldSize; i++) {
            if (oldTable[i].isOccupied && !oldTable[i].isDeleted) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
    }

public:
    // Constructor for initializing the hash table
    HashTable(int initialSize) : size(nextPrime(initialSize)), count(0) {
        table.resize(size);
    }

    // Insert function using quadratic probing and handling deleted slots
    void insert(int key, int value) {
        // Resize the table if the load factor threshold is exceeded
        if ((double)(count + 1) / size > loadFactorThreshold) {
            resize();
        }

        int i = 0;
        int index;
        int firstDeletedIndex = -1;  // Track the first deleted index found

        while (i < size) {
            index = quadraticProbe(key, i);

            if (!table[index].isOccupied) {
                if (table[index].isDeleted) {
                    if (firstDeletedIndex == -1) {
                        firstDeletedIndex = index;
                    }
                } else {
                    // Empty and never occupied
                    break;
                }
            } else if (table[index].isOccupied && table[index].key == key) {
                // Key already exists, update the value
                table[index].value = value;
                return;
            }
            i++;
        }

        if (firstDeletedIndex != -1) {
            index = firstDeletedIndex;
        }

        // Insert the new key-value pair
        table[index].key = key;
        table[index].value = value;
        table[index].isOccupied = true;
        table[index].isDeleted = false;
        count++;
    }

    // Search function using quadratic probing
    bool search(int key, int &value) const {
        int i = 0;
        int index;

        while (i < size) {
            index = (hash(key) + i * i) % size;

            if (!table[index].isOccupied) {
                if (!table[index].isDeleted) {
                    // Slot is empty and never occupied; key not present
                    return false;
                }
                // Slot was deleted; continue searching
            } else if (table[index].isOccupied && table[index].key == key) {
                value = table[index].value;
                return true;
            }
            i++;
        }
        return false;  // Key not found after probing all slots
    }

    // Remove function marking a key as deleted
    bool remove(int key) {
        int i = 0;
        int index;

        while (i < size) {
            index = (hash(key) + i * i) % size;

            if (!table[index].isOccupied) {
                if (!table[index].isDeleted) {
                    // Slot is empty and never occupied; key not present
                    return false;
                }
                // Slot was deleted; continue searching
            } else if (table[index].isOccupied && table[index].key == key) {
                // Mark as deleted
                table[index].isDeleted = true;
                table[index].isOccupied = false;
                count--;
                return true;
            }
            i++;
        }
        return false;  // Key not found
    }

    // Print the contents of the hash table
    void printTable() const {
        for (int i = 0; i < size; i++) {
            if (table[i].isOccupied && !table[i].isDeleted) {
                std::cout << "(" << table[i].key << ", " << table[i].value << ") ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }

    // Get current size of the table
    int getSize() const {
        return size;
    }

    // Get current count of elements
    int getCount() const {
        return count;
    }
};

