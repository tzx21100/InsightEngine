#include "Pch.h"
#include "JsonSaveLoad.h"

#include <iostream>
#include <fstream>
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

        Json::CharReaderBuilder builder;
        builder["collectComments"] = true;
        JSONCPP_STRING errs;
        try {
            bool ok = Json::parseFromStream(builder, file, &root, &errs);
            if (!ok) {
                std::cerr << "Failed to parse JSON: " << errs << std::endl;
                return false;
            }
        } catch (std::exception const& e) {
            std::cerr << e.what() << std::endl;
        }

        return true;
    }

}
