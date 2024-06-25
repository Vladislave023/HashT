#include "HashTable.h"

HashTable::HashTable(int size) : table_size(size) {
    table.resize(size);
}

int HashTable::hash_function(const string& full_name, int s) const {
    int hash = 0;
    for (char ch : full_name) {
        hash = (hash * 31 + ch) % table_size;
    }
    hash = (hash * 31 + s) % table_size;
    return hash;
}

int HashTable::quadratic_probing(int hash, int step) const {
    return (hash + step * step) % table_size;
}

int HashTable::find_slot(const string& full_name, int s) const {
    int hash = hash_function(full_name, s);
    int step = 0;
    while (table[quadratic_probing(hash, step)].is_occupied && !table[quadratic_probing(hash, step)].is_deleted &&
        !(table[quadratic_probing(hash, step)].key.full_name == full_name && table[quadratic_probing(hash, step)].key.s == s)) {
        step++;
    }
    return quadratic_probing(hash, step);
}

void HashTable::insert(const Key& key) {
    int slot = find_slot(key.full_name, key.s);
    if (table[slot].is_occupied && !table[slot].is_deleted) {
        throw runtime_error("Duplicate key insertion attempted");
    }
    table[slot].key = key;
    table[slot].is_occupied = true;
    table[slot].is_deleted = false;
}

void HashTable::remove(const string& full_name, int s) {
    int slot = find_slot(full_name, s);
    if (table[slot].is_occupied && !table[slot].is_deleted && table[slot].key.full_name == full_name && table[slot].key.s == s) {
        table[slot].is_deleted = true;
    }
}

int HashTable::search(const string& full_name, int s, int& steps) const {
    int hash = hash_function(full_name, s);
    int step = 0;
    steps = 0;
    while (steps <= table_size && table[quadratic_probing(hash, step)].is_occupied) {
        steps++;
        if (!table[quadratic_probing(hash, step)].is_deleted &&
            table[quadratic_probing(hash, step)].key.full_name == full_name &&
            table[quadratic_probing(hash, step)].key.s == s) {
            return quadratic_probing(hash, step);
        }
        step++;
    }
    return -1;
}

void HashTable::load_from_file(const string& filename, int n) {
    ifstream in(filename);
    if (in.is_open()) {
        string line;
        int count = 0;
        while (getline(in, line) && count < n) {
            stringstream ss(line);
            Key k;
            string group;

            getline(ss, k.full_name, ' ');
            ss >> group;
            k.group.l = group[0];
            k.group.num = stoi(group.substr(1));
            ss >> k.dir_title;
            ss >> k.s;

            insert(k);
            count++;
        }
        in.close();
    }
    else {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
}

void HashTable::save_to_file(const string& filename) const {
    ofstream out(filename);
    if (out.is_open()) {
        for (size_t i = 0; i < table.size(); ++i) {
            const auto& entry = table[i];
            if (entry.is_occupied) {
                out << entry.key.full_name << " " << entry.key.group.l << entry.key.group.num << " "
                    << entry.key.dir_title << " " << entry.key.s << " "
                    << (entry.is_deleted ? "deleted" : "active") << "\n";
            }
        }
        out.close();
    }
    else {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
}
