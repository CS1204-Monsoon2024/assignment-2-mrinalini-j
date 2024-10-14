#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class HashTable {
private:
    struct Entry {
        int key;
        int value;
        bool isActive; // To track active keys
        bool isDeleted; // To track deleted keys
        Entry() : key(0), value(0), isActive(false), isDeleted(false) {}
    };

    vector<Entry> table;  // Hash table to store entries
    int tableSize;        // Current size of the table
    int numElements;      // Number of active elements
    const double loadFactorThreshold = 0.8; // Load factor threshold

    // Hash function: h(k) = k mod m
    int hashFunction(int key) const {
        return key % tableSize;
    }

    // Resize the table when load factor exceeds the threshold
    void resize() {
        int oldSize = tableSize;
        tableSize = nextPrime(2 * tableSize); // Approximately double the table size
        vector<Entry> oldTable = table;

        // Reset table and element count
        table.clear();
        table.resize(tableSize);
        numElements = 0;

        // Rehash all the elements from the old table to the new table
        for (int i = 0; i < oldSize; ++i) {
            if (oldTable[i].isActive && !oldTable[i].isDeleted) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
    }

    // Check if the number is prime
    bool isPrime(int num) {
        if (num <= 1) return false;
        if (num == 2 || num == 3) return true;
        if (num % 2 == 0 || num % 3 == 0) return false;
        for (int i = 5; i * i <= num; i += 6) {
            if (num % i == 0 || num % (i + 2) == 0) return false;
        }
        return true;
    }

    // Find the next prime number greater than or equal to n
    int nextPrime(int n) {
        while (!isPrime(n)) ++n;
        return n;
    }

    // Quadratic probing to find an index for insertion or search
    int findPosition(int key) {
        int hashIndex = hashFunction(key);
        int i = 0;
        int index;

        while (true) {
            index = (hashIndex + i * i) % tableSize;
            if (!table[index].isActive || table[index].key == key) {
                return index;
            }
            i++;
            if (i == tableSize) {
                // Probing failed, table is full or something went wrong
                return -1;
            }
        }
    }

public:
    // Constructor to initialize hash table with given size
    HashTable(int initialSize = 11) : tableSize(initialSize), numElements(0) {
        table.resize(tableSize);
    }

    // Insert key-value pair into the hash table
    void insert(int key, int value) {
        if ((double)numElements / tableSize >= loadFactorThreshold) {
            resize();  // Resize the table if load factor exceeds the threshold
        }

        int index = findPosition(key);
        if (index == -1) {
            cout << "Hash table is full, cannot insert." << endl;
            return;
        }

        if (!table[index].isActive || table[index].isDeleted) {
            // Insert new key
            table[index].key = key;
            table[index].value = value;
            table[index].isActive = true;
            table[index].isDeleted = false;
            numElements++;
        } else {
            // Update existing key's value
            table[index].value = value;
        }
    }

    // Search for a key in the hash table and return its value
    int search(int key) {
        int index = findPosition(key);
        if (index != -1 && table[index].isActive && !table[index].isDeleted) {
            return table[index].value;
        }
        return -1;  // Key not found
    }

    // Delete a key from the hash table
    void remove(int key) {
        int index = findPosition(key);
        if (index != -1 && table[index].isActive && !table[index].isDeleted) {
            table[index].isDeleted = true;  // Mark the key as deleted
            numElements--;
        } else {
            cout << "Key not found, cannot delete." << endl;
        }
    }

    // Display the hash table (for debugging)
    void display() {
        for (int i = 0; i < tableSize; ++i) {
            if (table[i].isActive && !table[i].isDeleted) {
                cout << "[" << i << "]: (" << table[i].key << ", " << table[i].value << ")" << endl;
            } else {
                cout << "[" << i << "]: (empty)" << endl;
            }
        }
    }
};

int main() {
    HashTable ht(7);  // Create a hash table with initial size 7

    // Insert some key-value pairs
    ht.insert(10, 100);
    ht.insert(20, 200);
    ht.insert(30, 300);
    ht.insert(17, 170);
    ht.insert(23, 230);

    ht.display();  // Display the hash table

    cout << "Search for key 20: " << ht.search(20) << endl;  // Search for a key

    ht.remove(20);  // Remove a key
    cout << "After deleting key 20:" << endl;
    ht.display();  // Display after deletion

    return 0;
}
