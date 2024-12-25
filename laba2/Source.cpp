#include "Header.h" 
#include <iostream> 

int FileSystem::Block::size;
int FileSystem::Cache::size;

FileSystem::File::File(fstream& f)
{

}

FileSystem::File::~File()
{
}

FileSystem::FileSystem(int block_syze, int num)
{
    if (block_syze > 8 || (block_syze & (block_syze - 1)) != 0) {
        cout << "Размер блока превышает 8 или не является степенью двойки. Задано оптимальное значение = 8 байт\n"; block_syze = 8;
    }
    if (num * block_syze > 64 || (num * block_syze & (num * block_syze - 1)) != 0) {
        cout << "Память файловой системы больше 64 или не равно степени двойки. Задано оптимальное значение памяти файловой системы = 64 байт\n";
        num = 64 / block_syze;
    }
    for (int i = 0; i < num; i++) {
        Block b; b.id = i; blocks.push_back(b);
    }
    size = num * block_syze; number_of_blocks = num; flag_tr = false;
    allocate_size = 0;
    Block::size = block_syze;
    update_free_blocks();
    update_blocks();
}


void FileSystem::free(int first, int num)
{
    int size1 = Cache::size;
    if (first + num < number_of_blocks) {
        for (int i = first; i < first + num; i++) {
            if (flag_tr && blocks[i].allocate == true) cache_fs.add(size1 + 1, blocks[i]);
            blocks[i].allocate = false;
            blocks[i].data = "";
        }
    }
    else cout << "Данной цепочки блоков не существует!\n";
    update_free_blocks();
    update_blocks();

}

void FileSystem::write(string s, int first, int num)
{
    vector<int> f1; int size1 = Cache::size;
    bool f = true;
    int c = 0;
    if (first + num > number_of_blocks) f = false;
    else for (int i = first; i < first + num; i++) if (blocks[i].allocate == true) f = false;

    if (f) {
        for (int i = first; i < first + num; i++) {
            if (flag_tr) cache_fs.add(size1 + 1, blocks[i]);
            blocks[i].allocate = true;
            blocks[i].data = s;
            f1.push_back(i);
            c++;
        }
        update_free_blocks();
        update_blocks();
        allocate_size += c * Block::size;
    }
    else cout << "Данной цепочки блоков не существует!\n";
}

void FileSystem::read(int first, int num)
{
    cout << "\n| Номер блока | Свободен | Данные | Размер |\n";
    for (int i = 0; i < free_blocks.size(); i++) {
        if (free_blocks[i].first == first && free_blocks[i].second == num)
            for (int i = first; i < first + num; i++)
                cout << blocks[i].info();
        else cout << "Данной цепочки блоков не существует!\n";
    }
}

void FileSystem::start_transaction() { flag_tr = true; }

void FileSystem::end_transaction() { if (flag_tr) { cache_fs.~Cache(); flag_tr = false; } }

void FileSystem::rollback_transaction() {
    if (flag_tr) {
        vector<FileSystem::Block> g;
        for (auto i : *(cache_fs.cache.end() - 1)) g.push_back(i);
        cache_fs.cache.erase(cache_fs.cache.end() - 1);
        Cache::size--;
        for (int i = 0; i < g.size(); i++) blocks[g[i].id] = g[i];
    }
    update_free_blocks();
    update_blocks();
}

void FileSystem::update_free_blocks()
{
    free_blocks.clear();
    pair<int, int> free;
    bool first = true; int count = 0; int fr = 0;
    for (int i = 0; i < number_of_blocks; i++) {
        if (!blocks[i].allocate && first) {
            free.first = i; first = false; count++;
        }
        else if (!blocks[i].allocate && !first)  count++;
        else if (blocks[i].allocate && !first) {
            free.second = count;
            free_blocks.push_back(free);
            fr += count;
            free.first = 0; free.second = 0;
            first = true; count = 0;
        }
    }
    if (!first) {
        free.second = count;
        free_blocks.push_back(free);
        fr += count;
    }
    number_of_free_blocks = fr;
}
void FileSystem::update_blocks()
{
    fstream file("BLOCKSPACE.txt", ios::out | ios::in | ios::trunc);
    for (int i = 0; i < free_blocks.size(); i++)
        file << "(" + to_string(free_blocks[i].first) + "; " + to_string(free_blocks[i].second) + "); ";
    file << "\n";
    for (int i = 0; i < number_of_blocks; i++)
        if (blocks[i].allocate)
            file << i << " " << blocks[i].data << "\n";


}
void FileSystem::info()
{
    fstream file("file.txt", ios::out | ios::in | ios::trunc);
    if (!file || !file.is_open()) cerr << "Ошибка при открытии файла" << endl;
    file << "Информация о файловой системе:\n";
    file << "Всего памяти: " + to_string(size) + "\n";
    file << "Количество занятой памяти: " + to_string(allocate_size) + "\n";
    file << "Общее количество блоков: " + to_string(number_of_blocks) + "\n";
    stringstream ss;
    int idWidth = ((int)string("| Номер блока |").length());
    int allocateWidth = ((int)string("| Свободен |").length());
    ss << "|";
    ss << setw(idWidth) << left << "Номер блока" << "|";
    ss << setw(allocateWidth) << left << "Свободен" << "|";
    ss << setw(40) << left << "Данные" << "|";
    ss << setw(10) << left << "Размер" << "|\n";

    file << ss.str();
    for (int i = 0; i < number_of_blocks; i++) file << blocks[i].info();
    file << "Общее количество пустых блоков: " + to_string(number_of_free_blocks) + "\n";
    file << "Список цепочек связных блоков:\n";
    for (int i = 0; i < free_blocks.size(); i++)
        file << "(" + to_string(free_blocks[i].first) + "; " + to_string(free_blocks[i].second) + "), ";
    file << "\nИнформация о кеше:\n";
    file << ss.str();
    file << cache_fs.info() << endl;
    file.seekg(0);
    string line;
    while (getline(file, line)) cout << line << endl;
    file.close();
}

FileSystem::Block::Block() { id = 0; allocate = false; data = ""; }

string FileSystem::Block::info()
{
    stringstream ss;
    int idWidth = to_string(id).length();
    int allocateWidth = 8;
    int dataWidth = data.length();
    int sizeWidth = to_string(Block::size).length();
    idWidth += 2;
    allocateWidth += 2;
    dataWidth += 2;
    sizeWidth += 2;
    idWidth = max(idWidth, (int)string("| Номер блока |").length());
    allocateWidth = max(allocateWidth, (int)string("| Свободен |").length());
    dataWidth = 40;
    sizeWidth = 10;

    ss << "|";
    ss << setw(idWidth) << left << id << "|";

    ss << setw(allocateWidth) << left << (allocate ? "Нет" : "Да") << "|";

    ss << setw(dataWidth) << left << (data.empty() ? "---" : data) << "|";

    ss << setw(sizeWidth) << left << size << "|\n";

    return ss.str();
}

FileSystem::Cache::~Cache() { cache.clear(); }

string FileSystem::Cache::info()
{
    string s;
    for (int i = 0; i < cache.size(); i++) {
        for (int j = 0; j < cache[i].size(); j++) s += cache[i][j].info();
    }
    s += "\nКоличество байт, занятых кешем: " + to_string(get_size() * FileSystem::Block::size) + "\n";
    return s;
}

void FileSystem::Cache::add(int num, FileSystem::Block f) {
    if (Cache::size < num) {
        vector<FileSystem::Block> q;
        cache.push_back(q);
        Cache::size++;
    }
    cache[num - 1].push_back(f);
}

int FileSystem::Cache::get_size() {
    return cache.size();
}

void clean() {
    cin.clear();
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    cout << "Error!" << endl;
}