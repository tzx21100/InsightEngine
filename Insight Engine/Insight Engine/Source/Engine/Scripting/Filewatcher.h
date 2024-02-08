/*!
* \file FileWatcher.h
* \author  Tan Zheng Xun, t.zhengxun@digipen.edu
* \par Course: CSD2401
* \date  08-02-2024
* \brief
* This header file declares the FileWatcher and the files it will watch
* this relies heavily on filesystem to mantain and watchover the files its responsible for.
*
* \copyright
* All content (C) 2023 DigiPen Institute of Technology Singapore.
* All rights reserved.
* Reproduction or disclosure of this file or its contents without the prior written
* consent of DigiPen Institute of Technology is prohibited.
*____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_SCENE_FILEWATCHER_H
#define GAM200_INSIGHT_ENGINE_EDITOR_SCENE_FILEWATCHER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
//this is the filewatcher class meant for hot reloading
#include <filesystem>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <thread>
#include <cstdlib>
#include "ScriptEngine.h"
#include "../Engine/Scripting/Filewatcher.h"

namespace IS {

    int WATCH_STUFF = 0;
    class FileWatcher {
    public:
        // Paths for areas to watch
        std::unordered_map<std::string, std::filesystem::file_time_type> mPaths;
        bool running_ = true;
        bool initial_scan_complete_ = false;  // Flag to track the initial scan

        // Function to call when a change is detected
        void OnChange() {
            // Hot reload here
            ScriptEngine::Reload();
        }

        // Start monitoring files (single-threaded)
        void Start(std::string& directory , std::string &directory2) {

            if (!initial_scan_complete_) {
                for (const auto& file : std::filesystem::directory_iterator(directory)) {
                    if (std::filesystem::is_regular_file(file.path()) && file.path().extension() == ".cs") {
                        std::string file_path = file.path().string();
                        // Only record the modification time for files that existed before the scan started
                        if (!initial_scan_complete_ || mPaths.find(file_path) == mPaths.end()) {
                            mPaths[file_path] = std::filesystem::last_write_time(file);
                        }
                    }
                }

                //directory 2
                for (const auto& file : std::filesystem::directory_iterator(directory2)) {

                    std::string file_path = file.path().string();
                    // Only record the modification time for files that existed before the scan started
                    if (!initial_scan_complete_ || mPaths.find(file_path) == mPaths.end()) {
                        mPaths[file_path] = std::filesystem::last_write_time(file);
                    }
                    
                }

            }

            initial_scan_complete_ = true;  // Set the flag to indicate the initial scan is complete

            // Monitor for changes after the initial scan
            if (running_) {

                WATCH_STUFF++;
                if (WATCH_STUFF < 10) {
                    return;
                }
                else {
                    WATCH_STUFF = 0;
                }

                for (const auto& file : std::filesystem::directory_iterator(directory)) {
                    if (std::filesystem::is_regular_file(file.path()) && file.path().extension() == ".cs") {
                        std::string file_path = file.path().string();
                        auto current_time = std::filesystem::last_write_time(file_path);
                        if (mPaths.find(file_path) != mPaths.end() && mPaths[file_path] != current_time) {
                            mPaths[file_path] = current_time;
                            OnChange();
                            InsightEngine::Instance().mRuntime = false;
                            auto sys = InsightEngine::Instance().GetSystem<ScriptManager>("ScriptManager");
                            sys->ReloadAllScriptClasses();
                            

                        }
                    }
                }

                for (const auto& file : std::filesystem::directory_iterator(directory2)) {
                    std::string file_path = file.path().string();
                    auto current_time = std::filesystem::last_write_time(file_path);
                    if (mPaths.find(file_path) != mPaths.end() && mPaths[file_path] != current_time) {
                        mPaths[file_path] = current_time;
                        Shader::deleteAllShaders();
                        Shader::compileAllShaders();
                        std::cout << "SHADER COMPILED" << std::endl;
                    }
                    
                }

            }
        }

        void Stop() {
            running_ = false;
        }
    };
}


#endif