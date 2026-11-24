#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;

class Date {
private:
    int day;
    int month;
    int year;

    bool isLeapYear(int y) const {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }

    int daysInMonth(int m, int y) const {
        switch (m) {
            case 2: return isLeapYear(y) ? 29 : 28;
            case 4: case 6: case 9: case 11: return 30;
            default: return 31;
        }
    }

public:
    Date() : day(1), month(1), year(2000) {}

    Date(int d, int m, int y) : day(1), month(1), year(2000) {
        setYear(y);
        setMonth(m);
        setDay(d);
    }

    Date(const Date& other) : day(other.day), month(other.month), year(other.year) {}

    bool setYear(int y) {
        if (y < 1 || y > 9999) {
            cout << "Pomylka: nekorektnyy rik (" << y << ")!" << endl;
            return false;
        }
        year = y;
        if (day > daysInMonth(month, year)) day = daysInMonth(month, year);
        return true;
    }

    bool setMonth(int m) {
        if (m < 1 || m > 12) {
            cout << "Pomylka: nekorektnyy misyats (" << m << ")!" << endl;
            return false;
        }
        month = m;
        if (day > daysInMonth(month, year)) day = daysInMonth(month, year);
        return true;
    }

    bool setDay(int d) {
        int maxD = daysInMonth(month, year);
        if (d < 1 || d > maxD) {
            cout << "Pomylka: nekorektnyy den (" << d << ") dlya misyatsya " << month << "!" << endl;
            return false;
        }
        day = d;
        return true;
    }

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }

    void printText() const {
        const string months[] = {
            "", "sichnya", "lyutoho", "bereznya", "kvitnya", "travnya", "chervnya",
            "lypnya", "serpnya", "veresnya", "zhovtnya", "lystopada", "hrudnya"
        };
        cout << day << " " << months[month] << " " << year << " roku" << endl;
    }

    void printNumeric() const {
        cout << setfill('0') << setw(2) << day << "."
             << setw(2) << month << "."
             << setw(4) << year << endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "=== ZAVDANNYA 1 (Zadacha 1.1 - Klas Date, Variant 11) ===" << endl;

    Date d1;
    Date d2(5, 1, 2019);
    cout << "1) Konstruktor za zamovchuvannyam: "; d1.printNumeric();
    cout << "2) Konstruktor z parametramy (5.01.2019):" << endl;
    cout << "   Shablon 1: "; d2.printText();
    cout << "   Shablon 2: "; d2.printNumeric();

    cout << "\n3) Perevirka nekorektnykh danykh:" << endl;
    d2.setMonth(15);
    d2.setDay(31);
    d2.setMonth(2);
    d2.setDay(30);

    int kd, km, ky;
    cout << "\n4) Vvedit datu z klaviatury (den misyats rik): ";
    cin >> kd >> km >> ky;
    Date dKey(kd, km, ky);
    cout << "   Vvedena data: "; dKey.printText();

    ofstream fout("date_input.txt");
    fout << "24 8 1991";
    fout.close();

    ifstream fin("date_input.txt");
    int fd, fm, fy;
    fin >> fd >> fm >> fy;
    fin.close();
    Date dFile(fd, fm, fy);
    cout << "\n5) Data z faylu 'date_input.txt': ";
    dFile.printText();
    cout << "   Chyslovyy format: "; dFile.printNumeric();

    int ry = 1990 + rand() % 37;
    int rm = 1 + rand() % 12;
    int rd = 1 + rand() % 28;
    Date dRand(rd, rm, ry);
    cout << "\n6) Vypadkova data: ";
    dRand.printText();
    cout << "   Chyslovyy format: "; dRand.printNumeric();

    return 0;
}