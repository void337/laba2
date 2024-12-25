#pragma once 
#ifndef HEADER_H 
#define HEADER_H 
#include <iostream> 
#include <Windows.h> 
#include <string> 
#include <vector> 
#include <fstream> 
#include <utility> 
#include <map> 
#include <iomanip>  
#include <sstream>  

using namespace std;

class FileSystem {

    struct Block {
        int id;
        bool allocate;
        string data;
        static int size;
        Block();
        string info();
    };

    struct File {
        string name;
        string description;
        string date;
        int startBlock;
        int size;
        fstream* FILE;
        File(fstream& f);
        ~File();
    };

    struct Cache {
        vector<vector<FileSystem::Block>> cache;
        static int size;
        ~Cache();
        string info();
        int get_size();
        void add(int num, FileSystem::Block f);
    };

    vector<Block> blocks;
    vector<pair<int, int>> free_blocks;

    Cache cache_fs;
    int size;
    int allocate_size;
    int number_of_blocks;
    int number_of_free_blocks;
    bool flag_tr;

public:
    FileSystem(int block_syze, int num);
    void free(int first, int num);
    void write(string s, int first, int num);
    void read(int first, int num);
    void start_transaction();
    void end_transaction();
    void rollback_transaction();
    void update_free_blocks();
    void update_blocks();
    void info();

    enum class CHOISE { FREE = 1, WRITE, READ, START_TRANSACTION, END_TRANSACTION, ROLLBACK, INFO, END };

};

void clean();

#endif