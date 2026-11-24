#include <iostream>   // Підключення бібліотеки введення/виведення у консоль
#include <fstream>    // Підключення бібліотеки для роботи з файлами
#include <iomanip>    // Підключення маніпуляторів форматування виводу (setw)
#include <string>     // Підключення бібліотеки рядків std::string
#include <cstdlib>    // Підключення функцій rand() та srand()
#include <ctime>      // Підключення функції time() для генератора випадкових чисел
#include <windows.h>  // Підключення Windows API для налаштування кодування консолі
using namespace std;  // Використання стандартного простору імен std

// Перелік можливих кодів стану помилки матриці[cite: 23]
enum MatState { M_OK = 0, M_ERR_MEM = 1, M_ERR_RANGE = 2, M_ERR_DIV0 = 3, M_ERR_DIM = 4 };

// Клас Вектор (рядок матриці типу long)[cite: 23]
class VectorLong {
private:              // Приватні поля класу VectorLong
    long* data;       // Вказівник на динамічний масив елементів типу long[cite: 23]
    int size;         // Розмір вектора (кількість елементів)[cite: 23]

public:               // Публічні методи класу VectorLong
    // Конструктор без параметрів (виділяє місце для 5 елементів та обнуляє їх)[cite: 23]
    VectorLong() : size(5) {
        data = new long[size]{0};    // Виділення динамічної пам'яті та ініціалізація нулями[cite: 23]
    }

    // Конструктор з одним параметром (виділяє місце для n елементів та обнуляє їх)[cite: 23]
    VectorLong(int n) : size(n > 0 ? n : 5) {
        data = new long[size]{0};    // Виділення пам'яті для n елементів[cite: 23]
    }

    // Конструктор з двома параметрами (розмір n та значення ініціалізації value)[cite: 23]
    VectorLong(int n, long value) : size(n > 0 ? n : 5) {
        data = new long[size];       // Виділення пам'яті для n елементів[cite: 23]
        for (int i = 0; i < size; i++) data[i] = value; // Заповнення значенням value[cite: 23]
    }

    // Конструктор копіювання[cite: 23]
    VectorLong(const VectorLong& other) : size(other.size) {
        data = new long[size];       // Виділення нової ділянки пам'яті для копії
        for (int i = 0; i < size; i++) data[i] = other.data[i]; // Поелементне копіювання
    }

    // Перевантаження операції присвоєння[cite: 23]
    VectorLong& operator=(const VectorLong& other) {
        if (this != &other) {        // Захист від самоприсвоєння
            delete[] data;           // Звільнення попередньо виділеної пам'яті
            size = other.size;       // Оновлення розміру вектора
            data = new long[size];   // Виділення пам'яті під новий розмір
            for (int i = 0; i < size; i++) data[i] = other.data[i]; // Копіювання елементів
        }
        return *this;                // Повернення посилання на поточний об'єкт
    }

    // Деструктор класу VectorLong[cite: 23]
    ~VectorLong() {
        delete[] data;               // Звільнення динамічної пам'яті масиву[cite: 23]
    }

    // Метод зміни розміру вектора та заповнення значенням value
    void resize(int newSize, long value = 0) {
        delete[] data;               // Видалення старого масиву
        size = newSize;              // Встановлення нового розміру
        data = new long[size];       // Виділення пам'яті під новий розмір
        for (int i = 0; i < size; i++) data[i] = value; // Ініціалізація елементів
    }

    int getSize() const { return size; } // Отримання розміру вектора
    long& operator[](int idx) { return data[idx]; } // Оператор індексації для зміни елемента
    const long& operator[](int idx) const { return data[idx]; } // Константний оператор індексації
};

// Клас Матриця на основі масиву об'єктів VectorLong[cite: 23]
class MatrixLong {
private:                     // Приватні поля класу MatrixLong
    VectorLong* rows;        // Вказівник на динамічний масив об'єктів класу VectorLong[cite: 23]
    int numRows;             // Кількість векторів (рядків матриці)[cite: 23]
    int numCols;             // Кількість стовпців матриці
    int state;               // Змінна стану помилки матриці[cite: 23]
    static int matrixCount;  // Статичний лічильник створених об'єктів MatrixLong[cite: 23]

public:                      // Публічні методи класу MatrixLong
    // Конструктор без параметрів (виділяє місце для 5 векторів по 5 елементів)[cite: 23]
    MatrixLong() : numRows(5), numCols(5), state(M_OK) {
        rows = new VectorLong[numRows]; // Створення 5 векторів розміром 5[cite: 23]
        matrixCount++;       // Збільшення лічильника об'єктів матриць[cite: 23]
    }

    // Конструктор з одним параметром n (квадратна матриця n на n)[cite: 23]
    MatrixLong(int n) : numRows(n), numCols(n), state(M_OK) {
        if (n <= 0) {                    // Перевірка коректності розміру
            numRows = numCols = 0;       // Обнулення розмірів при помилці
            rows = nullptr;              // Встановлення порожнього вказівника
            state = M_ERR_RANGE;         // Код помилки некоректного діапазону[cite: 23]
        } else {                         // Якщо розмір коректний
            rows = new VectorLong[numRows]; // Виділення масиву з n векторів[cite: 23]
            for (int i = 0; i < numRows; i++) rows[i].resize(numCols, 0); // Ініціалізація нулями[cite: 23]
        }
        matrixCount++;                   // Збільшення лічильника об'єктів[cite: 23]
    }

    // Конструктор з трьома параметрами: розміри (n, m) та значення ініціалізації value[cite: 23]
    MatrixLong(int n, int m, long value) : numRows(n), numCols(m), state(M_OK) {
        if (n <= 0 || m <= 0) {          // Перевірка коректності розмірів матриці
            numRows = numCols = 0;       // Обнулення розмірів при помилці
            rows = nullptr;              // Встановлення порожнього вказівника
            state = M_ERR_RANGE;         // Встановлення коду помилки[cite: 23]
        } else {                         // Якщо розміри коректні
            rows = new VectorLong[numRows]; // Виділення пам'яті для рядків матриці[cite: 23]
            for (int i = 0; i < numRows; i++) rows[i].resize(numCols, value); // Заповнення значенням value[cite: 23]
        }
        matrixCount++;                   // Збільшення лічильника об'єктів[cite: 23]
    }

    // Конструктор копіювання[cite: 23]
    MatrixLong(const MatrixLong& other) : numRows(other.numRows), numCols(other.numCols), state(other.state) {
        if (numRows > 0) {               // Якщо матриця не порожня
            rows = new VectorLong[numRows]; // Виділення пам'яті під масив векторів
            for (int i = 0; i < numRows; i++) rows[i] = other.rows[i]; // Копіювання кожного вектора
        } else {                         // Якщо матриця порожня
            rows = nullptr;              // Встановлення вказівника в nullptr
        }
        matrixCount++;                   // Збільшення лічильника об'єктів[cite: 23]
    }

    // Перевантаження операції присвоєння[cite: 23]
    MatrixLong& operator=(const MatrixLong& other) {
        if (this != &other) {            // Захист від самоприсвоєння
            delete[] rows;               // Звільнення старої динамічної пам'яті
            numRows = other.numRows;     // Копіювання кількості рядків
            numCols = other.numCols;     // Копіювання кількості стовпців
            state = other.state;         // Копіювання стану помилки
            if (numRows > 0) {           // Якщо є рядки для копіювання
                rows = new VectorLong[numRows]; // Виділення нової пам'яті
                for (int i = 0; i < numRows; i++) rows[i] = other.rows[i]; // Копіювання векторів
            } else {
                rows = nullptr;          // Встановлення порожнього вказівника
            }
        }
        return *this;                    // Повернення посилання на поточний об'єкт
    }

    // Деструктор класу MatrixLong[cite: 23]
    ~MatrixLong() {
        delete[] rows;                   // Звільнення динамічного масиву векторів[cite: 23]
        matrixCount--;                   // Зменшення лічильника об'єктів при знищенні[cite: 23]
    }

    // Метод присвоєння елементу матриці значення (параметр val за замовчуванням 0)[cite: 23]
    void setElement(int i, int j, long val = 0) {
        if (i < 0 || i >= numRows || j < 0 || j >= numCols) { // Перевірка виходу за межі масивів[cite: 23]
            state = M_ERR_RANGE;         // Встановлення коду помилки виходу за межі[cite: 23]
            return;                      // Вихід з методу без запису
        }
        rows[i][j] = val;                // Запис значення у відповідний елемент
    }

    // Метод отримання елемента матриці за індексами i та j[cite: 23]
    long getElement(int i, int j) {
        if (i < 0 || i >= numRows || j < 0 || j >= numCols) { // Перевірка виходу за межі масивів[cite: 23]
            state = M_ERR_RANGE;         // Встановлення коду помилки виходу за межі[cite: 23]
            return 0;                    // Повернення 0 при помилці
        }
        return rows[i][j];               // Повернення значення елемента [i][j][cite: 23]
    }

    int getState() const { return state; }         // Отримання поточного стану помилки[cite: 23]
    static int getCount() { return matrixCount; }  // Отримання кількості активних об'єктів[cite: 23]

    // Метод виведення матриці на екран[cite: 23]
    void print(const string& title) const {
        cout << title << " (" << numRows << "x" << numCols << ", state=" << state << "):" << endl;
        for (int i = 0; i < numRows; i++) {        // Цикл по рядках матриці
            for (int j = 0; j < numCols; j++) {    // Цикл по стовпцях матриці
                cout << setw(5) << rows[i][j];     // Форматований вивід елемента
            }
            cout << endl;                          // Перехід на новий рядок консолі
        }
    }

    // Метод додавання двох матриць[cite: 23]
    MatrixLong add(const MatrixLong& other) const {
        if (numRows != other.numRows || numCols != other.numCols) { // Перевірка збігу розмірностей
            MatrixLong err(1);           // Створення об'єкта для повернення помилки
            err.state = M_ERR_DIM;       // Встановлення помилки невідповідності розмірів
            return err;                  // Повернення об'єкта з помилкою
        }
        MatrixLong res(numRows, numCols, 0); // Створення результуючої матриці
        for (int i = 0; i < numRows; i++)    // Цикл по рядках
            for (int j = 0; j < numCols; j++) // Цикл по стовпцях
                res.rows[i][j] = rows[i][j] + other.rows[i][j]; // Поелементне додавання
        return res;                      // Повернення суми матриць
    }

    // Метод віднімання двох матриць[cite: 23]
    MatrixLong sub(const MatrixLong& other) const {
        if (numRows != other.numRows || numCols != other.numCols) { // Перевірка збігу розмірностей
            MatrixLong err(1);           // Створення об'єкта для повернення помилки
            err.state = M_ERR_DIM;       // Встановлення помилки розмірностей
            return err;                  // Повернення об'єкта з помилкою
        }
        MatrixLong res(numRows, numCols, 0); // Створення результуючої матриці
        for (int i = 0; i < numRows; i++)    // Цикл по рядках
            for (int j = 0; j < numCols; j++) // Цикл по стовпцях
                res.rows[i][j] = rows[i][j] - other.rows[i][j]; // Поелементне віднімання
        return res;                      // Повернення різниці матриць
    }

    // Метод множення двох матриць[cite: 23]
    MatrixLong multiply(const MatrixLong& other) const {
        if (numCols != other.numRows) {  // Перевірка узгодженості розмірів для множення
            MatrixLong err(1);           // Створення об'єкта для повернення помилки
            err.state = M_ERR_DIM;       // Встановлення помилки розмірностей
            return err;                  // Повернення об'єкта з помилкою
        }
        MatrixLong res(numRows, other.numCols, 0); // Створення результуючої матриці
        for (int i = 0; i < numRows; i++)          // Цикл по рядках першої матриці
            for (int j = 0; j < other.numCols; j++) // Цикл по стовпцях другої матриці
                for (int k = 0; k < numCols; k++)   // Внутрішній цикл скалярного добутку
                    res.rows[i][j] += rows[i][k] * other.rows[k][j]; // Накопичення добутку
        return res;                      // Повернення добутку матриць
    }

    // Метод ділення матриці на скаляр типу long[cite: 23]
    MatrixLong divScalar(long scalar) {
        if (scalar == 0) {               // Перевірка ділення на нуль[cite: 23]
            state = M_ERR_DIV0;          // Встановлення коду помилки ділення на 0[cite: 23]
            cout << "Pomylka: dilennya matrytsi na 0!" << endl; // Виведення повідомлення
            return *this;                // Повернення матриці без виконання ділення[cite: 23]
        }
        MatrixLong res(numRows, numCols, 0); // Створення результуючої матриці
        for (int i = 0; i < numRows; i++)    // Цикл по рядках
            for (int j = 0; j < numCols; j++) // Цикл по стовпцях
                res.rows[i][j] = rows[i][j] / scalar; // Ділення кожного елемента на скаляр
        return res;                      // Повернення результату ділення
    }

    // Допоміжний метод підрахунку суми всіх елементів матриці для порівняння
    long totalSum() const {
        long s = 0;                      // Змінна для накопичення суми
        for (int i = 0; i < numRows; i++) // Цикл по рядках
            for (int j = 0; j < numCols; j++) s += rows[i][j]; // Додавання елемента до суми
        return s;                        // Повернення загальної суми
    }

    // Функції порівняння матриць: більше, менше, рівно[cite: 23]
    bool isGreater(const MatrixLong& other) const { return totalSum() > other.totalSum(); } // Більше[cite: 23]
    bool isLess(const MatrixLong& other) const { return totalSum() < other.totalSum(); }       // Менше[cite: 23]
    bool isEqual(const MatrixLong& other) const { return totalSum() == other.totalSum(); }   // Рівно[cite: 23]
};

// Ініціалізація статичного лічильника об'єктів класу MatrixLong[cite: 23]
int MatrixLong::matrixCount = 0;

// Головна функція тестування класів VectorLong та MatrixLong[cite: 23]
int main() {
    SetConsoleOutputCP(65001);           // Встановлення кодування виводу UTF-8
    SetConsoleCP(65001);                 // Встановлення кодування вводу UTF-8
    srand(static_cast<unsigned int>(time(nullptr))); // Ініціалізація генератора випадкових чисел[cite: 12]

    cout << "=== ZAVDANNYA 3 (Zadacha 3.8 - VectorLong & MatrixLong, Variant 11) ===" << endl;

    // 1. Введення елементів матриці A (2x2) з клавіатури[cite: 12]
    MatrixLong mKey(2, 2, 0);            // Створення матриці A розміром 2x2
    cout << "Vvedit 4 elementy matrytsi A (2x2) z klaviatury: ";
    for (int i = 0; i < 2; i++) {        // Цикл по рядках
        for (int j = 0; j < 2; j++) {    // Цикл по стовпцях
            long val;                    // Змінна для введення числа
            cin >> val;                  // Зчитування елемента з клавіатури[cite: 12]
            mKey.setElement(i, j, val);  // Запис елемента в матрицю A
        }
    }

    // 2. Запис та зчитування матриці B (2x2) з файлу[cite: 12]
    ofstream fout("matrix_input.txt");   // Відкриття файлу для запису
    fout << "2 4 6 8";                   // Запис 4 елементів матриці у файл
    fout.close();                        // Закриття файлу

    ifstream fin("matrix_input.txt");    // Відкриття файлу для читання[cite: 12]
    MatrixLong mFile(2, 2, 0);           // Створення матриці B розміром 2x2
    for (int i = 0; i < 2; i++) {        // Цикл по рядках
        for (int j = 0; j < 2; j++) {    // Цикл по стовпцях
            long val;                    // Змінна для зчитування з файлу
            fin >> val;                  // Зчитування числа з файлу[cite: 12]
            mFile.setElement(i, j, val); // Запис елемента в матрицю B
        }
    }
    fin.close();                         // Закриття файлу

    // 3. Генерація матриці C (2x2) випадковими числами[cite: 12]
    MatrixLong mRand(2, 2, 0);           // Створення матриці C розміром 2x2
    for (int i = 0; i < 2; i++)          // Цикл по рядках
        for (int j = 0; j < 2; j++)      // Цикл по стовпцях
            mRand.setElement(i, j, rand() % 10 + 1); // Заповнення випадковим числом 1..10[cite: 12]

    cout << "\nKilkist aktyvnykh obyektiv MatrixLong: " << MatrixLong::getCount() << endl; // Лічильник об'єктів[cite: 23]
    mKey.print("Matrytsya A (klaviatura)"); // Виведення матриці A
    mFile.print("Matrytsya B (fayl)");      // Виведення матриці B
    mRand.print("Matrytsya C (vypadkova)"); // Виведення матриці C

    // Виконання арифметичних операцій над матрицями[cite: 23]
    MatrixLong mAdd = mKey.add(mFile);      // Додавання матриць A + B[cite: 23]
    MatrixLong mSub = mKey.sub(mFile);      // Віднімання матриць A - B[cite: 23]
    MatrixLong mMul = mKey.multiply(mFile); // Множення матриць A * B[cite: 23]
    MatrixLong mDiv = mFile.divScalar(2);   // Ділення матриці B на скаляр 2[cite: 23]

    cout << endl;
    mAdd.print("A + B");                    // Виведення результату додавання
    mSub.print("A - B");                    // Виведення результату віднімання
    mMul.print("A * B");                    // Виведення результату множення
    mDiv.print("B / 2");                    // Виведення результату ділення на скаляр

    // Перевірка обробки помилок (ділення на 0 та вихід за межі масиву) і порівняння[cite: 23]
    cout << "\nPerevirka pomylok ta porivnyannya:" << endl;
    mKey.divScalar(0);                      // Спроба поділити матрицю A на 0[cite: 23]
    mKey.getElement(10, 10);                // Спроба звернутися за межі матриці[cite: 23]
    cout << "Stan matrytsi A pislya pomylok: " << mKey.getState() << endl; // Виведення стану помилки[cite: 23]
    cout << "Porivnyannya A > B: " << (mKey.isGreater(mFile) ? "True" : "False") << endl; // Порівняння[cite: 23]

    return 0;                               // Успішне завершення роботи програми
}