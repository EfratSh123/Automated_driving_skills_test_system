cmake_minimum_required(VERSION 3.10) # או גרסה מתאימה של CMake אצלך
project(GoogleMapsDirections)

# הגדרת דגל סטנדרט C++17 (מומלץ עבור ספריות מודרניות)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- חיפוש והוספת ספריית cpr ---
find_package(cpr CONFIG REQUIRED)
if(TARGET cpr::cpr)
    message(STATUS "Found cpr library")
else()
    message(FATAL_ERROR "cpr library not found. Please install it (e.g., using vcpkg or a similar package manager).")
endif()

# --- חיפוש והוספת ספריית nlohmann-json ---
find_package(nlohmann_json CONFIG REQUIRED)
if(TARGET nlohmann_json::nlohmann_json)
    message(STATUS "Found nlohmann-json library")
else()
    message(FATAL_ERROR "nlohmann-json library not found. Please install it (e.g., using vcpkg or a similar package manager).")
endif()

# --- הוספת ספריית polyline (header-only) ---
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include) # הנחה ש-polyline.hpp נמצא בתיקיית "include"

# --- הוספת קובץ הביצועה ---
add_executable(google_maps_app main.cpp)

# --- קישור הספריות לקובץ הביצועה ---
target_link_libraries(google_maps_app PRIVATE cpr::cpr nlohmann_json::nlohmann_json)