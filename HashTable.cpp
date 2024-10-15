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

    // Resize the hash table when load factor exceeds the threshold
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

    // Check if a number is prime
    bool isPrime(int n) {
        if (n <= 1) return false;
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) return false;
        }
        return true;
    }

    // Get the next prime number greater than or equal to n
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

    // Insert a key-value pair into the hash table
    void insert(int key, int value) {
        if ((float)numElements / m >= loadFactorThreshold) {
            resize();
        }

        int index = hash(key);
        int i = 0;

        // Quadratic probing to find an empty or deleted slot
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

    // Search for a key in the hash table and return its value
    int search(int key) {
        int index = hash(key);
        int i = 0;

        // Quadratic probing to find the key
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

    // Delete a key-value pair from the hash table
    void erase(int key) {
        int index = hash(key);
        int i = 0;

        // Quadratic probing to find the key
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

    // Print the contents of the hash table for debugging
    void printTable() {
        for (int i = 0; i < m; i++) {
            if (table[i].isOccupied && !table[i].isDeleted) {
                cout << "Index " << i << ": Key = " << table[i].key << ", Value = " << table[i].value << endl;
            } else if (table[i].isDeleted) {
                cout << "Index " << i << ": Deleted" << endl;
            } else {
                cout << "Index " << i << ": Empty" << endl;
            }
        }
    }
};

int main() {
    HashTable ht(7);  // Initial size is 7 (a prime number)

    ht.insert(10, 100);
    ht.insert(20, 200);
    ht.insert(30, 300);
    ht.insert(40, 400);
    ht.insert(50, 500);
    ht.insert(60, 600);

    ht.printTable();

    cout << "\nSearch for key 30: " << ht.search(30) << endl;
    cout << "Search for key 50: " << ht.search(50) << endl;

    ht.erase(30);
    cout << "\nAfter deleting key 30:\n";
    ht.printTable();

    return 0;
}


