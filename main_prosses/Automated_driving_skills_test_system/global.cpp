#include "global.h"

global::global() {
}

void global::print(const std::string& message)
{
    std::lock_guard<std::mutex> lock(mtxprint);
    std::cout << message << std::endl;
};