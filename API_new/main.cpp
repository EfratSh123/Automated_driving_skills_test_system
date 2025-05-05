#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <polyline/polylineencoder.h>

using json = nlohmann::json;

int main() {
    std::string apiKey ="AIzaSyAk14K2nn4dxIhkAekB7bwtyuFedMp1OEg";
    std::string origin = "32.751152,35.138711"; // שעורה 2, רכסים
    std::string destination = "32.753594,35.138342"; // נרקיסים 8, רכסים

    std::string url = "https://maps.googleapis.com/maps/api/directions/json"
                        "?destination=" + destination +
                        "&origin=" + origin +
                        "&key=" + apiKey;

    cpr::Response r = cpr::Get(cpr::Url{url});

    if (r.status_code != 200 || json::parse(r.text)["status"] != "OK") {
        std::cerr << "Error getting directions." << std::endl;
        return 1;
    }

    json response = json::parse(r.text);
    auto steps = response["routes"][0]["legs"][0]["steps"];

    std::cout << "Route Coordinates:" << std::endl;
    for (const auto& step : steps) {
        std::string encodedPolyline = step["polyline"]["points"];
        std::vector<std::pair<double, double>> decodedPolylinePoints; // וקטור לנקודות מפוענחות
    
        gepaf::PolylineEncoder<>::Polyline polylineResult = gepaf::PolylineEncoder<>::decode(encodedPolyline);
    
        std::cout << "Step Coordinates:" << std::endl;
        for (const gepaf::PolylineEncoder<>::Point& point : polylineResult) {
            std::cout << point.latitude() << ", " << point.longitude() << std::endl;
            decodedPolylinePoints.push_back({point.latitude(), point.longitude()}); // אם את עדיין צריכה את הוקטור של pairs
        }
        std::cout << "---" << std::endl;
    }

    return 0;
}