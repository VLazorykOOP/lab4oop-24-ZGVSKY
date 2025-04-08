#include <iostream>
#include <map>
#include <string>
#include <clocale>
int const MaxElem =1000;
using namespace std;

int CodeError = 0; // 0 - ок, 1 - помилка (сутність не знайдена)

class Student {
public:
    string surname;
    string name;
    string patronymic;

    Student(string s = "", string n = "", string p = "")
        : surname(s), name(n), patronymic(p) {}

    friend ostream& operator<<(ostream& out, const Student& st) {
        out << st.surname << " " << st.name << " " << st.patronymic;
        return out;
    }

    friend istream& operator>>(istream& in, Student& st) {
        in >> st.surname >> st.name >> st.patronymic;
        return in;
    }
};

struct mPara {
Student pib;
int id;
}
class AssocArray {
private:
   mPara  data[MaxElem];
   int current =0;

public:
    // Додати елемент
    void add(int id, const Student& st) {
        data[current].pib = st;
        data[current].id = id;
        if(current<MaxElem-1) current++; 
    }

    // Перевантаження [] - доступ за індексом (id)
    Student operator[](int id) {
   bool yes = false;
        
   for(int i=0; i<current; i++)
       if(data[i].id==id)
       { yes =true;
        return data[i].pib;
       }   
 
        if (yes==false) {
            CodeError = 1;
            return Student(); // Повертає пустий об'єкт
        }

    }

    // Перевантаження виклику функції
    Student operator()(int id) {
        return (*this)[id];
    }

    // Дружнє перевантаження виводу
    friend ostream& operator<<(ostream& out, const AssocArray& aa) {
        for (auto& pair : aa.data) {
            out << "ID: " << pair.id << " -> " << pair.pib << endl;
        }
        return out;
    }

    // Дружнє перевантаження введення (вставка одного студента)
    friend istream& operator>>(istream& in, AssocArray& aa) {
        // укр не працює ?
        int id;
        Student st;
        cout << " ID: " << endl;
        in >> id;

        cout << "  Прізвище: " << endl;
        in >> st.surname;

        cout << "  Ім'я: " << endl;
        in >> st.name;

        cout << "  По батькові: " << endl;
        in >> st.patronymic;
        cout << st;
        aa.add(id, st);
        return in;
    }
};

// Функція створення тестового набору
AssocArray createSampleData() {
    AssocArray aa;
    aa.add(1, Student("Іваненко", "Іван", "Іванович"));
    aa.add(2, Student("Петренко", "Петро", "Петрович"));
    aa.add(3, Student("Сидоренко", "Сидір", "Сидорович"));
    return aa;
}

int main() {
    system("chcp 65001");
    AssocArray aa = createSampleData();

    cout << "=== Вміст асоціативного масиву ===" << endl;
    cout << aa;

    cout << "\n=== Перевірка доступу через operator[] ===" << endl;
    Student s1 = aa[2];
    if (!CodeError) cout << "ID 2: " << s1 << endl;
    else cout << "Студент з ID 2 не знайдений\n";

    Student s2 = aa[5]; // Несуществующий ID
    if (!CodeError) cout << "ID 5: " << s2 << endl;
    else cout << "Студент з ID 5 не знайдений (CodeError = " << CodeError << ")\n";

    cout << "\n=== Перевірка доступу через operator() ===" << endl;
    cout << "ID 1: " << aa(1) << endl;

    cout << "\n=== Тест введення нового студента ===" << endl;
    cin >> aa;

    cout << "\n=== Оновлений список студентів ===" << endl;
    cout << aa;

    return 0;
}
