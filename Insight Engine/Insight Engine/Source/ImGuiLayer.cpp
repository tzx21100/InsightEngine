/*!
 * \file ImGuiLayer.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This source file defines the implementation for class ImGuiLayer, which
 * encapsulates the functionalities of a graphical user interface (GUI)
 * using the Dear ImGui libary.
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
#include "ImGuiLayer.h"

// Dependencies
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>
#include <IconsLucide.h>

namespace IS {

    void ImGuiLayer::OnAttach()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        // Add Fonts
        const float FONT_SIZE = 16.f;

        // Icon font config
        ImGuiStyle& style = ImGui::GetStyle();
        ImFontConfig config;
        config.MergeMode = true;
        config.GlyphOffset.y = style.FramePadding.y;
        static const ImWchar icon_ranges[] = { ICON_MIN_LC, ICON_MAX_LC, 0};

        // Editor fonts
        io.FontDefault =io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto/Roboto-Regular.ttf", FONT_SIZE); // default
        io.Fonts->AddFontFromFileTTF("Assets/Fonts/IconFont/" FONT_ICON_FILE_NAME_LC, FONT_SIZE, &config, icon_ranges); // add icon font
        io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto/Roboto-Bold.ttf", FONT_SIZE); // bold
        io.Fonts->AddFontFromFileTTF("Assets/Fonts/IconFont/" FONT_ICON_FILE_NAME_LC, FONT_SIZE, &config, icon_ranges);; // add icon font
        io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto/Roboto-Italic.ttf", FONT_SIZE); // italic
        io.Fonts->AddFontFromFileTTF("Assets/Fonts/IconFont/" FONT_ICON_FILE_NAME_LC, FONT_SIZE, &config, icon_ranges);; // add icon font

        // Log console fonts
        io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto_Mono/static/RobotoMono-Regular.ttf", FONT_SIZE); // default
        io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto_Mono/static/RobotoMono-Bold.ttf", FONT_SIZE); // bold

        // Setup Dear ImGui style
        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        SetDarkThemeColors();

        // Setup Platform/Renderer backends
        const char* glsl_version = "#version 450";
        ImGui_ImplGlfw_InitForOpenGL(InsightEngine::Instance().GetSystem<WindowSystem>("Window")->GetNativeWindow(), true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        IS_CORE_DEBUG("{} attached.", mDebugName);
    }

    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        IS_CORE_DEBUG("{} detached.", mDebugName);
    }

    void ImGuiLayer::Begin()
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::End()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::SetDarkThemeColors() const
    {
        auto& style = ImGui::GetStyle();
        auto& colors = style.Colors;

        constexpr ImColor DARK_SHARK    = ImColor( 25,  26, 28);
        constexpr ImColor LIGHT_SHARK   = ImColor( 51,  52, 53);
        constexpr ImColor ABBEY         = ImColor( 76,  77, 79);
        constexpr ImColor MINE_SHAFT    = ImColor( 38,  38, 38);
        constexpr ImColor DOVE_GRAY     = ImColor( 97,  97, 97);
        constexpr ImColor TUNDORA       = ImColor( 71,  71, 71);
        constexpr ImColor WEB_ORANGE    = ImColor(255, 165,  0);

        colors[ImGuiCol_WindowBg]           = DARK_SHARK;

        // Headers
        colors[ImGuiCol_Header]             = LIGHT_SHARK;
        colors[ImGuiCol_HeaderHovered]      = ABBEY;
        colors[ImGuiCol_HeaderActive]       = MINE_SHAFT;

        // Buttons
        colors[ImGuiCol_Button]             = LIGHT_SHARK;
        colors[ImGuiCol_ButtonHovered]      = ABBEY;
        colors[ImGuiCol_ButtonActive]       = MINE_SHAFT;

        // Frame BG
        colors[ImGuiCol_FrameBg]            = LIGHT_SHARK;
        colors[ImGuiCol_FrameBgHovered]     = ABBEY;
        colors[ImGuiCol_FrameBgActive]      = MINE_SHAFT;

        // Tabs
        colors[ImGuiCol_Tab]                = MINE_SHAFT;
        colors[ImGuiCol_TabHovered]         = DOVE_GRAY;
        colors[ImGuiCol_TabActive]          = TUNDORA;
        colors[ImGuiCol_TabUnfocused]       = MINE_SHAFT;
        colors[ImGuiCol_TabUnfocusedActive] = LIGHT_SHARK;

        // Title
        colors[ImGuiCol_TitleBg]            = MINE_SHAFT;
        colors[ImGuiCol_TitleBgActive]      = MINE_SHAFT;
        colors[ImGuiCol_TitleBgCollapsed]   = MINE_SHAFT;

        // Scrollbar
        colors[ImGuiCol_SliderGrab]         = WEB_ORANGE;
        colors[ImGuiCol_SliderGrabActive]   = WEB_ORANGE;
        style.GrabMinSize                   = 5.f;
        style.GrabRounding                  = 5.f;

        // Check Mark
        colors[ImGuiCol_CheckMark]          = WEB_ORANGE;

        // Resize Grip
        colors[ImGuiCol_ResizeGripHovered]  = WEB_ORANGE;
        colors[ImGuiCol_ResizeGripActive]   = WEB_ORANGE;

        // Dockspace
        colors[ImGuiCol_DockingPreview]     = WEB_ORANGE;
        style.DockingSeparatorSize          = 1.f;

        // Menubar
        colors[ImGuiCol_MenuBarBg]          = DARK_SHARK;

        // Drop/drop
        colors[ImGuiCol_DragDropTarget]     = WEB_ORANGE;
    }

    bool ImGuiLayer::WantCaptureMouse() const 
    {
        if (InsightEngine::Instance().mUsingGUI)
            return ImGui::GetIO().WantCaptureMouse;

        return true;
    }

    bool ImGuiLayer::WantCaptureKeyboard() const 
    {
        if (InsightEngine::Instance().mUsingGUI)
            return ImGui::GetIO().WantCaptureKeyboard;

        return true;
    }

} // end namespace IS
