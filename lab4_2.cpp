#include <iostream>
#include <vector>
#include <string>
#include <clocale>
#include <utility>
#include <fcntl.h>
#include <io.h>

using namespace std;

int CodeError = 0; // 0 - ок, 1 - помилка (сутність не знайдена)

class Student {
public:
    wstring surname;
    wstring name;
    wstring patronymic;

    Student(wstring s = L"", wstring n = L"", wstring p = L"")
        : surname(s), name(n), patronymic(p) {}

    friend wostream& operator<<(wostream& out, const Student& st);

    friend wistream& operator>>(wistream& in, Student& st);


};

wostream& operator<<(wostream& out, const Student& st) {
    out << st.surname << L" " << st.name << L" " << st.patronymic;
    return out;
}

wistream& operator>>(wistream& in, Student& st) {
    in >> st.surname >> st.name >> st.patronymic;
    return in;
}

class AssocArray {
private:
    vector<pair<int, Student>> data;

public:
    // Додати елемент
    void add(int id, const Student& st) {
        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i].first == id) {
                data[i].second = st;
                return;
            }
        }
        data.push_back(make_pair(id, st));
    }

    // Перевантаження [] - доступ за індексом (id)
    Student operator[](int id) {
        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i].first == id) {
                CodeError = 0;
                return data[i].second;
            }
        }
        CodeError = 1;
        return Student(); // Повертає пустий об'єкт
    }

    // Перевантаження виклику функції
    Student operator()(int id) {
        return (*this)[id];
    }

    friend wostream& operator<<(wostream& out, const AssocArray& aa);
    friend wistream& operator>>(wistream& in, AssocArray& aa);

};
// Дружнє перевантаження виводу
wostream& operator<<(wostream& out, const AssocArray& aa) {
    for (size_t i = 0; i < aa.data.size(); ++i) {
        out << L"ID: " << aa.data[i].first << L" -> " << aa.data[i].second << endl;
    }
    return out;
}

// Дружнє перевантаження введення (вставка одного студента)
wistream& operator>>(wistream& in, AssocArray& aa) {
    int id;
    Student st;
    wcout << L" ID: " << endl;
    in >> id;

    wcout << L"  Прізвище: " << endl;
    in >> st.surname;

    wcout << L"  Ім'я: " << endl;
    in >> st.name;

    wcout << L"  По батькові: " << endl;
    in >> st.patronymic;
    aa.add(id, st);
    return in;
}
// Функція створення тестового набору
AssocArray createSampleData() {
    AssocArray aa;
    aa.add(1, Student(L"Іваненко", L"Іван", L"Іванович"));
    aa.add(2, Student(L"Петренко", L"Петро", L"Петрович"));
    aa.add(3, Student(L"Сидоренко", L"Сидір", L"Сидорович"));
    return aa;
}

int main() {
    setlocale(LC_ALL, "");
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    AssocArray aa = createSampleData();

    wcout << L"=== Вміст асоціативного масиву ===" << endl;
    wcout << aa;

    wcout << L"\n=== Перевірка доступу через operator[] ===" << endl;
    Student s1 = aa[2];
    if (!CodeError) wcout << L"ID 2: " << s1 << endl;
    else wcout << L"Студент з ID 2 не знайдений\n";

    Student s2 = aa[5];
    if (!CodeError) wcout << L"ID 5: " << s2 << endl;
    else wcout << L"Студент з ID 5 не знайдений (CodeError = " << CodeError << L")\n";

    wcout << L"\n=== Перевірка доступу через operator() ===" << endl;
    wcout << L"ID 1: " << aa(1) << endl;

    wcout << L"\n=== Тест введення нового студента ===" << endl;
    wcin >> aa;

    wcout << L"\n=== Оновлений список студентів ===" << endl;
    wcout << aa;

    return 0;
}
