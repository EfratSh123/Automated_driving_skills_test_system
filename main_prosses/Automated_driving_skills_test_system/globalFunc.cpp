#include "globalFunc.h"

globalFunc::globalFunc() {
}

void globalFunc::print(const std::string& message)
{
    std::lock_guard<std::mutex> lock(mtxprint);
    std::cout << message << '\n' << std::endl;
};

void globalFunc::printError(const std::string& message)
{
    std::lock_guard<std::mutex> lock(mtxprint); 
    std::cerr << "ERROR: " << message << '\n' << std::endl;
};