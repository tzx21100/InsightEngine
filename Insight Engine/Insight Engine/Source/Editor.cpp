/*!
 * \file Editor.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This source file defines the implementation for class Editor, which
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
#include "Editor.h"

// Dependencies
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>
#include <IconsLucide.h>

namespace IS {

    Editor::Editor() {}

    Editor::~Editor() { Terminate(); }

    void Editor::Initialize()
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

        mEditorLayer = std::make_shared<EditorLayer>();
        PushLayer(mEditorLayer);
    }

    void Editor::Update(float delta_time)
    {
        Begin();
        mLayers.Update(delta_time);
        mLayers.Render();
        End();
    }

    void Editor::Terminate()
    {
        mLayers.ClearStack();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Editor::HandleMessage(Message const&) {}

    std::string Editor::GetName() { return "Editor"; }

    void Editor::Begin()
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void Editor::End()
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

    void Editor::PushLayer(layer_type layer)
    {
        mLayers.PushLayer(layer);
        layer->OnAttach();
    }

    void Editor::PushOverlay(layer_type overlay)
    {
        mLayers.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Editor::PopLayer(layer_type layer)
    {
        mLayers.PopLayer(layer);
        layer->OnDetach();
    }

    void Editor::PopOverlay(layer_type overlay)
    {
        mLayers.PopOverlay(overlay);
        overlay->OnDetach();
    }

    void Editor::SetDarkThemeColors() const
    {
        auto& style = ImGui::GetStyle();
        auto& colors = style.Colors;

        // Colors
        //------------------------------------------------------------------------
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Scrollbar
        colors[ImGuiCol_SliderGrab] = ImVec4{ 1.f, 0.647f, 0.f, 1.f };
        colors[ImGuiCol_SliderGrabActive] = ImVec4{ 1.f, 0.647f, 0.f, 1.f };

        // Check Mark
        colors[ImGuiCol_CheckMark] = ImVec4{ 1.f, 0.647f, 0.f, 1.f };

        // Resize Grip
        colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 1.f, 0.647f, 0.f, 1.f };
        colors[ImGuiCol_ResizeGripActive] = ImVec4{ 1.f, 0.647f, 0.f, 1.f };
        style.GrabMinSize = 5.f;
        style.GrabRounding = 5.f;

        // Dockspace
        colors[ImGuiCol_DockingPreview] = ImVec4{ 1.f, 0.647f, 0.f, 1.f };

        // Menubar
        colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    }

    bool Editor::WantCaptureMouse() const { return ImGui::GetIO().WantCaptureMouse; }

    bool Editor::WantCaptureKeyboard() const { return ImGui::GetIO().WantCaptureKeyboard; }

    std::shared_ptr<EditorLayer> Editor::GetEditorLayer() { return mEditorLayer; }

} // end namespace IS
