#pragma once
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
        void Start(std::string& directory) {
            for (const auto& file : std::filesystem::directory_iterator(directory)) {
                if (std::filesystem::is_regular_file(file.path()) && file.path().extension() == ".cs") {
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
            }
        }

        void Stop() {
            running_ = false;
        }
    };
}


