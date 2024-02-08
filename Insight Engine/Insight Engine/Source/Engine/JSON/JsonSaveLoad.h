/*!
 * \file JsonSaveLoad.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 * This header file declares the 2 functions for saving and loading to json files
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_JSON_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_JSON_H_

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include <json/json.h>  // Make sure to link against JsonCpp
#include <string>

namespace IS {
     /**
     * \brief Saves a given JSON value to a file.
     *
     * This function saves the provided JSON value to a specified file. If the directory
     * in which the file resides doesn't exist, it attempts to create the necessary directories.
     * It uses the JsonCpp library for writing the JSON data.
     *
     * \param root The JSON value to be saved.
     * \param filename The path and name of the file where the JSON should be saved.
     *
     * \return Returns true if the file was successfully saved, otherwise returns false.
     *
     * \note The function will overwrite the file if it already exists.
     *       Errors during the save process, such as issues with directory creation or file
     *       writing, will be printed to the IS_CORE_DEBUG.
     */
    bool SaveJsonToFile(const Json::Value& root, const std::string& filename);

    /**
     * \brief Loads a JSON value from a specified file.
     *
     * This function attempts to read a file and parse its contents as JSON. It uses the
     * JsonCpp library for reading and parsing the JSON data.
     *
     * \param[out] root The JSON value where the read data will be stored.
     * \param filename The path and name of the file to be read.
     *
     * \return Returns true if the file was successfully read and parsed, otherwise returns false.
     *
     * \note If there are issues during the reading or parsing process, such as file not
     *       being found or JSON parsing errors, they will be printed to the IS_CORE_DEBUG.
     */
    bool LoadJsonFromFile(Json::Value& root, const std::string& filename);

}

//Example on how to use the functions
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