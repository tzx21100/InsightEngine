#ifndef GAM200_INSIGHT_ENGINE_SOURCE_JSON_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_JSON_H_

#include <json/json.h>  // Make sure to link against JsonCpp
#include <string>

namespace IS {

    bool SaveJsonToFile(const Json::Value& root, const std::string& filename);
    bool LoadJsonFromFile(Json::Value& root, const std::string& filename);

}

//Example
//int main() {
//    // Create a sample JSON object
//    Json::Value root;
//    root["name"] = "John";
//    root["age"] = 30;
//    root["is_alive"] = true;
//
//    // Save to file
//    if (saveJsonToFile(root, "data.json")) {
//        std::cout << "Saved JSON data to data.json" << std::endl;
//    }
//
//    // Load from file
//    Json::Value loadedRoot;
//    if (loadJsonFromFile(loadedRoot, "data.json")) {
//        std::cout << "Loaded JSON data from data.json" << std::endl;
//        std::cout << "Name: " << loadedRoot["name"].asString() << std::endl;
//        std::cout << "Age: " << loadedRoot["age"].asInt() << std::endl;
//        std::cout << "Is Alive: " << (loadedRoot["is_alive"].asBool() ? "Yes" : "No") << std::endl;
//    }
//
//    return 0;
//}


#endif