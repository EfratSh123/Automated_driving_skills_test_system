#include "RoadSpeedInfo.h"
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <iostream>
#include <sstream>
#include <pplx/pplxtasks.h> // Ensure this header is included for pplx::task
#include <cpprest/json.h>   // For web::json::value

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::json;
using namespace utility;
using namespace std;
using namespace pplx; 

RoadSpeedInfo::RoadSpeedInfo() : apiKey("AIzaSyDY8eUa9WUDRDnY215UaTuplCddBLzSShE") {}
RoadSpeedInfo::~RoadSpeedInfo() {}

// Private function to send an asynchronous HTTP GET request and receive the response as JSON
pplx::task<web::json::value> sendHttpRequest(const std::wstring& url) {
	http_client client(url);
	return client.request(methods::GET).then([](http_response response) {
		utility::string_t response_string = response.to_string();
		std::cerr << "Response Status Code: " << response.status_code() << std::endl;
		std::cerr << "Response Body: " << utility::conversions::to_utf8string(response_string) << std::endl;
		if (response.status_code() == status_codes::OK) {
			return response.extract_json();
		}
		else {
			return task_from_result(value::null());
		}
		});
}// Public function to get the speed limit for given latitude and longitude
pplx::task<std::pair<bool, int>> RoadSpeedInfo::getSpeedLimit(double latitude, double longitude) {
	std::wstring apiUrl;
	wstringstream ss;
	// url for the speed limit API
	ss << L"https://roads.googleapis.com/v1/speedLimits";
	// setting of latitude and longitude
	//uri_builder builder(ss.str());
	std::wstring query = L"path=" + utility::conversions::to_string_t(std::to_string(latitude) + "," + std::to_string(longitude)) + L"&key=" + utility::conversions::to_string_t(apiKey);
	uri_builder builder(ss.str() + L"?" + query);

	//builder.append_query(L"locations", utility::conversions::to_string_t(std::to_string(latitude) + "," + std::to_string(longitude)));
	//builder.append_query(L"key", utility::conversions::to_string_t(apiKey));
	// Sends the HTTP request and processes the response
	std::wcout << L"Request URL: " << builder.to_uri().to_string() << std::endl;
	return sendHttpRequest(builder.to_uri().to_string()).then([](value jsonValue) {
		if (!jsonValue.is_null()) {
			try {
				// Tries to extract the speed limit from
				if (jsonValue.has_field(L"speed_limit") && jsonValue[L"speed_limit"].is_number()) {
					return std::make_pair(true, jsonValue[L"speed_limit"].as_number().to_int32());
				}
				else if (jsonValue.has_field(L"max_speed_kmh") && jsonValue[L"max_speed_kmh"].is_integer()) {
					return std::make_pair(true, jsonValue[L"max_speed_kmh"].as_integer());
				}
				else {
					std::cerr << "Error: Could not find expected speed limit field in JSON response." << std::endl;
					std::cerr << "Response JSON: " << utility::conversions::to_utf8string(jsonValue.serialize()) << std::endl;
					return std::make_pair(false, -1);
				}
			}
			catch (const std::exception& e) {
				std::cerr << "Error parsing JSON response: " << e.what() << " - " << utility::conversions::to_utf8string(jsonValue.serialize()) << std::endl;
				return std::make_pair(false, -1);
			}
		}
		else {
			return std::make_pair(false, -1);
		}
		});
}