#ifndef DRIVINGROUTE_H
#define DRIVINGROUTE_H
#include <windows.h>

#include <string>
#include <vector>
#include "globalFunc.h"
#include <nlohmann/json.hpp>
#include <mutex>
//using namespace std;

class DrivingRoute {
public:
    DrivingRoute() {};
    //DrivingRoute(const std::string& apiKey);
    void getDirections(const std::string& origin, const std::string& destination);
    std::vector<std::string> getInstructions() const;

private:
    static const std::string apiKey_;
    std::vector<std::string> instructions_;
    globalFunc globalPrint;

};

#endif