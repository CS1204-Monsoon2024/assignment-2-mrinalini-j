#include <iostream>
#include <vector>
using namespace std;

class HashTable {
private:
    vector<vector<pair<int, int>>> table;
    int capacity;
    const float loadFactorThreshold;
    int size;

    // Hash function
    int hashFunction(int key) {
        return key % capacity;
    }

public:
    // Constructor
    HashTable(int capacity) : capacity(capacity), loadFactorThreshold(0.75), size(0) {
        table.resize(capacity);
    }

    // Insert function, modified to insert only the key
    void insert(int key) {
        int hashIndex = hashFunction(key);

        // Check if the key already exists in the table
        for (auto &item : table[hashIndex]) {
            if (item.first == key) {
                item.second = key; // Update value if key already exists
                return;
            }
        }

        // If the key doesn't exist, insert the key and use the key as the value
        table[hashIndex].emplace_back(key, key);
        size++;
    }

    // Remove a key from the hash table
    void remove(int key) {
        int hashIndex = hashFunction(key);

        // Find the key in the bucket and remove it
        for (auto it = table[hashIndex].begin(); it != table[hashIndex].end(); ++it) {
            if (it->first == key) {
                table[hashIndex].erase(it);
                size--;
                return;
            }
        }
        cout << "Key " << key << " not found" << endl;
    }

    // Search for a key and return its value
    int search(int key) {
        int hashIndex = hashFunction(key);

        // Search for the key in the bucket
        for (auto &item : table[hashIndex]) {
            if (item.first == key) {
                return item.second;
            }
        }
        return -1; // Return -1 if key is not found
    }

    // Display the hash table
    void display() {
        for (int i = 0; i < capacity; i++) {
            cout << i << ": ";
            for (auto &item : table[i]) {
                cout << "(" << item.first << ", " << item.second << ") ";
            }
            cout << endl;
        }
    }
};


