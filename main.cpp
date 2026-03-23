#include <iostream>
#include <windows.h>

#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <sstream>

using namespace std;

// [cite: 7]
struct Date {
    int day;
    int month;
    int year;
};

// [cite: 8]
struct Phone {
    int countryCode;
    int cityCode;
    long long number;
};

// [cite: 9]
enum Group { WORK, FAMILY, FRIENDS, OTHERS };

// [cite: 10]
struct Contact {
    string lastName; // [cite: 11]
    string firstName; // [cite: 11]
    string middleName; // [cite: 11]
    Phone phone; // [cite: 12]
    Date birthDate; // [cite: 13]
    string email; // [cite: 14]
    Group category; // [cite: 15]
};

// Вспомогательные функции для безопасного ввода [cite: 33, 34]
int getValidInt() {
    int value;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка ввода. Введите число: ";
    }
    return value;
}

long long getValidLongLong() {
    long long value;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка ввода. Введите число: ";
    }
    return value;
}

// Валидация Email 
bool isValidEmail(const string& email) {
    size_t atPos = email.find('@'); // [cite: 40]
    if (atPos == string::npos) return false;
    size_t dotPos = email.find('.', atPos); // [cite: 40]
    return dotPos != string::npos && dotPos > atPos + 1;
}

// Валидация телефона (код страны) [cite: 30]
bool isValidCountryCode(int code) {
    return code >= 1 && code <= 999;
}

// Перевод строки в нижний регистр для поиска
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return tolower(c); });
    return s;
}

// Форматирование группы
string groupToString(Group g) {
    switch(g) {
        case WORK: return "Работа";
        case FAMILY: return "Семья";
        case FRIENDS: return "Друзья";
        case OTHERS: return "Остальные";
        default: return "Неизвестно";
    }
}

// Функция добавления контакта [cite: 18, 42]
void addContact(vector<Contact>& db) { // [cite: 38]
    Contact c;
    cout << "Введите Фамилию: "; cin >> c.lastName;
    cout << "Введите Имя: "; cin >> c.firstName;
    cout << "Введите Отчество: "; cin >> c.middleName;

    cout << "Код страны (1-999): ";
    do {
        c.phone.countryCode = getValidInt();
        if (!isValidCountryCode(c.phone.countryCode)) cout << "Код должен быть от 1 до 999. Повторите: ";
    } while (!isValidCountryCode(c.phone.countryCode));

    cout << "Код города: "; c.phone.cityCode = getValidInt();
    cout << "Номер телефона (только цифры): "; c.phone.number = getValidLongLong();

    cout << "Дата рождения (день месяц год через пробел): ";
    c.birthDate.day = getValidInt();
    c.birthDate.month = getValidInt();
    c.birthDate.year = getValidInt();

    cout << "Email: ";
    do {
        cin >> c.email;
        if (!isValidEmail(c.email)) cout << "Некорректный формат почты. Нужен @ и точка. Повторите: ";
    } while (!isValidEmail(c.email));

    cout << "Группа (0-Работа, 1-Семья, 2-Друзья, 3-Другое): ";
    int grp;
    do {
        grp = getValidInt();
    } while (grp < 0 || grp > 3);
    c.category = static_cast<Group>(grp);

    db.push_back(c);
    cout << "Контакт успешно добавлен!\n";
}

// Вывод одного контакта с форматированием [cite: 32, 43]
void printContactRow(const Contact& c) {
    string dLast = c.lastName;
    if (dLast.length() > 15) { // [cite: 40]
        dLast = dLast.substr(0, 15) + "..."; // [cite: 40]
    }
    
    cout << left << setw(20) << dLast 
         << setw(15) << c.firstName 
         << setw(15) << c.middleName
         << "+" << c.phone.countryCode << "(" << c.phone.cityCode << ")" << c.phone.number << "\t"
         << right << setfill('0') << setw(2) << c.birthDate.day << "." 
         << setw(2) << c.birthDate.month << "." << setfill(' ') << setw(4) << left << c.birthDate.year << "\t"
         << setw(20) << c.email 
         << setw(10) << groupToString(c.category) << "\n";
}

// Вывод всех контактов [cite: 19, 42]
void viewAllContacts(const vector<Contact>& db) {
    if (db.empty()) {
        cout << "База контактов пуста.\n";
        return;
    }
    cout << string(110, '-') << "\n";
    cout << left << setw(20) << "Фамилия" << setw(15) << "Имя" << setw(15) << "Отчество" << setw(20) << "Телефон" << setw(15) << "ДР" << setw(20) << "Email" << "Группа\n";
    cout << string(110, '-') << "\n";

    for (const auto& c : db) {
        printContactRow(c);
    }
    cout << string(95, '-') << "\n";
}

// Поиск по нескольким ключевым словам [cite: 20, 35, 42]
void searchContacts(const vector<Contact>& db) {
    cin.ignore(10000, '\n');
    cout << "Введите ключевые слова для поиска (через пробел): ";
    string input;
    getline(cin, input);
    
    vector<string> keywords;
    stringstream ss(input);
    string kw;
    while (ss >> kw) {
        keywords.push_back(toLower(kw));
    }

    bool foundAny = false;
    for (const auto& c : db) {
        string fName = toLower(c.firstName);
        string lName = toLower(c.lastName);
        bool match = false;
        
        for (const auto& k : keywords) {
            if (fName.find(k) != string::npos || lName.find(k) != string::npos) { // [cite: 40]
                match = true;
                break;
            }
        }
        
        if (match) {
            if (!foundAny) {
                cout << "\nНайденные контакты:\n";
                cout << string(95, '-') << "\n";
            }
            printContactRow(c);
            foundAny = true;
        }
    }
    if (!foundAny) cout << "Контакты не найдены.\n";
}

// Удаление контакта по фамилии [cite: 21, 42]
void deleteContact(vector<Contact>& db) {
    cout << "Введите точную фамилию для удаления: ";
    string target;
    cin >> target;
    
    auto it = remove_if(db.begin(), db.end(), [&](const Contact& c) {
        return toLower(c.lastName) == toLower(target);
    });
    
    if (it != db.end()) {
        db.erase(it, db.end());
        cout << "Контакт(ы) удален(ы).\n";
    } else {
        cout << "Контакт не найден.\n";
    }
}

// Редактирование контакта [cite: 22]
void editContact(vector<Contact>& db) {
    cout << "Введите точную фамилию для редактирования: ";
    string target;
    cin >> target;
    
    for (auto& c : db) {
        if (toLower(c.lastName) == toLower(target)) {
            cout << "Контакт найден. Введите новые данные.\n";
            cout << "Новый Email: ";
            do {
                cin >> c.email;
                if (!isValidEmail(c.email)) cout << "Некорректный формат почты. Повторите: ";
            } while (!isValidEmail(c.email));
            cout << "Данные обновлены.\n";
            return;
        }
    }
    cout << "Контакт не найден.\n";
}

// Сортировка контактов [cite: 23, 42]
void sortContacts(vector<Contact>& db) {
    cout << "1. По алфавиту (Фамилия + Имя)\n2. По дате рождения\nВыбор: "; // [cite: 24, 25]
    int choice = getValidInt();
    
    if (choice == 1) {
        sort(db.begin(), db.end(), [](const Contact& a, const Contact& b) {
            if (a.lastName == b.lastName) return a.firstName < b.firstName;
            return a.lastName < b.lastName;
        });
        cout << "Отсортировано по алфавиту.\n";
    } else if (choice == 2) {
        sort(db.begin(), db.end(), [](const Contact& a, const Contact& b) {
            if (a.birthDate.year != b.birthDate.year) return a.birthDate.year < b.birthDate.year;
            if (a.birthDate.month != b.birthDate.month) return a.birthDate.month < b.birthDate.month;
            return a.birthDate.day < b.birthDate.day;
        });
        cout << "Отсортировано по дате рождения.\n";
    }
}

// Доп. функция 1: Статистика по группам [cite: 27]
void showStats(const vector<Contact>& db) {
    int w = 0, f = 0, fr = 0, o = 0;
    for (const auto& c : db) {
        if (c.category == WORK) w++;
        else if (c.category == FAMILY) f++;
        else if (c.category == FRIENDS) fr++;
        else o++;
    }
    cout << "--- Статистика по группам ---\n";
    cout << "Работа: " << w << "\nСемья: " << f << "\nДрузья: " << fr << "\nОстальные: " << o << "\n";
}

// Доп. функция 2: Очистка базы [cite: 27]
void clearDatabase(vector<Contact>& db) {
    cout << "Вы уверены? (1 - Да, 0 - Нет): ";
    if (getValidInt() == 1) {
        db.clear();
        cout << "База полностью очищена.\n";
    }
}

// Файловые операции [cite: 26, 39]
void saveToFile(const vector<Contact>& db) {
    ofstream out("database.txt");
    for (const auto& c : db) {
        out << c.lastName << " " << c.firstName << " " << c.middleName << " "
            << c.phone.countryCode << " " << c.phone.cityCode << " " << c.phone.number << " "
            << c.birthDate.day << " " << c.birthDate.month << " " << c.birthDate.year << " "
            << c.email << " " << static_cast<int>(c.category) << "\n";
    }
    out.close();
}

void loadFromFile(vector<Contact>& db) {
    ifstream in("database.txt");
    if (!in.is_open()) return;
    
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        vector<string> words;
        string word;
        while (ss >> word) {
            words.push_back(word);
        }
        
        if (words.size() == 11) {
            Contact c;
            c.lastName = words[0];
            c.firstName = words[1];
            c.middleName = words[2];
            try {
                c.phone.countryCode = stoi(words[3]);
                c.phone.cityCode = stoi(words[4]);
                c.phone.number = stoll(words[5]);
                c.birthDate.day = stoi(words[6]);
                c.birthDate.month = stoi(words[7]);
                c.birthDate.year = stoi(words[8]);
                c.email = words[9];
                c.category = static_cast<Group>(stoi(words[10]));
                db.push_back(c);
            } catch (...) {}
        } else if (words.size() == 10) { // Fallback for old format without middleName
            Contact c;
            c.lastName = words[0];
            c.firstName = words[1];
            c.middleName = "---"; // Placeholder
            try {
                c.phone.countryCode = stoi(words[2]);
                c.phone.cityCode = stoi(words[3]);
                c.phone.number = stoll(words[4]);
                c.birthDate.day = stoi(words[5]);
                c.birthDate.month = stoi(words[6]);
                c.birthDate.year = stoi(words[7]);
                c.email = words[8];
                c.category = static_cast<Group>(stoi(words[9]));
                db.push_back(c);
            } catch (...) {}
        }
    }
    in.close();
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    vector<Contact> db; // [cite: 38]



    loadFromFile(db); // [cite: 26]
    
    int choice;
    do { // [cite: 41]
        cout << "\n--- Информационная система «Smart-Contacts» ---\n";
        cout << "1. Добавить контакт\n"; // [cite: 18]
        cout << "2. Просмотр всех записей\n"; // [cite: 19]
        cout << "3. Поиск по маске\n"; // [cite: 20]
        cout << "4. Удаление контакта\n"; // [cite: 21]
        cout << "5. Редактирование\n"; // [cite: 22]
        cout << "6. Сортировка\n"; // [cite: 23]
        cout << "7. Статистика по группам (доп)\n"; // [cite: 27]
        cout << "8. Очистить базу (доп)\n"; // [cite: 27]
        cout << "0. Выход\n";
        cout << "Выбор: ";
        
        choice = getValidInt();
        
        switch (choice) { // [cite: 41]
            case 1: addContact(db); break;
            case 2: viewAllContacts(db); break;
            case 3: searchContacts(db); break;
            case 4: deleteContact(db); break;
            case 5: editContact(db); break;
            case 6: sortContacts(db); break;
            case 7: showStats(db); break;
            case 8: clearDatabase(db); break;
            case 0: saveToFile(db); cout << "Данные сохранены. Выход...\n"; break; // [cite: 26]
            default: cout << "Неверный пункт меню.\n";
        }
    } while (choice != 0);

    return 0;
}