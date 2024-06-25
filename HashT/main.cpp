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
    int n; // Количество строк для обработки
    string full_name;
    int unique_number; // Уникальное число для поиска

    cout << "Введите количество строк для обработки: ";
    cin >> n;
    cin.ignore(); // Чтобы удалить символ новой строки из буфера после ввода n
    cout << "Введите полное ФИО для поиска (Фамилия Имя Отчество): ";
    getline(cin, full_name);
    cout << "Введите уникальный номер для поиска: ";
    cin >> unique_number;

    vector<Key> arr;
    read_file(arr, "input.txt", n);

    HashTable ht(n * 2); // Создание таблицы с запасом

    // Загрузка данных в хэш-таблицу
    for (const auto& item : arr) {
        if (!ht.insert(item)) {
            cerr << "Ошибка добавления элемента с ключом " << item.s << endl;
        }
    }

    // Формирование ключа для поиска
    string search_key = full_name + " " + to_string(unique_number);

    // Поиск элемента
    int steps = 0;
    auto start = high_resolution_clock::now();
    int index = ht.search(search_key, steps);
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;

    if (index != -1) {
        cout << "Ключ найден в строке: " << index << " за " << steps << " шагов.\n";
    }
    else {
        cout << "Ключ не найден.\n";
    }

    cout << "Время поиска: " << fixed << setprecision(6) << diff.count() << " s\n";

    // Сохранение хэш-таблицы в файл
    ht.save_to_file("output.txt");

    return 0;
}
