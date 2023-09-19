#ifndef GAM200_INSIGHT_ENGINE_SOURCE_JSON_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_JSON_H_

#include <iostream>
#include <fstream>
#include <json/json.h>  // Make sure to link against JsonCpp
#include <string>
#include <filesystem>

namespace IS {


    bool SaveJsonToFile(const Json::Value& root, const std::string& filename) {
        std::ofstream file(filename, std::ofstream::binary);
        if (!file.is_open()) {
            std::cout << "Current path is " << std::filesystem::current_path() << '\n';
            std::cerr << "Could not open file for writing: " << filename << std::endl;
            return false;
        }

        Json::StreamWriterBuilder writer;
        std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
        jsonWriter->write(root, &file);

        return true;
    }

    bool LoadJsonFromFile(Json::Value& root, const std::string& filename) {
        std::ifstream file(filename, std::ifstream::binary);
        if (!file.is_open()) {
            std::cerr << "Could not open file for reading: " << filename << std::endl;
            return false;
        }

        Json::CharReaderBuilder reader;
        std::string errs;
        if (!Json::parseFromStream(reader, file, &root, &errs)) {
            std::cerr << "Failed to parse JSON: " << errs << std::endl;
            return false;
        }

        return true;
    }
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