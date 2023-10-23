/*!
 * \file JsonSaveLoad.cpp
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 * This header file defines the 2 functions for saving and loading to json files
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "JsonSaveLoad.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace IS {

    // It will create a directory at the path if its not found, so it needs a directory
    bool SaveJsonToFile(const Json::Value& root, const std::string& filename) {
        // Extract directory path from filename
        std::filesystem::path filePath(filename);
        std::filesystem::path dirPath = filePath.parent_path();

        // Check if the directory exists
        if (std::filesystem::is_directory(dirPath) && !std::filesystem::exists(dirPath)) {
            // If it doesn't exist, create the directory

            std::filesystem::create_directories(dirPath);   
            if (!std::filesystem::exists(dirPath)) {
                IS_CORE_DEBUG("FAILED TO CREATE A DIRECTORY");
                return false;
            }
        }
        // Create the file
        std::ofstream file(filename, std::ofstream::binary);
        if (!file.is_open()) {
            IS_CORE_DEBUG("COULD NOT OPEN FILE FOR WRITING");
            return false;
        }

        // Write with the library
        Json::StreamWriterBuilder writer;
        std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
        jsonWriter->write(root, &file);

        return true;
    }

    // Read the json file
    bool LoadJsonFromFile(Json::Value& root, const std::string& filename) {
        std::ifstream file(filename, std::ifstream::binary);
        if (!file.is_open()) {
            IS_CORE_DEBUG("COULD NOT OPEN FILE FOR READING");
            return false;
        }

        // Followed documentation
        Json::CharReaderBuilder builder;
        builder["collectComments"] = true;
        JSONCPP_STRING errs;
        bool ok = Json::parseFromStream(builder, file, &root, &errs);
        if (!ok) {
            IS_CORE_DEBUG("FAILED TO PARSE JSON");
            return false;
        }
        

        return true;
    }

}
