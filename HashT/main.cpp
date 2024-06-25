#include "HashTable.h"
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

void read_file(vector<Key>& arr, const string& filename, int n) {
    ifstream in(filename);
    if (in.is_open()) {
        string line;
        int count = 0;
        while (getline(in, line) && count < n) {
            stringstream ss(line);
            Key k;

            ss >> k.last_name >> k.first_name >> k.middle_name;

            string group;
            ss >> group;
            k.group.l = group[0];
            k.group.num = stoi(group.substr(1));

            ss >> k.dir.title;

            if (ss.fail()) {
                cerr << "Error parsing line: " << line << endl;
                continue;
            }

            string last_number;
            ss >> last_number;
            k.s = stoi(last_number);

            arr.push_back(k);
            count++;
        }
        in.close();
    }
    else {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
}

int main() {
    setlocale(LC_ALL, "ru");
    int n; // ���������� ����� ��� ���������
    string full_name;
    int unique_number; // ���������� ����� ��� ������

    cout << "������� ���������� ����� ��� ���������: ";
    cin >> n;
    cin.ignore(); // ����� ������� ������ ����� ������ �� ������ ����� ����� n
    cout << "������� ������ ��� ��� ������ (������� ��� ��������): ";
    getline(cin, full_name);
    cout << "������� ���������� ����� ��� ������: ";
    cin >> unique_number;

    vector<Key> arr;
    read_file(arr, "input.txt", n);

    HashTable ht(n * 2); // �������� ������� � �������

    // �������� ������ � ���-�������
    for (const auto& item : arr) {
        if (!ht.insert(item)) {
            cerr << "������ ���������� �������� � ������ " << item.s << endl;
        }
    }

    // ������������ ����� ��� ������
    string search_key = full_name + " " + to_string(unique_number);

    // ����� ��������
    int steps = 0;
    auto start = high_resolution_clock::now();
    int index = ht.search(search_key, steps);
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;

    if (index != -1) {
        cout << "���� ������ � ������: " << index << " �� " << steps << " �����.\n";
    }
    else {
        cout << "���� �� ������.\n";
    }

    cout << "����� ������: " << fixed << setprecision(6) << diff.count() << " s\n";

    // ���������� ���-������� � ����
    ht.save_to_file("output.txt");

    return 0;
}
