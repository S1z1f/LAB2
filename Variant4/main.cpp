#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
using namespace std;
struct EMPLOYEE {   // Структура - сотрудник
    string fio;     // ФИО
    string date;    // Дата приема на работу
    string position;    // Должность
    double salary;      // Доход
};
void print_employee(ostream& out, vector<EMPLOYEE>& Employees) {    // Печатаем массив сотрудников
    for (auto& pos : Employees)        //Перебираем элементы в списке
        out << "Сотрудник: " << pos.fio << ", принят: " << pos.date << ", должность: " << pos.position << ", доход: " << pos.salary << endl;
}
void write_text(ostream& out,EMPLOYEE& c) {     // Записываем одного сотрудника в файл в текстовом режиме
    out << c.fio << ends << c.date << ends << c.position << ends;   // Каждая строка с новой строки
    out << c.salary << endl;    // Число на отдельной строке
}
void write_binary(ostream& out,EMPLOYEE& c) {   // Записываем одного сотрудника в файл в двоичном виде
    out << c.fio << ends << c.date << ends << c.position << ends;   // После строк вставляем 0, как признак конца
    out.write((char*)&c.salary, sizeof(c.salary));      //Записываем число по байтам
}
void read_text(istream& in,EMPLOYEE& c) {   // Чтение одного сотрудника из файла в текстовом режиме
    in.ignore(2, '\n');     // Пропускаем символы перехода на новую строку
    getline(in, c.fio);     //Считываем целую строку
    getline(in, c.date);
    getline(in, c.position);
    in >> c.salary;     //Считываем значение
}
void read_binary(istream& in,EMPLOYEE& c) {     // Чтение одного сотрудника из файла в двоичном виде
    getline(in, c.fio, '\0');
    getline(in, c.date, '\0');
    getline(in, c.position, '\0');
    in.read((char *)&c.salary, sizeof(c.salary));   //Считаем количество байтов
}
int main(int args, const char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    if (args != 4) {
        cerr << "Error: Use three parameters: write|read text|binary file_name" << endl;
        return 5;
    }
    const string mode(argv[1]);     // Режим работы (запись или чтение)
    const string open_mode(argv[2]); // Режим открытия файла (текстовый или двоичный)
    const string file_name(argv[3]); // Имя файла
    int n =0;    // Переменная, задающая число элементов массива
    vector<EMPLOYEE> Employees;     // Массив структур
    if (mode == "write") {      // Запись в файл
        cout << "Количество сотрудников ="; cin >> n;     // Ввод числа сотрудников
        Employees.resize(n);        // Задаем размер массива
        for (int i = 0; i < n; i++) {       // Цикл ввода данных о сотрудниках с клавиатуры
            cout << "Сотрудник " << (i + 1) << endl;
            cin.ignore();       // Пропускаем символ перехода на новую строку
            cout << "ФИО: ";
            getline(cin, Employees[i].fio);
            cout << "Дата трудоустройства (ДД.MM.ГГГГ): ";
            getline(cin, Employees[i].date);
            cout << "Должность: ";
            getline(cin, Employees[i].position);
            cout << "Доход: "; cin >> Employees[i].salary;
        }

        cout << endl << "Введённые данные: " << endl;   // Выводим введенные данные
        print_employee(cout, Employees);
        if (open_mode == "text") {      // Работа с файлом в текстовом режиме
            ofstream foutt(file_name);  // Открываем файл для записи в текстовом режиме
            if (!foutt) {       // Ошибка файл не открыт
                cerr << "Error, cannot open file" << endl;
                return 1;       //Возвращаем код ошибки
            }foutt << n<< endl;     // Записываем n в текстовом режиме, далее переход на новую строку
            for (auto& pos : Employees) write_text(foutt, pos);  //Перебираем сотрудников из списка
            foutt.close();      // Закрываем файл
        }else if (open_mode== "binary") {       // Работа с файлом в двоичном режиме
            ofstream foutb(file_name, ios::binary); // Открываем файл для записи в двоичном режиме
            if (!foutb) {// Ошибка файл не открыт
                cerr << "Error, cannot open file" << endl;
                return 1;    //Возвращаем код ошибки
            }foutb.write((char*) &n, sizeof(n));    // Записываем n в двоичном режиме
            for (auto& pos : Employees) write_binary(foutb, pos);
            foutb.close();      // Закрываем файл
        }else {
            cerr << "Error: second parameter must be text or binary" << endl;
            return 5;    //Возвращаем код ошибки
        }
    }else if (mode == "read") {     // Чтение из файла
        if (open_mode == "text") {
            ifstream fint(file_name);       // Открываем файл для чтения в текстовом режиме
            if (!fint) {    // Ошибка файл не открыт
                cout << endl << "Error, file not found" << endl;
                return 1;    //Возвращаем код ошибки
            }
            fint.seekg(0, ios::end); // Переходим в конец файла, чтобы узнать его размер
            long size = (long)fint.tellg(); // Размер файла в байтах
            fint.seekg(0); // Возвращаемся в начало файла
            fint >> n; // Читаем n в текстовом режиме
            if (!fint || n < 0 || n > size) {       // Не число, отрицательное или больше размера файла
                cout << endl << "Error, wrong file format" << endl;
                return 1;   //Возвращаем код ошибки
            }
            Employees.resize(n); // Задаем размер массива
            for (auto & pos : Employees) read_text(fint, pos);
            if (!fint) {// Данные в файле закончились раньше времен
                cout << endl << "Error, wrong file format" << endl;
                return 1;
            }fint.close();      // Закрываем файл
        }else if (open_mode == "binary") {
            ifstream finb(file_name, ios::binary);      // Открываем файл для чтения в двоичном режиме
            if (!finb) {        // Ошибка файл не открыт
                cout << endl << "Error, file not found" << endl;
                return 1;
            }
            finb.seekg(0, ios::end);     // Переходим в конец файла, чтобы узнать его размер
            long size = (long)finb.tellg();     // Размер файла в байтах
            finb.seekg(0);      // Возвращаемся в начало файла
            finb.read((char*)&n, sizeof(n));     // Читаем n в двоичном режиме
            if (!finb || n < 0 || n > size) {       // Не прочиталось, отрицательное или больше размера файла
                cout << endl << "Error, wrong file format" << endl;
                return 1;
            }
            Employees.resize(n);        // Задаем размер массива
            for (auto & pos : Employees) read_binary(finb, pos);
            if (!finb) {        // Данные в файле закончились раньше времени
                cout << endl << "Error, wrong file format" << endl;
                return 1;
            }
            finb.close();       // Закрываем файл
        }else {
            cout << "Error: second parameter must be text or binary" << endl;
            return 5;
        }       // Печатаем сотрудников, прочитанных из файла
        print_employee(cout, Employees);
    }else {
        cout << "Error: first parameter must be write or read" << endl;
        return 5;
    }
    return 0;
}

