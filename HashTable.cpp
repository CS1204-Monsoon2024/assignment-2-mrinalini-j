#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class HashTable {
private:
    struct Entry {
        int key;
        int value;
        bool isDeleted;  // To handle lazy deletion
        
        Entry() : key(-1), value(0), isDeleted(false) {}  // Default entry
    };

    vector<Entry> table;    // The hash table
    int size;               // Current number of entries
    int capacity;           // Maximum capacity before resizing
    const float loadFactorThreshold = 0.8; // Threshold for resizing

    // Hash function: h(k) = k % m
    int hash(int key) const {
        return key % capacity;
    }

    // Resize the table when the load factor exceeds the threshold
    void resize() {
        int oldCapacity = capacity;
        capacity = capacity * 2; // Double the size
        vector<Entry> oldTable = table;  // Save the old table

        table.clear();
        table.resize(capacity);   // Create a new table with increased size
        size = 0;  // Reset size to 0, we will re-insert existing entries

        for (int i = 0; i < oldCapacity; i++) {
            if (oldTable[i].key != -1 && !oldTable[i].isDeleted) {
                insert(oldTable[i].key, oldTable[i].value);  // Rehash the entries
            }
        }
    }

public:
    // Constructor: initialize hash table with initial capacity
    HashTable(int initialCapacity = 8) : capacity(initialCapacity), size(0) {
        table.resize(capacity);
    }

    // Insert a key-value pair into the hash table
    void insert(int key, int value) {
        if (float(size) / capacity > loadFactorThreshold) {
            resize();  // Resize if load factor exceeds threshold
        }

        int hashIndex = hash(key);  // Calculate initial hash index
        int i = 0;

        while (table[(hashIndex + i * i) % capacity].key != -1 && 
               !table[(hashIndex + i * i) % capacity].isDeleted) {
            if (table[(hashIndex + i * i) % capacity].key == key) {
                // Update existing key
                table[(hashIndex + i * i) % capacity].value = value;
                return;
            }
            i++;
        }

        // Insert new key-value pair
        int insertIndex = (hashIndex + i * i) % capacity;
        table[insertIndex].key = key;
        table[insertIndex].value = value;
        table[insertIndex].isDeleted = false;
        size++;
    }

    // Search for a key in the hash table and return its value
    int search(int key) const {
        int hashIndex = hash(key);
        int i = 0;

        while (table[(hashIndex + i * i) % capacity].key != -1) {
            int probeIndex = (hashIndex + i * i) % capacity;
            if (table[probeIndex].key == key && !table[probeIndex].isDeleted) {
                return table[probeIndex].value;
            }
            i++;
        }

        return -1;  // Key not found
    }

    // Delete a key from the hash table
    void remove(int key) {
        int hashIndex = hash(key);
        int i = 0;

        while (table[(hashIndex + i * i) % capacity].key != -1) {
            int probeIndex = (hashIndex + i * i) % capacity;
            if (table[probeIndex].key == key && !table[probeIndex].isDeleted) {
                table[probeIndex].isDeleted = true;  // Lazy deletion
                size--;
                return;
            }
            i++;
        }

        cout << "Key not found." << endl;  // If key not found
    }

    // Display the hash table
    void display() const {
        for (int i = 0; i < capacity; i++) {
            if (table[i].key != -1 && !table[i].isDeleted) {
                cout << "Index " << i << ": Key = " << table[i].key 
                     << ", Value = " << table[i].value << endl;
            }
        }
    }
};

// Main function to demonstrate hash table operations
int main() {
    HashTable ht(8);

    // Insert values
    ht.insert(10, 100);
    ht.insert(20, 200);
    ht.insert(30, 300);
    ht.insert(25, 250);
    ht.insert(15, 150);
    ht.insert(5, 50);

    cout << "Hash Table after insertion:" << endl;
    ht.display();

    // Search for a key
    int searchKey = 25;
    cout << "Value for key " << searchKey << ": " << ht.search(searchKey) << endl;

    // Remove a key
    ht.remove(15);
    cout << "Hash Table after deleting key 15:" << endl;
    ht.display();

    return 0;
}

