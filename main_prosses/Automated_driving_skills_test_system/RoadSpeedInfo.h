#ifndef ROADSPEEDINFO_H
#define ROADSPEEDINFO_H

#include <string>
#include <future>
#include <pplx/pplxtasks.h>
#include <cpprest/json.h>
#include "globalFunc.h"
#include <mutex>
using namespace std;

class RoadSpeedInfo {
public:
    RoadSpeedInfo();
     //pplx::task<web::json::value> sendHttpRequest(const std::wstring& url); // Add this line here
     pplx::task<std::pair<bool, int>> getSpeedLimit(double latitude, double longitude);
    //pplx::task<web::json::value> sendHttpRequest(const std::wstring& url); // Remove this line
private:
    string apiKey;
    globalFunc globalPrint;

};

#endif // ROADSPEEDINFO_H