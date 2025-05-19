#include "DrivingRoute.h"
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <algorithm>

using json = nlohmann::json;

DrivingRoute::DrivingRoute(const std::string& apiKey) : apiKey_(apiKey) {}

void DrivingRoute::getDirections(const std::string& origin, const std::string& destination) {
    std::string url = "https://maps.googleapis.com/maps/api/directions/json"
        "?destination=" + destination +
        "&origin=" + origin +
        //"?waypoints=" + waypoints +
        //"?language=iw"
        "&key=" + apiKey_;

	globalPrint.print("URL: " + url);
    cpr::Response r = cpr::Get(cpr::Url{ url });
	globalPrint.print("Response Code: " + std::to_string(r.status_code));

    if (r.status_code != 200) {
		globalPrint.print("URL: " + url);
		globalPrint.printError("error in getting path: " + std::to_string(r.status_code));
        globalPrint.printError("Response Body (if any): " + r.text);
        return;
    }

    json response = json::parse(r.text);
    if (response["status"] != "OK") {
        globalPrint.printError("Google API Error: " + response["status"]);
        return;
    }

    auto steps = response["routes"][0]["legs"][0]["steps"];
    instructions_.clear();

	globalPrint.print("driving rules: ");
    for (const auto& step : steps) {
        std::string instruction = step["html_instructions"];
        std::string distance = step["distance"]["text"];

        instruction.erase(std::remove_if(instruction.begin(), instruction.end(), [](char c) { return c == '<' || c == '>'; }), instruction.end());

		globalPrint.print("in: " + distance + " - " + instruction);
        instructions_.push_back("in: " + distance + " - " + instruction);
    }
}

std::vector<std::string> DrivingRoute::getInstructions() const {
    return instructions_;
}