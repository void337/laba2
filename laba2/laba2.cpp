#include "Header.h"  
#include <iostream>  

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    cout << "Введите память в одном блоке\n"; int byte; cin >> byte;
    while (!cin || cin.peek() != '\n' || byte <= 0)
    {
        clean();
        cout << "Введите память в одном блоке\n"; cin >> byte;
    }
    cout << "Введите количество блоков\n"; int memory; cin >> memory;
    while (!cin || cin.peek() != '\n' || memory <= 0)
    {
        clean();
        cout << "Введите количество блоков\n"; cin >> memory;
    }

    FileSystem file_system(byte, memory);
    int choise, n, a;
    string s;
    while (true) {
        cout << "Выберите действие: \nFREE - 1\nWRITE - 2\nREAD - 3\nSTART_TRANSACTION - 4\nEND_TRANSACTION - 5\nROLLBACK - 6\nINFO - 7\nEND PROGRAMM - 8\n";
        cin >> choise;
        while (!cin || cin.peek() != '\n' || choise < 1 || choise > 8)
        {
            clean();
            cout << "Выберите действие: FREE - 1\nWRITE - 2\nREAD - 3\nSTART_TRANSACTION - 4\nEND_TRANSACTION - 5\nROLLBACK - 6\nINFO - 7\nEND PROGRAMM - 8\n";
            cin >> choise;
        }
        switch (FileSystem::CHOISE(choise))
        {
        case FileSystem::CHOISE::FREE:
            cout << "Введите номер блока и количество блоков: "; cin >> n >> a;
            while (!cin || cin.peek() != '\n' || n < 0 || a < 0)
            {
                clean();
                cout << "Введите номер блока и количество блоков: "; cin >> n >> a;
            }
            file_system.free(n, a);
            break;
        case FileSystem::CHOISE::WRITE:
            cout << "Введите номер блока и количество блоков: "; cin >> n >> a;
            while (!cin || cin.peek() != '\n' || n < 0 || a < 0)
            {
                clean();
                cout << "Введите номер блока и количество блоков: "; cin >> n >> a;
            }
            cout << "Введите данные\n";
            cin.ignore();
            getline(cin, s);
            file_system.write(s, n, a);
            break;
        case FileSystem::CHOISE::READ:
            cout << "Введите номер блока и количество блоков: "; cin >> n >> a;
            while (!cin || cin.peek() != '\n' || n < 0 || a < 0)
            {
                clean();
                cout << "Введите номер блока и количество блоков: "; cin >> n >> a;
            }
            file_system.read(n, a);
            break;
        case FileSystem::CHOISE::START_TRANSACTION:
            file_system.start_transaction();
            break;
        case FileSystem::CHOISE::END_TRANSACTION:
            file_system.end_transaction();
            break;
        case FileSystem::CHOISE::ROLLBACK:
            file_system.rollback_transaction();
            break;
        case FileSystem::CHOISE::INFO:
            file_system.info();
            break;
        case FileSystem::CHOISE::END:
            cout << "Хорошего Вам дня :)\n";
            return 0;
            break;
        default:
            cout << "Введите один из вариантов\n";
            break;
        }
    }
}