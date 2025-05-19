#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <mutex>
#include <iostream>

class global {
private:
    std::mutex mtxprint;
public:
    global();
    void print(const std::string& message);
};

#endif