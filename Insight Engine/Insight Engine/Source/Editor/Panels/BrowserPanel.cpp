/*!
 * \file BrowserPanel.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2451
 * \date 24-10-2023
 * \brief
 * This source file defines the implementation for class BrowserPanel which
 * encapsulates the functionalities of an asset browser panel.
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

#include "BrowserPanel.h"
#include "Engine/Systems/Input/Input.h"
#include "Editor/Utils/EditorUtils.h"

#include <imgui.h>

namespace IS {

    std::filesystem::path BrowserPanel::ASSETS_PATH = "Assets";

    const std::string BrowserPanel::IMPORTED = "Imported";

    void BrowserPanel::UpdatePanel()
    {
        auto& engine = InsightEngine::Instance();
        auto const input = engine.GetSystem<InputManager>("Input");
        const bool CTRL_HELD = input->IsKeyHeld(GLFW_KEY_LEFT_CONTROL) || input->IsKeyHeld(GLFW_KEY_RIGHT_CONTROL);
        ImGuiIO& io = ImGui::GetIO();

        if (CTRL_HELD && mChildWindowHovered)
        {
            mControls.mThumbnailSize = std::max(mControls.mThumbnailSize + io.MouseWheel, MIN_THUMBNAIL_SIZE);
        }
    }

    void BrowserPanel::RenderPanel()
    {
        // Render asset browser window
        ImGui::Begin((ICON_LC_FOLDER_SEARCH_2 "  " + mName).c_str());
        
        // Window Contents
        {
            ImGuiTableFlags table_flags = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable;
            EditorUtils::RenderTableFixedWidth("Assets Browser Table", 2, [&]()
            {
                ImGui::TableNextColumn();
                if (ImGui::BeginChild("Tree Browser", { 0, 0 }, false, ImGuiWindowFlags_HorizontalScrollbar))
                {
                    EditorUtils::RenderTable("Tree Browser Table", 1, [&]()
                    {
                        ImGui::TableNextColumn();
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                        if (ImGui::Button((ICON_LC_IMPORT "  " + IMPORTED).c_str()))
                        {
                            SwitchImportedAsset(IMPORTED);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button(ICON_LC_ROTATE_CW))
                        {
                            auto& engine = InsightEngine::Instance();
                            auto const asset = engine.GetSystem<AssetManager>("Asset");
                            asset->RefreshDirectiories();
                        }
                        ImGui::SetItemTooltip("Reload Imported Assets");
                        ImGui::PopStyleColor();

                        ImGui::TableNextColumn();
                        RenderImportedAssetsTree();

                        ImGui::Spacing();

                        ImGui::TableNextColumn();
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                        if (ImGui::Button(ICON_LC_FOLDERS "  All Assets"))
                        {
                            SwitchCurrentDirectory(ASSETS_PATH);
                        }
                        ImGui::PopStyleColor();

                        ImGui::TableNextColumn();
                        RenderDirectoryContents(ASSETS_PATH);

                    }, ImGuiTableFlags_BordersInnerH);
                }
                ImGui::EndChild(); // end child window Tree Browser

                ImGui::TableNextColumn();

                // Texture filter
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
                EditorUtils::RenderFilterWithHint(mFilter, "Filter Asset...");
                ImGui::PopStyleVar();

                mShowImportedAssets ? RenderImportedAssets() : RenderAllAssetsBrowser();
            }, table_flags);
        }

        // Save window states
        mFocused    = ImGui::IsWindowFocused();
        mAppearing  = ImGui::IsWindowAppearing();
        mHovered    = ImGui::IsWindowHovered();
        ImGui::End(); // end window Asset Browser
    }

    void BrowserPanel::RenderPath()
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        if (ImGui::Button(ASSETS_PATH.filename().string().c_str())) { SwitchCurrentDirectory(ASSETS_PATH); }
        ImGui::PopStyleColor();

        if (mCurrentDirectory == ASSETS_PATH)
            return;

        std::vector<std::filesystem::path> path_components;

        // Traverse the path from current diretory back to assets directory
        for (std::filesystem::path path = mCurrentDirectory; path != ASSETS_PATH; path = path.parent_path())
        {
            path_components.push_back(path);
        }

        // Render buttons for each directory component in reverse order
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        for (auto it = path_components.rbegin(); it != path_components.rend(); ++it)
        {
            ImGui::SameLine(0.f, 0.f);
            ImGui::TextUnformatted(ICON_LC_CHEVRON_RIGHT);
            ImGui::SameLine(0.f, 0.f);
            if (ImGui::Button(it->filename().string().c_str())) { SwitchCurrentDirectory(*it); }
        }
        ImGui::PopStyleColor();
    }

    void BrowserPanel::RenderAllAssetsBrowser()
    {
        RenderPath();
        RenderControls();

        float cell_size = mControls.mThumbnailSize + mControls.mPadding;
        float panel_width = ImGui::GetContentRegionAvail().x;
        int column_count = std::clamp(static_cast<int>(panel_width / cell_size), 1, static_cast<int>(panel_width));

        ImGuiWindowFlags child_window_flags = ImGuiWindowFlags_AlwaysUseWindowPadding;

        // Back Button
        if (mCurrentDirectory != std::filesystem::path(ASSETS_PATH))
        {
            ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
            if (ImGui::Button(ICON_LC_ARROW_LEFT)) { mCurrentDirectory = mCurrentDirectory.parent_path(); }
            ImGui::PopStyleColor();
            ImGui::SameLine();
        }

        ImGui::SetNextWindowBgAlpha(.3f);

        if (ImGui::BeginChild("Assets", { 0, 0 }, false, child_window_flags))
        {
            EditorUtils::RenderTable("Assets Layout", column_count, [&]()
            {
                for (auto const& entry : std::filesystem::directory_iterator(mCurrentDirectory))
                {
                    static std::string selected_file;
                    auto const& path = entry.path();
                    auto const& relative_path = std::filesystem::relative(path, ASSETS_PATH);
                    auto const& extension = path.extension();
                    std::string filename_string = relative_path.filename().string();

                    if (!mFilter.PassFilter(filename_string.c_str()))
                    {
                        continue;
                    }

                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered())
                    {
                        selected_file = std::string();
                    }

                    const bool is_directory = entry.is_directory();
                    ImGui::TableNextColumn();
                    std::string icon_name = is_directory ? "Folder" :
                        extension == ".insight" ? "Insight" :
                        extension == ".json" ? "Json" :
                        (extension == ".mp3" || extension == ".MP3") ? "MP3" :
                        (extension == ".wav" || extension == ".WAV") ? "WAV" :
                        extension == ".png" ? "PNG" :
                        (extension == ".jpeg" || extension == "jpg") ? "JPEG" : "File";

                    ImTextureID icon = mEditorLayer.GetIcon(icon_name.c_str());
                    RenderAssetItem(filename_string, selected_file, icon);

                    // Start file drag
                    if (ImGui::BeginDragDropSource())
                    {
                        const wchar_t* item_path = path.c_str();
                        ImGui::SetDragDropPayload("ASSET_BROWSER_ITEM", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));

                        // Tooltip
                        const ImVec2 image_size = { 48.f, 48.f };
                        ImGui::Image(icon, image_size);

                        ImGui::EndDragDropSource();
                    }

                    // Double-click
                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        if (!is_directory)
                        {
                            std::string filepath = mCurrentDirectory.string() + "\\" + path.filename().string();
                            path.extension() == ".insight" ? SceneManager::Instance().LoadScene(filepath) : FileUtils::OpenFileFromDefaultApp(filepath.c_str());
                        }
                        else { mCurrentDirectory /= path.filename(); }
                    }

                    if (!is_directory)
                    {
                        // Right-click
                        if (ImGui::BeginPopupContextItem())
                        {
                            // Construct filepath
                            std::string filepath = mCurrentDirectory.string() + "\\" + path.filename().string();

                            // Open file
                            if (ImGui::MenuItem(ICON_LC_EXTERNAL_LINK "  Open File"))
                            {
                                FileUtils::OpenFileFromDefaultApp(filepath.c_str());
                            }
                            ImGui::SetItemTooltip("Opens file in its default application.");

                            // Delete file
                            if (ImGui::MenuItem(ICON_LC_TRASH_2 "  Delete File"))
                            {
                                FileUtils::FileDelete(filepath);
                            }
                            ImGui::SetItemTooltip("Deletes file from local hard disk.");

                            ImGui::EndPopup();
                        }
                    }

                    // Render label
                    ImGui::TextWrapped(filename_string.c_str());
                }
            });
            mChildWindowHovered = ImGui::IsWindowHovered();
        }

        ImGui::EndChild(); // end child window Assets
    }

    void BrowserPanel::RenderImportedAssetsTree()
    {
        const std::string textures = "Textures";
        const std::string sounds   = "Sounds";
        const std::string prefabs  = "Prefabs";
        const std::string scripts  = "Scripts";
        const std::string shaders  = "Shaders";

        ImGuiTreeNodeFlags textures_tree_flags = (textures == mSelectedImportedAsset ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
        ImGuiTreeNodeFlags sounds_tree_flags   = (sounds   == mSelectedImportedAsset ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
        ImGuiTreeNodeFlags prefabs_tree_flags  = (prefabs  == mSelectedImportedAsset ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
        ImGuiTreeNodeFlags scripts_tree_flags  = (scripts  == mSelectedImportedAsset ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
        ImGuiTreeNodeFlags shaders_tree_flags  = (shaders  == mSelectedImportedAsset ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;

        if (ImGui::TreeNodeEx((ICON_LC_IMAGE "  " + textures).c_str(), textures_tree_flags))
        {
            if (ImGui::IsItemClicked())
                SwitchImportedAsset(textures);
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx((ICON_LC_MUSIC "  " + sounds).c_str(), sounds_tree_flags))
        {
            if (ImGui::IsItemClicked())
                SwitchImportedAsset(sounds);
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx((ICON_LC_PACKAGE "  " + prefabs).c_str(), prefabs_tree_flags))
        {
            if (ImGui::IsItemClicked())
                SwitchImportedAsset(prefabs);
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx((ICON_LC_CODE "  " + scripts).c_str(), scripts_tree_flags))
        {
            if (ImGui::IsItemClicked())
                SwitchImportedAsset(scripts);
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx((ICON_LC_IMAGE_PLUS "  " + shaders).c_str(), shaders_tree_flags))
        {
            if (ImGui::IsItemClicked())
                SwitchImportedAsset(shaders);
            ImGui::TreePop();
        }
    }

    void BrowserPanel::SwitchImportedAsset(std::string const& asset)
    {
        mSelectedImportedAsset = asset;
        mCurrentDirectory = ASSETS_PATH;
        mShowImportedAssets = true;
    }

    void BrowserPanel::RenderDirectoryContents(std::filesystem::path const& directory)
    {
        for (auto const& entry : std::filesystem::directory_iterator(directory))
        {
            std::filesystem::path const& path = entry.path();
            bool is_directory = entry.is_directory();

            if (is_directory)
            {
                HasSubDirectory(path) ? RenderDirectoryNode(path) : RenderDirectoryLeafNode(path);
            }
        }
    }

    void BrowserPanel::RenderDirectoryNode(std::filesystem::path const& directory)
    {
        std::string const& filename_string = directory.filename().string();
        const bool is_current = directory == mCurrentDirectory;
        ImGuiTreeNodeFlags tree_flags = is_current ? ImGuiTreeNodeFlags_Selected : 0;
        bool opened = ImGui::TreeNodeEx(((is_current ? ICON_LC_FOLDER_OPEN "  " : ICON_LC_FOLDER "  ") + filename_string).c_str(), tree_flags);

        if (ImGui::IsItemClicked()) { mCurrentDirectory = directory; }

        if (opened)
        {
            RenderDirectoryContents(directory);
            ImGui::TreePop();
        }
    }

    void BrowserPanel::RenderDirectoryLeafNode(std::filesystem::path const& directory)
    {
        std::string const& filename_string = directory.filename().string();
        const bool is_current = directory == mCurrentDirectory;
        ImGuiTreeNodeFlags tree_flags = (is_current ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
        bool opened = ImGui::TreeNodeEx(((is_current ? ICON_LC_FOLDER_OPEN "  " : ICON_LC_FOLDER "  ") + filename_string).c_str(), tree_flags);
        
        if (ImGui::IsItemClicked())
            SwitchCurrentDirectory(directory);
        
        if (opened)
        {
            ImGui::TreePop();
        }
    }

    void BrowserPanel::SwitchCurrentDirectory(std::filesystem::path const& directory)
    {
        mCurrentDirectory = directory;
        mSelectedImportedAsset = IMPORTED;
        mShowImportedAssets = false;
    }

    bool BrowserPanel::HasSubDirectory(std::filesystem::path const& directory)
    {
        for (auto const& entry : std::filesystem::directory_iterator(directory))
        {
            if (entry.is_directory())
                return true;
        }
        return false;
    }

    void BrowserPanel::RenderImportedPath()
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        if (ImGui::Button(IMPORTED.c_str())) { SwitchImportedAsset(IMPORTED); }
        ImGui::PopStyleColor();

        if (mSelectedImportedAsset == IMPORTED)
            return;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::SameLine(0.f, 0.f);
        ImGui::TextUnformatted(ICON_LC_CHEVRON_RIGHT);
        ImGui::SameLine(0.f, 0.f);
        if (ImGui::Button(mSelectedImportedAsset.c_str())) { SwitchImportedAsset(mSelectedImportedAsset); }
        ImGui::PopStyleColor();
    }

    void BrowserPanel::RenderImportedAssets()
    {
        auto& engine = InsightEngine::Instance();
        auto const asset = engine.GetSystem<AssetManager>("Asset");

        float cell_size = mControls.mThumbnailSize + mControls.mPadding;
        float panel_width = ImGui::GetContentRegionAvail().x;
        int column_count = std::clamp(static_cast<int>(panel_width / cell_size), 1, static_cast<int>(panel_width));
        static bool show_alltextures = false;

        RenderImportedPath();
        RenderControls();

        if (mSelectedImportedAsset != IMPORTED)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
            if (ImGui::Button(ICON_LC_ARROW_LEFT)) { SwitchImportedAsset(IMPORTED); }
            ImGui::PopStyleColor();
            ImGui::SameLine();
        }

        ImGuiWindowFlags child_window_flags = ImGuiWindowFlags_AlwaysUseWindowPadding;
        ImGui::SetNextWindowBgAlpha(.3f);
        if (ImGui::BeginChild("Imported Assets Browser", { 0.f, 0.f }, false, child_window_flags))
        {
            EditorUtils::RenderTable("Imported Assets Layout", column_count, [&]()
            {
                static std::string selected_file;

                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered())
                {
                    selected_file = std::string();
                }

                if (mSelectedImportedAsset == IMPORTED)
                    RenderImportedAssetFolder({ "Textures", "Sounds", "Prefabs", "Scripts", "Shaders" }, selected_file);
                                
                // Show all imported textures
                if (mSelectedImportedAsset == "Textures")
                {
                    for (auto it = asset->mImageList.begin(); it != asset->mImageList.end(); ++it)
                    {
                        auto const& [name, img] = *it;

                        if (!mFilter.PassFilter(name.c_str()))
                        {
                            continue;
                        }

                        std::filesystem::path path(name);
                        ImTextureID icon = EditorUtils::ConvertTextureID(img.texture_id);
                        float aspect_ratio = static_cast<float>(img.width) / static_cast<float>(img.height);
                        ImGui::TableNextColumn();

                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
                        RenderAssetItem(name, selected_file, icon, aspect_ratio);
                        ImGui::PopStyleVar();

                        // Texture Tooltip
                        if (ImGui::BeginItemTooltip())
                        {
                            ImGui::Image(icon, { 48.f * aspect_ratio, 48.f });
                            ImGui::EndTooltip();
                        }

                        // Double-click
                        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                        {
                            FileUtils::OpenFileFromDefaultApp(path.string().c_str(), AssetManager::TEXTURE_DIRECTORY);
                        }

                        // Right-click
                        RenderContextMenuImported(asset->mImageList, it, path.string(), AssetManager::TEXTURE_DIRECTORY);

                        // Start texture drag
                        if (ImGui::BeginDragDropSource())
                        {
                            const wchar_t* item_path = path.c_str();
                            ImGui::SetDragDropPayload("IMPORTED_TEXTURE", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));

                            // Tooltip
                            const ImVec2 image_size = { 48.f * aspect_ratio, 48.f };
                            ImGui::Image(icon, image_size, { 0, 0 }, { 1, 1 }, { 1, 1, 1, .5f });

                            ImGui::EndDragDropSource();
                        }

                        ImGui::TextWrapped(path.filename().string().c_str());
                    }
                }

                // Show all imported sounds
                else if (mSelectedImportedAsset == "Sounds")
                {
                    for (auto it = asset->mSoundList.begin(); it != asset->mSoundList.end(); ++it)
                    {
                        auto const& [name, sound] = *it;

                        if (!mFilter.PassFilter(name.c_str()))
                        {
                            continue;
                        }
                        std::filesystem::path path(name);
                        std::string extension = path.extension().string();
                        bool is_mp3 = extension == "MP3" || extension == "mp3";
                        ImTextureID icon = mEditorLayer.GetIcon(is_mp3 ? "MP3" : "WAV");
                        ImGui::TableNextColumn();

                        // Render image
                        RenderAssetItem(name, selected_file, icon);

                        // Double-click
                        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                        {
                            FileUtils::OpenFileFromDefaultApp(path.string().c_str(), AssetManager::SOUND_DIRECTORY);
                        }

                        // Right-click
                        RenderContextMenuImported(asset->mSoundList, it, path.string(), AssetManager::SOUND_DIRECTORY);

                        // Start file drag
                        if (ImGui::BeginDragDropSource())
                        {
                            const wchar_t* item_path = path.c_str();
                            ImGui::SetDragDropPayload("IMPORTED_SOUND", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));

                            // Tooltip
                            const ImVec2 image_size = { 48.f , 48.f };
                            ImGui::Image(icon, image_size, { 0, 0 }, { 1, 1 }, { 1, 1, 1, .5f });

                            ImGui::EndDragDropSource();
                        }

                        ImGui::TextWrapped(path.filename().string().c_str());
                    }
                }

                // Show all imported prefabs
                else if (mSelectedImportedAsset == "Prefabs")
                {
                    for (auto it = asset->mPrefabList.begin(); it != asset->mPrefabList.end(); ++it)
                    {
                        auto const& [name, prefab] = *it;

                        if (!mFilter.PassFilter(name.c_str()))
                        {
                            continue;
                        }

                        std::filesystem::path path(name);
                        ImTextureID icon = mEditorLayer.GetIcon("Json");
                        ImGui::TableNextColumn();

                        // Render image
                        RenderAssetItem(name, selected_file, icon);

                        // Double-click
                        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                        {
                            std::string filepath = path.string() + ".json";
                            FileUtils::OpenFileFromDefaultApp(filepath.c_str(), AssetManager::PREFAB_DIRECTORY);
                        }

                        // Right-click
                        RenderContextMenuImported(asset->mPrefabList, it, path.string(), AssetManager::PREFAB_DIRECTORY);

                        // Start file drag
                        if (ImGui::BeginDragDropSource())
                        {
                            const wchar_t* item_path = path.c_str();
                            ImGui::SetDragDropPayload("IMPORTED_PREFAB", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));

                            // Tooltip
                            const ImVec2 image_size = { 48.f , 48.f };
                            ImGui::Image(icon, image_size, { 0, 0 }, { 1, 1 }, { 1, 1, 1, .5f });

                            ImGui::EndDragDropSource();
                        }

                        ImGui::TextWrapped(path.filename().string().c_str());
                    }
                }

                // Show all imported scripts
                else if (mSelectedImportedAsset == "Scripts")
                {
                    for (auto it{ asset->mScriptList.begin() }; it != asset->mScriptList.end(); ++it)
                    {
                        std::string const& name = *it;

                        if (!mFilter.PassFilter(name.c_str()))
                        {
                            continue;
                        }

                        std::filesystem::path path(name);
                        ImTextureID icon = mEditorLayer.GetIcon("C#");
                        ImGui::TableNextColumn();

                        // Render image
                        RenderAssetItem(name, selected_file, icon);

                        // Double-click
                        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                        {
                            FileUtils::OpenFileFromDefaultApp(path.string().c_str(), AssetManager::SCRIPT_DIRECTORY);
                        }

                        // Right-click
                        RenderContextMenuImported(asset->mScriptList, it, path.string(), AssetManager::SCRIPT_DIRECTORY);

                        // Start file drag
                        if (ImGui::BeginDragDropSource())
                        {
                            const wchar_t* item_path = path.c_str();
                            ImGui::SetDragDropPayload("IMPORTED_SCRIPT", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));

                            // Tooltip
                            const ImVec2 image_size = { 48.f , 48.f };
                            ImGui::Image(icon, image_size, { 0, 0 }, { 1, 1 }, { 1, 1, 1, .5f });

                            ImGui::EndDragDropSource();
                        }

                        ImGui::TextWrapped(path.filename().string().c_str());
                    }
                }

                // Show all imported shaders
                else if (mSelectedImportedAsset == "Shaders")
                {
                    for (auto it{ asset->mShaderList.begin() }; it != asset->mShaderList.end(); ++it)
                    {
                        std::string const& name = *it;

                        if (!mFilter.PassFilter(name.c_str()))
                        {
                            continue;
                        }

                        std::filesystem::path path(name);
                        ImTextureID icon = mEditorLayer.GetIcon("File");
                        ImGui::TableNextColumn();

                        // Render image
                        RenderAssetItem(name, selected_file, icon);

                        // Double-click
                        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                        {
                            FileUtils::OpenFileFromDefaultApp(path.string().c_str(), AssetManager::SHADER_DIRECTORY);
                        }

                        // Right-click
                        RenderContextMenuImported(asset->mShaderList, it, path.string(), AssetManager::SHADER_DIRECTORY);

                        // Start file drag
                        if (ImGui::BeginDragDropSource())
                        {
                            const wchar_t* item_path = path.c_str();
                            ImGui::SetDragDropPayload("IMPORTED_SHADER", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));

                            // Tooltip
                            const ImVec2 image_size = { 48.f , 48.f };
                            ImGui::Image(icon, image_size, { 0, 0 }, { 1, 1 }, { 1, 1, 1, .5f });

                            ImGui::EndDragDropSource();
                        }

                        ImGui::TextWrapped(path.filename().string().c_str());
                    }
                }

            });
            mChildWindowHovered = ImGui::IsWindowHovered();
        }

        ImGui::EndChild(); // end child window Imported Assets Browser
    }

    void BrowserPanel::RenderControls()
    {
        auto& style = ImGui::GetStyle();
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - style.FramePadding.x - style.ItemSpacing.x);

        ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
        if (ImGui::Button(ICON_LC_MORE_VERTICAL))
            ImGui::OpenPopup("BrowserSettings");
        ImGui::PopStyleColor();

        if (ImGui::BeginPopup("BrowserSettings"))
        {
            // Browser Controls
            float label_width = ImGui::CalcTextSize("Thumbnail Size").x + 2 * ImGui::GetStyle().FramePadding.x;
            EditorUtils::RenderTableFixedWidth("Browser Control", 2, [&]()
            {
                float panel_width = ImGui::GetContentRegionAvail().x;
                ImGui::TableNextColumn();
                ImGui::TextUnformatted("Thumbnail Size");
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(100.f);
                ImGui::SliderFloat("##thumbnail size", &mControls.mThumbnailSize, MIN_THUMBNAIL_SIZE, panel_width, "%.2f", ImGuiSliderFlags_NoInput);

                ImGui::TableNextColumn();
                ImGui::TextUnformatted("Padding");
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(100.f);
                ImGui::SliderFloat("##padding", &mControls.mPadding, MIN_PADDING, MAX_PADDING, "%.2f", ImGuiSliderFlags_NoInput);

            }, 0, label_width);

            ImGui::EndPopup(); // end popup BrowserSettings
        }
    }

    void BrowserPanel::RenderImportedAssetFolder(std::initializer_list<std::string> imported_assets, std::string& selected_filename)
    {
        for (std::string const& asset : imported_assets)
        {
            ImGui::TableNextColumn();

            if (selected_filename != asset)
                ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });

            bool selected = ImGui::ImageButton(("##" + asset).c_str(), mEditorLayer.GetIcon("Folder"), { mControls.mThumbnailSize, mControls.mThumbnailSize });

            if (selected_filename != asset)
                ImGui::PopStyleColor();

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                SwitchImportedAsset(asset.c_str());
            }
            ImGui::TextWrapped(asset.c_str());

            if (selected)
                selected_filename = asset;
        }
    }

    void BrowserPanel::RenderAssetItem(std::string const& item, std::string& selected_filename, ImTextureID& icon, float aspect_ratio)
    {
        if (selected_filename != item)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
        }
        bool selected = ImGui::ImageButton(("##" + item).c_str(), icon, { mControls.mThumbnailSize * aspect_ratio, mControls.mThumbnailSize });
        if (selected_filename != item)
        {
            ImGui::PopStyleColor();
        }
        if (selected)
        {
            selected_filename = item;
        }
    }

} // end namespace IS

#endif // USING_IMGUI
