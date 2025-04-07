#include <iostream>
#include <cmath>

using namespace std;

enum State {
    // перечислення усіх станів вектора
    OK,
    NULL_POINTER_ERROR,
    DIVISION_BY_ZERO,
    INDEX_ERROR
};

class Vector3D {
private:
    double x = 0, y = 0, z = 0;
    State state = OK;
    static int count;

public:
    Vector3D() { count++; }// по умолчанию = вектор 0
    Vector3D(double val) : x(val), y(val), z(val) { count++; } // для створення вектора з щаданими координатами
    Vector3D(const double* arr) {   //  создаєм з масива і перевіряємо на nul ptr
        if (!arr) {
            state = NULL_POINTER_ERROR;
        } else {
            x = arr[0]; y = arr[1]; z = arr[2];
        }
        count++;
    }

    ~Vector3D() {       //  деструктор з виводом стану вектора
        cout << "Vector state: ";
        switch (state) {
            case OK: cout << "OK"; break;
            case NULL_POINTER_ERROR: cout << "NULL pointer error"; break;
            case DIVISION_BY_ZERO: cout << "Division by zero"; break;
            case INDEX_ERROR: cout << "Index error"; break;
        }
        cout << endl;
        count--;        //  уменшаєм кількість записаних  векторів
    }

    // перевантаження операторів
    Vector3D& operator++() { x++; y++; z++; return *this; }
    Vector3D operator++(int) { Vector3D tmp(*this); ++(*this); return tmp; }
    Vector3D& operator--() { x--; y--; z--; return *this; }
    Vector3D operator--(int) { Vector3D tmp(*this); --(*this); return tmp; }
    bool operator!() const { return x != 0 || y != 0 || z != 0; }
    Vector3D operator~() const { return (x == 0 && y == 0) ? Vector3D(1, 1, 0) : Vector3D(y, -x, 0); }
    Vector3D operator-() const { return Vector3D(-x, -y, -z); }

    Vector3D& operator=(const Vector3D& o) = default;
    Vector3D& operator+=(const Vector3D& o) { x += o.x; y += o.y; z += o.z; return *this; }
    Vector3D& operator-=(const Vector3D& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    Vector3D& operator*=(double s) { x *= s; y *= s; z *= s; return *this; }
    Vector3D& operator/=(double s) {
        if (s == 0) { state = DIVISION_BY_ZERO; return *this; }
        x /= s; y /= s; z /= s; return *this;
    }
    Vector3D& operator&=(const Vector3D& o) {
        double nx = y * o.z - z * o.y;
        double ny = z * o.x - x * o.z;
        double nz = x * o.y - y * o.x;
        x = nx; y = ny; z = nz;
        return *this;
    }
    Vector3D& operator%=(int s) {
        if (s == 0) { state = DIVISION_BY_ZERO; return *this; }
        x = fmod(x, s); y = fmod(y, s); z = fmod(z, s);
        return *this;
    }

    Vector3D operator+(const Vector3D& o) const { return Vector3D(x + o.x, y + o.y, z + o.z); }
    Vector3D operator-(const Vector3D& o) const { return Vector3D(x - o.x, y - o.y, z - o.z); }
    Vector3D operator*(double s) const { return Vector3D(x * s, y * s, z * s); }
    Vector3D operator/(double s) const {
        if (s == 0) return Vector3D(*this).setState(DIVISION_BY_ZERO);
        return Vector3D(x / s, y / s, z / s);
    }
    Vector3D operator%(int s) const {
        if (s == 0) return Vector3D(*this).setState(DIVISION_BY_ZERO);
        return Vector3D(fmod(x, s), fmod(y, s), fmod(z, s));
    }

    bool operator==(const Vector3D& o) const { return x == o.x && y == o.y && z == o.z; }
    bool operator!=(const Vector3D& o) const { return !(*this == o); }
    bool operator>(const Vector3D& o) const { return length() > o.length(); }
    bool operator>=(const Vector3D& o) const { return length() >= o.length(); }
    bool operator<(const Vector3D& o) const { return length() < o.length(); }
    bool operator<=(const Vector3D& o) const { return length() <= o.length(); }

    double& operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        state = INDEX_ERROR;
        return z;
    }
    double operator()(int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        const_cast<Vector3D*>(this)->state = INDEX_ERROR;
        return z;
    }

    static int getCount() { return count; }
    //  перевантажені дружні функції для роботи з виводом
    friend ostream& operator<<(ostream& os, const Vector3D& v) {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }
    friend istream& operator>>(istream& is, Vector3D& v) {
        return is >> v.x >> v.y >> v.z;
    }
    friend Vector3D operator*(double s, const Vector3D& v) { return v * s; }

private:
    // конструктор для создання тимчасових обєктів
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) { count++; }
    double length() const { return sqrt(x*x + y*y + z*z); }
    Vector3D& setState(State s) { state = s; return *this; }
};

int Vector3D::count = 0;


int main() {
    // Тестування класу Vector3D

    cout << "Creating vectors..." << endl;
    Vector3D v1; // Конструктор без параметрів
    Vector3D v2(5.0); // Конструктор з одним параметром
    double arr[] = {1.0, 2.0, 3.0};
    Vector3D v3(arr); // Конструктор з масивом
    Vector3D v4(nullptr); // Конструктор з nullptr (має встановити помилку)

    cout << "v1: " << v1 << endl;
    cout << "v2: " << v2 << endl;
    cout << "v3: " << v3 << endl;
    cout << "v4: " << v4 << endl;

    cout << "\nTesting unary operators..." << endl;
    cout << "!v1: " << !v1 << endl;
    cout << "!v3: " << !v3 << endl;
    cout << "~v3: " << ~v3 << endl;
    cout << "-v3: " << -v3 << endl;

    cout << "\nTesting increment/decrement..." << endl;
    cout << "v3++: " << v3++ << endl;
    cout << "Now v3: " << v3 << endl;
    cout << "++v3: " << ++v3 << endl;
    cout << "--v3: " << --v3 << endl;
    cout << "v3--: " << v3-- << endl;
    cout << "Now v3: " << v3 << endl;

    cout << "\nTesting assignment operators..." << endl;
    v1 = v3;
    cout << "v1 = v3: " << v1 << endl;
    v1 += v2;
    cout << "v1 += v2: " << v1 << endl;
    v1 -= v2;
    cout << "v1 -= v2: " << v1 << endl;
    v1 *= 2.0;
    cout << "v1 *= 2.0: " << v1 << endl;
    v1 /= 2.0;
    cout << "v1 /= 2.0: " << v1 << endl;
    v3 &= v2; // Векторний добуток
    cout << "v3 &= v2 (vector product): " << v3 << endl;
    v3 %= 2;
    cout << "v3 %= 2: " << v3 << endl;

    cout << "\nTesting binary operators..." << endl;
    cout << "v1 + v2: " << v1 + v2 << endl;
    cout << "v1 - v2: " << v1 - v2 << endl;
    cout << "v1 * 3.0: " << v1 * 3.0 << endl;
    cout << "3.0 * v1: " << 3.0 * v1 << endl;
    cout << "v1 / 2.0: " << v1 / 2.0 << endl;
    cout << "v1 % 2: " << v1 % 2 << endl;

    cout << "\nTesting comparison operators..." << endl;
    cout << "v1 == v2: " << (v1 == v2) << endl;
    cout << "v1 != v2: " << (v1 != v2) << endl;
    cout << "v1 > v2: " << (v1 > v2) << endl;
    cout << "v1 >= v2: " << (v1 >= v2) << endl;
    cout << "v1 < v2: " << (v1 < v2) << endl;
    cout << "v1 <= v2: " << (v1 <= v2) << endl;

    cout << "\nTesting indexing..." << endl;
    cout << "v3[0]: " << v3[0] << endl;
    cout << "v3[1]: " << v3[1] << endl;
    cout << "v3[2]: " << v3[2] << endl;
    cout << "v3[3] (invalid): " << v3[3] << endl;

    cout << "\nTesting function call operator..." << endl;
    cout << "v3(0): " << v3(0) << endl;
    cout << "v3(1): " << v3(1) << endl;
    cout << "v3(2): " << v3(2) << endl;
    cout << "v3(3) (invalid): " << v3(3) << endl;

    cout << "\nTesting division by zero..." << endl;
    Vector3D v5 = v1 / 0.0;
    Vector3D v6 = v1 % 0;

    cout << "\nTesting new/delete..." << endl;
    Vector3D* pv = new Vector3D(7.0);
    cout << "*pv: " << *pv << endl;
    delete pv;

    cout << "\nNumber of Vector3D objects: " << Vector3D::getCount() << endl;

    return 0;
}