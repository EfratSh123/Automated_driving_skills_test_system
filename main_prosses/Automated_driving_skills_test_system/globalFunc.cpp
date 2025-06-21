#include "globalFunc.h"
#include <iostream>
#include "Car.h"
#include <iostream>
#include <cmath>
#include <chrono> 
#include <functional>
#include <fstream>
#include <string>[
#include <sstream>
#include <unordered_map>
#include <filesystem>
#include <windows.h>
namespace fs = std::filesystem;
#include <filesystem>
#include <windows.h>

std::mutex globalFunc::mtxprint; // <-- Add this line

globalFunc::globalFunc() {}
void globalFunc::print(const std::string& message)
{
    std::lock_guard<std::mutex> lock(mtxprint);
    std::cout << message << '\n \n' << std::endl;
};
void globalFunc::printError(const std::string& message)
{
    std::lock_guard<std::mutex> lock(mtxprint);
    std::cerr << "ERROR: " << message << '\n \n' << std::endl;
};

// פונקציה לקבלת הנתיב של הקובץ האחרון שנוצר בתיקייה
string globalFunc::getLatestFile(const string& folderPath) {
    fs::path latestFile;
    // יש לוודא ש-latestWriteTime מאותחל בצורה שתבטיח שכל קובץ חוקי יהיה "מאוחר" ממנו
    auto latestWriteTime = fs::file_time_type::min();
	// בדוק אם הנתיב קיים ואם הוא תיקייה
    if (fs::exists(folderPath) && fs::is_directory(folderPath)) {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            // וודא שזהו קובץ רגיל
            if (fs::is_regular_file(entry.path())) {
                // מניעת קריאת קבצים זמניים
                if (entry.path().extension() != ".tmp") {
                    // קבל את זמן הכתיבה של הקובץ
                    auto writeTime = fs::last_write_time(entry.path());
                    // אם זמן הכתיבה של הקובץ הנוכחי מאוחר יותר מהקובץ האחרון שנמצא, עדכן את הנתיב והזמן
                    if (writeTime > latestWriteTime) {
                        latestWriteTime = writeTime;
                        latestFile = entry.path();
                    }
                }
            }
        }
    }
    return latestFile.string();
}