#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <stdexcept>

using namespace std;

struct Group {
    char l;
    int num;
};

struct Key {
    string full_name;
    Group group;
    string dir_title;
    int s;
};

class HashTable {
private:
    struct HashEntry {
        Key key;
        bool is_occupied;
        bool is_deleted;
        HashEntry() : is_occupied(false), is_deleted(false) {}
    };

    vector<HashEntry> table;
    int table_size;
    int hash_function(const string& full_name, int s) const;
    int quadratic_probing(int hash, int step) const;
    int find_slot(const string& full_name, int s) const;

public:
    HashTable(int size);
    ~HashTable() {}
    void insert(const Key& key);
    void remove(const string& full_name, int s);
    int search(const string& full_name, int s, int& steps) const;
    void load_from_file(const string& filename, int n);
    void save_to_file(const string& filename) const;
};

#endif // HASHTABLE_H
