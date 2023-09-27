#include "Pch.h"
#include "FileDialog.h"
#include "CoreEngine.h"

#include <filesystem>

// Dependencies
#pragma warning(push)
#pragma warning(disable: 4005) // redefine APIENTRY and IS_ERROR macros
#include <Windows.h>
#pragma warning(pop)
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace IS {

    namespace filedialog {

        std::string OpenFile(const char* filter, std::string const& relative_path) {
            InsightEngine& engine = InsightEngine::Instance();
            auto window = engine.GetSystem<WindowSystem>("Window");
            std::string initial_dir = GetCurrentWorkingDirectory() + "\\" + relative_path;

            OPENFILENAMEA ofn;
            CHAR sz_file[260]{ 0 };
            ZeroMemory(&ofn, sizeof(OPENFILENAME));
            ofn.lpstrInitialDir = initial_dir.c_str();
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = glfwGetWin32Window(window->GetNativeWindow());
            ofn.lpstrFile = sz_file;
            ofn.nMaxFile = sizeof(sz_file);
            ofn.lpstrFilter = filter;
            ofn.nFilterIndex = 1;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
            if (GetOpenFileName(&ofn) == TRUE)
                return ofn.lpstrFile;

            return std::string();
        }

        std::string SaveFile(const char* filter, std::string const& relative_path) {
            InsightEngine& engine = InsightEngine::Instance();
            auto window = engine.GetSystem<WindowSystem>("Window");
            std::string initial_dir = GetCurrentWorkingDirectory() + "\\" + relative_path;

            OPENFILENAMEA ofn;
            CHAR sz_file[260]{ 0 };
            ZeroMemory(&ofn, sizeof(OPENFILENAME));
            ofn.lpstrInitialDir = initial_dir.c_str();
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = glfwGetWin32Window(window->GetNativeWindow());
            ofn.lpstrFile = sz_file;
            ofn.nMaxFile = sizeof(sz_file);
            ofn.lpstrFilter = filter;
            ofn.nFilterIndex = 1;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
            if (GetSaveFileName(&ofn) == TRUE)
                return ofn.lpstrFile;

            return std::string();
        }

        std::string GetCurrentWorkingDirectory() {
            std::filesystem::path path = std::filesystem::current_path();
            std::string path_str = path.generic_string();

            // Replace forward slashes with backslashes
            std::replace(path_str.begin(), path_str.end(), '/', '\\');
            return path_str;
        }

    } // end namespace filedialog

} // end namespace IS
