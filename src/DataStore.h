#ifndef DATASTORE_H
#define DATASTORE_H

#include <iostream>
#include <string>
using namespace std;

// Template class — stores any type T using dynamic memory
// Satisfies: templates, dynamic memory, operator overloading requirements
template <class T>
class DataStore {
private:
    T**  data;
    int  count;
    int  capacity;

    void resize() {
        capacity *= 2;
        T** newData = new T*[capacity];
        for (int i = 0; i < count; i++) newData[i] = data[i];
        delete[] data;
        data = newData;
    }

public:
    DataStore(int initialCap = 10)
        : count(0), capacity(initialCap) {
        data = new T*[capacity];
    }

    // Copy constructor
    DataStore(const DataStore& other)
        : count(other.count), capacity(other.capacity) {
        data = new T*[capacity];
        for (int i = 0; i < count; i++)
            data[i] = new T(*other.data[i]);
    }

    // Assignment operator
    DataStore& operator=(const DataStore& other) {
        if (this != &other) {
            for (int i = 0; i < count; i++) delete data[i];
            delete[] data;
            count    = other.count;
            capacity = other.capacity;
            data     = new T*[capacity];
            for (int i = 0; i < count; i++)
                data[i] = new T(*other.data[i]);
        }
        return *this;
    }

    ~DataStore() {
        for (int i = 0; i < count; i++) delete data[i];
        delete[] data;
    }

    void add(T* item) {
        if (count == capacity) resize();
        data[count++] = item;
    }

    bool removeById(const string& id) {
        for (int i = 0; i < count; i++) {
            if (data[i]->getId() == id) {
                delete data[i];
                for (int j = i; j < count - 1; j++) data[j] = data[j+1];
                count--;
                return true;
            }
        }
        return false;
    }

    T* findById(const string& id) const {
        for (int i = 0; i < count; i++)
            if (data[i]->getId() == id) return data[i];
        return nullptr;
    }

    // Operator[] for array-style access
    T* operator[](int index) const {
        if (index < 0 || index >= count) return nullptr;
        return data[index];
    }

    int getCount() const { return count; }

    void displayAll() const {
        if (count == 0) { cout << "  (no records)\n"; return; }
        for (int i = 0; i < count; i++) {
            cout << "  " << (i+1) << ". ";
            data[i]->displayInfo();
        }
    }
};

#endif
