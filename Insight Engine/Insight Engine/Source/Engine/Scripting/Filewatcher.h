#pragma once
//this is the filewatcher class meant for hot reloading
#include <filesystem>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <thread>
#include <cstdlib>

namespace IS {




    class FileWatcher {
    public:

        // paths for areas to watch
        std::unordered_map<std::string, std::filesystem::file_time_type> mPaths;
        bool running_ = true;
        std::thread watcher_thread_;

        // Check if a file has been created or modified
        bool FileModified(const std::string& path) {
            auto current_time = std::filesystem::last_write_time(path);
            if (mPaths.find(path) == mPaths.end() || mPaths[path] != current_time) {
                mPaths[path] = current_time;
                return true;
            }
            return false;
        }

        // Function to call when a change is detected
        void OnChange(const std::string& path) {
            //hot reload here
            isHotReloading = true;
            std::cout << "File changed: " << path << std::endl;
            std::cout << "HotReload changed: " << isHotReloading << std::endl;
        }

        // Start monitoring files
        void Start(const std::string& directory, std::chrono::duration<int, std::milli> interval) {
            // Initialize the file list
            for (const auto& file : std::filesystem::directory_iterator(directory)) {
                if (std::filesystem::is_regular_file(file.path())) {
                    mPaths[file.path().string()] = std::filesystem::last_write_time(file);
                }
            }

            watcher_thread_ = std::thread([this, directory, interval]() {
                while (running_) {
                    std::this_thread::sleep_for(interval);

                    for (const auto& file : std::filesystem::directory_iterator(directory)) {
                        if (std::filesystem::is_regular_file(file.path())) {
                            std::string file_path = file.path().string();
                            if (FileModified(file_path)) {
                                OnChange(file_path);
                            }
                        }
                    }
                }
                });
        }

        void Stop() {
            running_ = false;
            if (watcher_thread_.joinable()) {
                watcher_thread_.join();
            }
        }
    };





}