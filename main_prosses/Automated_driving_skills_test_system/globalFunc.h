#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <mutex>
#include <iostream>
using namespace std;

class globalFunc {
private:
    std::mutex mtxprint;
public:
    globalFunc();
    void print(const std::string& message);
    void printError(const std::string& message);
};
#endif // GLOBAL_H