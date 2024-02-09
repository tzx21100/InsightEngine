/*!
 * \file FileUtils.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2451
 * \date 24-10-2023
 * \brief
 * This source file defines the implementation for file utility functions using
 * Win32 API.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#ifdef USING_IMGUI

#include "FileUtils.h"
#include "Engine/Core/CoreEngine.h"
#include "Engine/Systems/Window/WindowSystem.h"

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

namespace IS::FileUtils {

    void OpenFileFromDefaultApp(const char* filepath, const char* directory)
    {
        ShellExecute(NULL, NULL, filepath, NULL, directory, SW_SHOW);
    }

    std::string OpenFile(const char* filter, std::string const& relative_path)
    {
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

    std::string SaveFile(const char* filter, std::string const& relative_path)
    {
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

    std::string GetCurrentWorkingDirectory()
    {
        std::filesystem::path path = std::filesystem::current_path();
        std::string path_str = path.generic_string();

        // Replace forward slashes with backslashes
        std::replace(path_str.begin(), path_str.end(), '/', '\\');
        return path_str;
    }

    void OpenScript()
    {
        InsightEngine& engine = InsightEngine::Instance();
        std::string filepath = FileUtils::OpenFile("C# Script File (*.cs)\0*.cs\0", "..\\Insight Engine\\Assets\\Scripts");
        if (!filepath.empty())
            engine.OpenGameScript(filepath);
    }

    std::string OpenAndGetScript()
    {
        return FileUtils::OpenFile("C# Script File (*.cs)\0*.cs\0", "..\\Insight Engine\\Assets\\Scripts");
    }

} // end namespcae IS

#endif // USING_IMGUI
