#include <iostream>   // Підключення бібліотеки введення/виведення у консоль
#include <fstream>    // Підключення бібліотеки для роботи з файлами
#include <string>     // Підключення бібліотеки рядків std::string
#include <cmath>      // Підключення математичних функцій (sqrt, fabs)
#include <cstdlib>    // Підключення функцій rand() та srand()
#include <ctime>      // Підключення функції time() для генератора випадкових чисел
#include <windows.h>  // Підключення Windows API для налаштування кодування консолі
using namespace std;  // Використання стандартного простору імен std

// Перелік можливих кодів стану об'єкта вектора[cite: 20]
enum VecState { OK = 0, ERR_NULLPTR = 1, ERR_DIV_ZERO = 2, ERR_INDEX = 3 };

// Оголошення класу тривимірного вектора Vector3D[cite: 20]
class Vector3D {
private:                     // Приватні поля класу (інкапсуляція даних)
    float x, y, z;           // Координати вектора типу float[cite: 20]
    int state;               // Змінна стану помилки вектора[cite: 20]
    static int objectCount;  // Статичний лічильник створених об'єктів класу[cite: 20]

public:                      // Публічні методи класу (інтерфейс)
    // Конструктор без параметрів (ініціалізує координати в нуль)[cite: 20]
    Vector3D() : x(0.0f), y(0.0f), z(0.0f), state(OK) {
        objectCount++;       // Збільшення лічильника об'єктів при створенні[cite: 20]
    }

    // Конструктор з одним параметром типу float (заповнює всі координати одним значенням)[cite: 20]
    Vector3D(float val) : x(val), y(val), z(val), state(OK) {
        objectCount++;       // Збільшення лічильника об'єктів[cite: 20]
    }

    // Конструктор з одним параметром-вказівником на масив типу float[cite: 20]
    Vector3D(const float* arr) {
        if (arr == nullptr) {            // Перевірка, чи не передано порожній вказівник NULL[cite: 20]
            x = y = z = 0.0f;            // Обнулення координат у разі помилки
            state = ERR_NULLPTR;         // Встановлення коду помилки порожнього вказівника[cite: 20]
        }
        else {                         // Якщо вказівник коректний
            x = arr[0];                  // Присвоєння першого елемента масиву координаті x[cite: 20]
            y = arr[1];                  // Присвоєння другого елемента масиву координаті y[cite: 20]
            z = arr[2];                  // Присвоєння третього елемента масиву координаті z[cite: 20]
            state = OK;                  // Встановлення нормального стану OK
        }
        objectCount++;                   // Збільшення лічильника об'єктів[cite: 20]
    }

    // Конструктор копіювання[cite: 20]
    Vector3D(const Vector3D& other) : x(other.x), y(other.y), z(other.z), state(other.state) {
        objectCount++;                   // Збільшення лічильника об'єктів при копіюванні[cite: 20]
    }

    // Перевантаження операції присвоєння[cite: 20]
    Vector3D& operator=(const Vector3D& other) {
        if (this != &other) {            // Захист від самоприсвоєння (v = v)
            x = other.x;                 // Копіювання координати x
            y = other.y;                 // Копіювання координати y
            z = other.z;                 // Копіювання координати z
            state = other.state;         // Копіювання стану помилки
        }
        return *this;                    // Повернення посилання на поточний об'єкт
    }

    // Деструктор із виведенням інформації про стан вектора[cite: 20]
    ~Vector3D() {
        objectCount--;                   // Зменшення лічильника при знищенні об'єкта[cite: 20]
        cout << "[Destructor Vector3D] state = " << state << ", zalyshylos obyektiv = " << objectCount << endl;
    }

    // Метод присвоєння координаті x (0), y (1) або z (2) значення (параметр за замовчуванням 0.0f)[cite: 20]
    void setCoord(int index, float val = 0.0f) {
        if (index == 0) x = val;         // Якщо індекс 0 — змінюємо координату x[cite: 20]
        else if (index == 1) y = val;    // Якщо індекс 1 — змінюємо координату y[cite: 20]
        else if (index == 2) z = val;    // Якщо індекс 2 — змінюємо координату z[cite: 20]
        else state = ERR_INDEX;          // Інакше встановлюємо помилку індексу[cite: 20]
    }

    // Метод отримання значення координати за індексом (0, 1 або 2)[cite: 20]
    float getCoord(int index) {
        if (index == 0) return x;        // Повертаємо координату x[cite: 20]
        if (index == 1) return y;        // Повертаємо координату y[cite: 20]
        if (index == 2) return z;        // Повертаємо координату z[cite: 20]
        state = ERR_INDEX;               // Встановлюємо помилку виходу за межі[cite: 20]
        return 0.0f;                     // Повертаємо 0.0f при помилці
    }

    int getState() const { return state; }         // Метод отримання поточного стану вектора[cite: 20]
    static int getCount() { return objectCount; }  // Статичний метод отримання кількості об'єктів[cite: 20]

    // Метод виведення координат та стану вектора на екран[cite: 20]
    void print(const string& label = "Vec") const {
        cout << label << " = (" << x << ", " << y << ", " << z << ") [state=" << state << "]" << endl;
    }

    // Метод додавання двох векторів[cite: 20]
    Vector3D add(const Vector3D& other) const {
        Vector3D res;                    // Створення результуючого вектора
        res.x = x + other.x;             // Додавання координат x
        res.y = y + other.y;             // Додавання координат y
        res.z = z + other.z;             // Додавання координат z
        return res;                      // Повернення суми векторів
    }

    // Метод віднімання двох векторів[cite: 20]
    Vector3D sub(const Vector3D& other) const {
        Vector3D res;                    // Створення результуючого вектора
        res.x = x - other.x;             // Віднімання координат x
        res.y = y - other.y;             // Віднімання координат y
        res.z = z - other.z;             // Віднімання координат z
        return res;                      // Повернення різниці векторів
    }

    // Метод обчислення векторного добутку[cite: 20]
    Vector3D cross(const Vector3D& other) const {
        Vector3D res;                    // Створення результуючого вектора
        res.x = y * other.z - z * other.y; // Обчислення x-компоненти векторного добутку
        res.y = z * other.x - x * other.z; // Обчислення y-компоненти векторного добутку
        res.z = x * other.y - y * other.x; // Обчислення z-компоненти векторного добутку
        return res;                      // Повернення векторного добутку
    }

    // Метод ділення вектора на ціле число типу short[cite: 20]
    Vector3D divShort(short divisor) {
        if (divisor == 0) {              // Перевірка ділення на нуль[cite: 20]
            state = ERR_DIV_ZERO;        // Встановлення коду помилки ділення на 0[cite: 20]
            cout << "Pomylka: dilennya na 0!" << endl; // Виведення повідомлення про помилку
            return *this;                // Повернення вектора без виконання ділення[cite: 20]
        }
        Vector3D res;                    // Створення результуючого вектора
        res.x = x / divisor;             // Ділення координати x на число
        res.y = y / divisor;             // Ділення координати y на число
        res.z = z / divisor;             // Ділення координати z на число
        return res;                      // Повернення результату ділення
    }

    // Метод обчислення модуля (довжини) вектора
    float length() const { return sqrt(x * x + y * y + z * z); }
    // Перевірка, чи поточний вектор більший за інший (за довжиною)[cite: 20]
    bool isGreater(const Vector3D& other) const { return length() > other.length(); }
    // Перевірка, чи поточний вектор менший за інший (за довжиною)[cite: 20]
    bool isLess(const Vector3D& other) const { return length() < other.length(); }
    // Перевірка рівності двох векторів за координатами[cite: 20]
    bool isEqual(const Vector3D& other) const {
        return fabs(x - other.x) < 1e-5 && fabs(y - other.y) < 1e-5 && fabs(z - other.z) < 1e-5;
    }
};

// Ініціалізація статичного лічильника об'єктів класу Vector3D[cite: 20]
int Vector3D::objectCount = 0;

// Головна функція тестування класу Vector3D[cite: 20]
int main() {
    SetConsoleOutputCP(65001);           // Встановлення кодування виводу UTF-8
    SetConsoleCP(65001);                 // Встановлення кодування вводу UTF-8
    srand(static_cast<unsigned int>(time(nullptr))); // Ініціалізація генератора випадкових чисел[cite: 14]

    cout << "=== ZAVDANNYA 2 (Zadacha 2.6 - Klas Vector3D, Variant 11) ===" << endl;

    // 1. Тестування всіх конструкторів та підрахунку об'єктів[cite: 20]
    Vector3D vDef;                       // Створення вектора конструктором за замовчуванням[cite: 20]
    Vector3D vOne(5.0f);                 // Створення вектора конструктором з одним параметром[cite: 20]
    float* nullArr = nullptr;            // Оголошення порожнього вказівника
    Vector3D vErr(nullArr);              // Перевірка обробки помилки при передачі nullptr[cite: 20]
    Vector3D vCopy(vOne);                // Створення копії вектора конструктором копіювання[cite: 20]

    vDef.print("vDef (default)");        // Друк нульового вектора
    vOne.print("vOne (5.0)");            // Друк вектора (5, 5, 5)
    vErr.print("vErr (nullptr)");        // Друк вектора зі станом помилки state=1
    vCopy.print("vCopy (kopiya vOne)");  // Друк скопійованого вектора
    cout << "Kilkist aktyvnykh obyektiv Vector3D: " << Vector3D::getCount() << endl; // Виведення лічильника об'єктів[cite: 20]

    // 2. Тестування методів setCoord (з параметром за замовчуванням) та getCoord[cite: 20]
    vDef.setCoord(0, 7.5f);              // Встановлення x = 7.5
    vDef.setCoord(1);                    // Виклик зі значенням за замовчуванням (y = 0.0f)[cite: 20]
    vDef.setCoord(2, -3.5f);             // Встановлення z = -3.5
    vDef.print("vDef pislya setCoord");  // Виведення оновленого вектора vDef
    cout << "Otrymana koordynata x cherez getCoord(0): " << vDef.getCoord(0) << endl; // Перевірка getCoord[cite: 20]

    // 3. Введення координат вектора з клавіатури[cite: 14]
    float kx, ky, kz;                    // Змінні для зчитування координат з клавіатури
    cout << "\nVvedit koordynaty vektora A z klaviatury (x y z): ";
    cin >> kx >> ky >> kz;               // Зчитування трьох координат із консолі[cite: 14]
    float arrKey[3] = { kx, ky, kz };      // Формування масиву координат
    Vector3D vA(arrKey);                 // Створення вектора vA з масиву[cite: 20]

    // 4. Запис та зчитування координат вектора з файлу[cite: 14]
    ofstream fout("vec_input.txt");      // Відкриття текстового файлу для запису
    fout << "2.0 4.0 6.0";               // Запис тестових координат у файл
    fout.close();                        // Закриття файлу

    ifstream fin("vec_input.txt");       // Відкриття файлу для читання[cite: 14]
    float arrFile[3];                    // Масив для координат з файлу
    fin >> arrFile[0] >> arrFile[1] >> arrFile[2]; // Зчитування координат з файлу[cite: 14]
    fin.close();                         // Закриття файлу
    Vector3D vB(arrFile);                // Створення вектора vB з даних файлу[cite: 20]

    // 5. Генерація випадкового вектора[cite: 14]
    float arrRand[3] = { (float)(rand() % 10 + 1), (float)(rand() % 10 + 1), (float)(rand() % 10 + 1) };
    Vector3D vRand(arrRand);             // Створення вектора vRand з випадковими координатами[cite: 14]

    vA.print("vA (klaviatura)");         // Виведення вектора з клавіатури
    vB.print("vB (fayl)");               // Виведення вектора з файлу
    vRand.print("vRand (vypadkovyy)");   // Виведення випадкового вектора

    // 6. Виконання арифметичних операцій та операції присвоєння[cite: 20]
    Vector3D vSum, vSub, vCross, vDiv;   // Оголошення об'єктів для результатів
    vSum = vA.add(vB);                   // Додавання векторів та присвоєння[cite: 20]
    vSub = vA.sub(vB);                   // Віднімання векторів[cite: 20]
    vCross = vA.cross(vB);               // Векторний добуток vA x vB[cite: 20]
    vDiv = vB.divShort(2);               // Ділення вектора vB на 2[cite: 20]

    cout << "\nAryfmetychni operatsiyi:" << endl;
    vSum.print("vA + vB");               // Виведення результату додавання
    vSub.print("vA - vB");               // Виведення результату віднімання
    vCross.print("vA x vB (vektornyy dobutok)"); // Виведення векторного добутку
    vDiv.print("vB / 2");                // Виведення результату ділення

    // 7. Перевірка всіх методів порівняння (більше, менше, рівно)[cite: 20]
    cout << "\nPorivnyannya vektoriv:" << endl;
    cout << "vA > vB (isGreater): " << (vA.isGreater(vB) ? "True" : "False") << endl; // Більше[cite: 20]
    cout << "vA < vB (isLess):    " << (vA.isLess(vB) ? "True" : "False") << endl;    // Менше[cite: 20]
    cout << "vOne == vCopy (isEqual): " << (vOne.isEqual(vCopy) ? "True" : "False") << endl; // Рівно[cite: 20]

    // 8. Перевірка обробки помилки ділення на 0[cite: 20]
    cout << "\nPerevirka dilennya na 0:" << endl;
    vA.divShort(0);                      // Спроба поділити вектор vA на 0[cite: 20]
    vA.print("vA pislya sproby dilennya na 0"); // Виведення стану помилки state=2

    cout << "\nZavershennya programy (vyklyk destruktoyiv):" << endl;
    return 0;                            // Успішне завершення роботи програми (виклик деструкторів)
}