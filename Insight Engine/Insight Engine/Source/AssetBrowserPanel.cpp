/*!
 * \file AssetBrowserPanel.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 24-10-2023
 * \brief
 * This source file defines the implementation for class AssetBrowserPanel which
 * encapsulates the functionalities of an asset browser panel.
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
#include "AssetBrowserPanel.h"
#include "EditorUtils.h"
#include "FileUtils.h"
#include "Editor.h"

#include <imgui.h>
#include <IconsLucide.h>

namespace IS {

    std::filesystem::path AssetBrowserPanel::ASSETS_PATH = "Assets";

    const std::string AssetBrowserPanel::IMPORTED = "Imported";

    AssetBrowserPanel::AssetBrowserPanel() : mCurrentDirectory(ASSETS_PATH), mSelectedImportedAsset(IMPORTED), mShowImportedAssets(false) {}

    void AssetBrowserPanel::RenderPanel()
    {
        auto const editor_layer = InsightEngine::Instance().GetSystem<Editor>("Editor")->GetEditorLayer();
        auto const FONT_BOLD = ImGui::GetIO().Fonts->Fonts[FONT_TYPE_BOLD];

        // Render asset browser window
        if (ImGui::Begin(ICON_LC_FOLDER_SEARCH_2 "  Browser"))
        {
            RenderControls();
            ImGui::Separator();

            ImGuiTableFlags table_flags = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable;
            if (ImGui::BeginTable("Assets Browser Table", 2, table_flags))
            {
                ImGui::TableNextColumn();
                if (ImGui::BeginChild("Tree Browser", { 0, 0 }, false, ImGuiWindowFlags_HorizontalScrollbar))
                {
                    if (ImGui::BeginTable("Tree Browser Table", 1, ImGuiTableFlags_BordersInnerH))
                    {
                        ImGui::TableNextColumn();
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                        ImGui::PushFont(FONT_BOLD);
                        if (ImGui::Button((ICON_LC_IMPORT "  " + IMPORTED).c_str())) { SwitchImportedAsset(IMPORTED); }
                        ImGui::PopFont();
                        ImGui::PopStyleColor();

                        ImGui::TableNextColumn();
                        RenderImportedAssetsTree();

                        ImGui::Dummy({ 0.f, 20.f });

                        ImGui::TableNextColumn();
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                        ImGui::PushFont(FONT_BOLD);
                        if (ImGui::Button(ICON_LC_FOLDERS "  All Assets")) { SwitchCurrentDirectory(ASSETS_PATH); }
                        ImGui::PopFont();
                        ImGui::PopStyleColor();

                        ImGui::TableNextColumn();
                        RenderDirectoryContents(ASSETS_PATH);

                        ImGui::EndTable(); // end table Tree Browser Table
                    }
                }
                ImGui::EndChild(); // end child window Tree Browser

                ImGui::TableNextColumn();
                mShowImportedAssets ? RenderImportedAssets() : RenderAllAssetsBrowser();

                ImGui::EndTable(); // end table Assets Browser Table
            }
        }

        ImGui::End(); // end window Asset Browser
    }

    void AssetBrowserPanel::RenderControls()
    {
        // Browser Controls
        float label_width = ImGui::CalcTextSize("Thumbnail Size").x + 2 * ImGui::GetStyle().FramePadding.x;
        if (ImGui::BeginTable("Browser Control", 2, 0, {}, label_width))
        {
            ImGuiIO& io = ImGui::GetIO();
            ImFont* const FONT_BOLD = io.Fonts->Fonts[FONT_TYPE_BOLD];
            float panel_width = ImGui::GetContentRegionAvail().x;
            ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;

            ImGui::TableSetupColumn("Browser Control", column_flags, label_width);

            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Thumbnail Size");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(100.f);
            ImGui::SliderFloat("##thumbnail size", &mControls.mThumbnailSize, MIN_THUMBNAIL_SIZE, panel_width, "%.2f", ImGuiSliderFlags_NoInput);

            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Padding");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(100.f);
            ImGui::SliderFloat("##padding", &mControls.mPadding, MIN_PADDING, MAX_PADDING, "%.2f", ImGuiSliderFlags_NoInput);

            ImGui::EndTable();
        }
    }

    void AssetBrowserPanel::RenderPath()
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
            ImGui::SameLine();
            ImGui::TextUnformatted(">");
            ImGui::SameLine();
            if (ImGui::Button(it->filename().string().c_str())) { SwitchCurrentDirectory(*it); }
        }
        ImGui::PopStyleColor();
    }

    void AssetBrowserPanel::RenderAllAssetsBrowser()
    {
        RenderPath();

        auto const editor_layer = InsightEngine::Instance().GetSystem<Editor>("Editor")->GetEditorLayer();

        float cell_size = mControls.mThumbnailSize + mControls.mPadding;
        float panel_width = ImGui::GetContentRegionAvail().x;
        int column_count = std::clamp(static_cast<int>(panel_width / cell_size), 1, static_cast<int>(panel_width));

        ImGuiWindowFlags child_window_flags = ImGuiWindowFlags_AlwaysUseWindowPadding;

        // Back Button
        if (mCurrentDirectory != std::filesystem::path(ASSETS_PATH))
        {
            ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
            if (ImGui::Button(ICON_LC_ARROW_LEFT))
            {
                mCurrentDirectory = mCurrentDirectory.parent_path();
                IS_CORE_TRACE("Returned to Directory: {}", mCurrentDirectory.string());
            }
            ImGui::PopStyleColor();
            ImGui::SameLine();
        }

        ImGui::SetNextWindowBgAlpha(.3f);

        if (ImGui::BeginChild("Assets", { 0, 0 }, false, child_window_flags))
        {
            ImGuiTableFlags table_flags = ImGuiTableFlags_None;
            if (ImGui::BeginTable("Assets Layout", column_count, table_flags))
            {
                for (auto const& entry : std::filesystem::directory_iterator(mCurrentDirectory))
                {
                    auto const& path = entry.path();
                    auto const& relative_path = std::filesystem::relative(path, ASSETS_PATH);
                    auto const& extension = path.extension();
                    std::string filename_string = relative_path.filename().string();
                    const bool is_directory = entry.is_directory();

                    ImGui::TableNextColumn();
                    ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
                    std::string icon_name = is_directory ? "Folder" :
                                extension == ".insight" ? "Insight" :
                                extension == ".json" ? "Json" :
                                extension == ".mp3" || extension == ".MP3" ? "MP3" :
                                extension == ".wav" || extension == ".WAV" ? "WAV" :
                                extension == ".png" ? "PNG" :
                                extension == ".jpeg" ? "JPEG" : "File";

                    ImTextureID icon = editor_layer->GetIcon(icon_name.c_str());
                    bool selected = ImGui::ImageButton(("##" + filename_string).c_str(), icon, { mControls.mThumbnailSize, mControls.mThumbnailSize });
                    ImGui::PopStyleColor();

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

                    if (selected)
                        ImGui::SetKeyboardFocusHere(-1);

                    // Clicking Item
                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        if (is_directory)
                        {
                            mCurrentDirectory /= path.filename();
                            IS_CORE_TRACE("Entered Directory: {}", mCurrentDirectory.string());
                        } else
                        {
                            std::string filepath = mCurrentDirectory.string() + "\\" + path.filename().string();
                            path.extension() == ".insight" ? SceneManager::Instance().LoadScene(filepath) : FileUtils::OpenFileFromDefaultApp(filepath.c_str());
                        }
                    }

                    // Render label
                    ImGui::TextWrapped(filename_string.c_str());
                }

                ImGui::EndTable(); // end table Assets Layout
            }
        }

        ImGui::EndChild(); // end child window Assets
    }

    void AssetBrowserPanel::RenderImportedAssetsTree()
    {
        const std::string textures = "Textures";
        const std::string sounds = "Sounds";
        const std::string prefabs = "Prefabs";

        ImGuiTreeNodeFlags textures_tree_flags = (textures == mSelectedImportedAsset ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
        ImGuiTreeNodeFlags sounds_tree_flags = (sounds == mSelectedImportedAsset ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
        ImGuiTreeNodeFlags prefabs_tree_flags = (prefabs == mSelectedImportedAsset ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;

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
    }

    void AssetBrowserPanel::SwitchImportedAsset(std::string const& asset)
    {
        mSelectedImportedAsset = asset;
        mCurrentDirectory = ASSETS_PATH;
        mShowImportedAssets = true;
    }

    void AssetBrowserPanel::RenderDirectoryContents(std::filesystem::path const& directory)
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

    void AssetBrowserPanel::RenderDirectoryNode(std::filesystem::path const& directory)
    {
        std::string const& filename_string = directory.filename().string();
        const bool is_current = directory == mCurrentDirectory;
        ImGuiTreeNodeFlags tree_flags = is_current ? ImGuiTreeNodeFlags_Selected : 0;
        bool opened = ImGui::TreeNodeEx(((is_current ? ICON_LC_FOLDER_OPEN "  " : ICON_LC_FOLDER "  ") + filename_string).c_str(), tree_flags);

        if (ImGui::IsItemClicked())
        {
            mCurrentDirectory = directory;
            IS_CORE_DEBUG("{}", mCurrentDirectory.string());
        }

        if (opened)
        {
            RenderDirectoryContents(directory);
            ImGui::TreePop();
        }
    }

    void AssetBrowserPanel::RenderDirectoryLeafNode(std::filesystem::path const& directory)
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

    void AssetBrowserPanel::SwitchCurrentDirectory(std::filesystem::path const& directory)
    {
        mCurrentDirectory = directory;
        mSelectedImportedAsset = IMPORTED;
        mShowImportedAssets = false;
    }

    bool AssetBrowserPanel::HasSubDirectory(std::filesystem::path const& directory)
    {
        for (auto const& entry : std::filesystem::directory_iterator(directory))
        {
            if (entry.is_directory())
                return true;
        }
        return false;
    }

    void AssetBrowserPanel::RenderImportedPath()
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        if (ImGui::Button(IMPORTED.c_str())) { SwitchImportedAsset(IMPORTED); }
        ImGui::PopStyleColor();

        if (mSelectedImportedAsset == IMPORTED)
            return;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::SameLine();
        ImGui::TextUnformatted(">");
        ImGui::SameLine();
        if (ImGui::Button(mSelectedImportedAsset.c_str())) { SwitchImportedAsset(mSelectedImportedAsset); }
        ImGui::PopStyleColor();
    }

    void AssetBrowserPanel::RenderImportedAssets()
    {
        auto& engine = InsightEngine::Instance();
        auto const editor = engine.GetSystem<Editor>("Editor");
        auto const asset = engine.GetSystem<AssetManager>("Asset");
        auto const editor_layer = editor->GetEditorLayer();

        float cell_size = mControls.mThumbnailSize + mControls.mPadding;
        float panel_width = ImGui::GetContentRegionAvail().x;
        int column_count = std::clamp(static_cast<int>(panel_width / cell_size), 1, static_cast<int>(panel_width));
        static bool show_alltextures = false;

        RenderImportedPath();

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
            ImGuiTableFlags table_flags = ImGuiTableFlags_None;
            if (ImGui::BeginTable("Imported Assets Layout", column_count, table_flags))
            {
                ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
                if (mSelectedImportedAsset == IMPORTED)
                {
                    ImGui::TableNextColumn();
                    ImGui::ImageButton("##Textures", editor_layer->GetIcon("Folder"), { mControls.mThumbnailSize, mControls.mThumbnailSize });
                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        SwitchImportedAsset("Textures");
                    }
                    ImGui::TextWrapped("Textures");

                    ImGui::TableNextColumn();
                    ImGui::ImageButton("##Sounds", editor_layer->GetIcon("Folder"), { mControls.mThumbnailSize, mControls.mThumbnailSize });
                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        SwitchImportedAsset("Sounds");
                    }
                    ImGui::TextWrapped("Sounds");

                    ImGui::TableNextColumn();
                    ImGui::ImageButton("##Prefabs", editor_layer->GetIcon("Folder"), { mControls.mThumbnailSize, mControls.mThumbnailSize });
                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        SwitchImportedAsset("Prefabs");
                    }
                    ImGui::TextWrapped("Prefabs");
                }

                // Show all imported textures
                if (mSelectedImportedAsset == "Textures")
                {
                    for (auto const& [name, img] : asset->mImageList)
                    {
                        std::filesystem::path path(name);
                        ImTextureID icon = EditorUtils::ConvertTextureID(img.texture_id);
                        float aspect_ratio = static_cast<float>(img.width) / static_cast<float>(img.height);
                        ImGui::TableNextColumn();
                        ImGui::ImageButton(("##" + name).c_str(), icon, { mControls.mThumbnailSize * aspect_ratio, mControls.mThumbnailSize });

                        // Texture Tooltip
                        if (ImGui::BeginItemTooltip())
                        {
                            ImGui::Image(icon, { 48.f * aspect_ratio, 48.f });
                            ImGui::EndTooltip();
                        }

                        // Start file drag
                        if (ImGui::BeginDragDropSource())
                        {
                            const wchar_t* item_path = path.c_str();
                            ImGui::SetDragDropPayload("IMPORTED_TEXTURE", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));

                            // Tooltip
                            const ImVec2 image_size = { 48.f * aspect_ratio, 48.f };
                            ImGui::Image(icon, image_size, { 0, 0 }, { 1, 1 }, { 1, 1, 1, .5f});

                            ImGui::EndDragDropSource();
                        }

                        ImGui::TextWrapped(path.filename().string().c_str());
                    }
                }

                // Show all imported sounds
                else if (mSelectedImportedAsset == "Sounds")
                {
                    for (auto const& [name, sound] : asset->mSoundList)
                    {
                        std::filesystem::path path(name);
                        ImTextureID icon = editor_layer->GetIcon("File");
                        ImGui::TableNextColumn();
                        ImGui::ImageButton(("##" + name).c_str(), icon, { mControls.mThumbnailSize, mControls.mThumbnailSize });

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
                    for (auto const& [name, prefab] : asset->mPrefabList)
                    {
                        std::filesystem::path path(name);
                        ImTextureID icon = editor_layer->GetIcon("File");
                        ImGui::TableNextColumn();
                        ImGui::ImageButton(("##" + name).c_str(), icon, { mControls.mThumbnailSize, mControls.mThumbnailSize });

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

                ImGui::PopStyleColor();

                ImGui::EndTable(); // Imported Assets Layout
            }
        }

        ImGui::EndChild();
    }

} // end namespace IS
