#ifndef GLOBAL_H
#define GLOBAL_H
#include <windows.h>
#include <string>
#include <mutex>
#include <iostream>
using namespace std;
#undef min 
class globalFunc {
private:
public:
    static std::mutex mtxprint;

    globalFunc();
    void print(const std::string& message);
    void printError(const std::string& message);
    string getLatestFile(const string& folderPath);
};
#endif // GLOBAL_H