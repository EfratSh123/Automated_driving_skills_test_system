#ifndef DRIVINGROUTE_H
#define DRIVINGROUTE_H

#include <string>
#include <vector>
#include "globalFunc.h"
#include <nlohmann/json.hpp>
#include <mutex>
//using namespace std;

class DrivingRoute {
public:
    DrivingRoute(const std::string& apiKey);
    void getDirections(const std::string& origin, const std::string& destination);
    std::vector<std::string> getInstructions() const;

private:
    std::string apiKey_;
    std::vector<std::string> instructions_;
    globalFunc globalPrint;

};

#endif