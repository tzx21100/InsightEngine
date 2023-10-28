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

#include <regex>

namespace IS {

    std::filesystem::path AssetBrowserPanel::ASSETS_PATH = "Assets";

    AssetBrowserPanel::AssetBrowserPanel() : mCurrentDirectory(ASSETS_PATH) {}

    void AssetBrowserPanel::RenderPanel()
    {
        auto const editor_layer = InsightEngine::Instance().GetSystem<Editor>("Editor")->GetEditorLayer();

        // Render asset browser window
        if (ImGui::Begin("Asset Browser"))
        {
            RenderControls();
            if (ImGui::BeginTable("TwoAssetBrowser", 2))
            {
                ImGui::TableNextColumn();
                RenderImportedAssets();

                ImGui::TableNextColumn();
                RenderPath();
                RenderAllAssetsBrowser();

                ImGui::EndTable(); // end table TwoAssetBrowser
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
            ImGui::SliderFloat("##thumbnail size", &mControls.mThumbnailSize, MIN_THUMBNAIL_SIZE, panel_width, "%.2f");

            ImGui::TableNextColumn();
            ImGui::PushFont(FONT_BOLD);
            ImGui::TextUnformatted("Padding");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(100.f);
            ImGui::SliderFloat("##padding", &mControls.mPadding, MIN_PADDING, MAX_PADDING, "%.2f");

            ImGui::EndTable();
        }
    }

    void AssetBrowserPanel::RenderPath()
    {
        // Replace backslashes with " >"
        std::string path_records = mCurrentDirectory.string();
        std::replace(path_records.begin(), path_records.end(), '\\', ' ');
        path_records = std::regex_replace(path_records, std::regex(" "), " > ");

        ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
        ImVec2 path_records_textsize = ImGui::CalcTextSize(path_records.c_str());
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        // Calculate the background rectangle's position and size
        ImGuiStyle& style = ImGui::GetStyle();
        float padding_width = style.FramePadding.x;
        ImVec2 background_pos = { cursor_pos.x - padding_width / 2.f, cursor_pos.y };
        ImVec2 background_size = ImVec2(path_records_textsize.x + padding_width, path_records_textsize.y);

        // Draw the background rectangle with the specified color
        draw_list->AddRectFilled(background_pos,
                                 ImVec2(background_pos.x + background_size.x, background_pos.y + background_size.y),
                                 IM_COL32(255, 255, 255, 80), 2.f);
        ImGui::Text("%s >", path_records.c_str());
    }

    void AssetBrowserPanel::RenderAllAssetsBrowser()
    {
        auto const editor_layer = InsightEngine::Instance().GetSystem<Editor>("Editor")->GetEditorLayer();

        float cell_size = mControls.mThumbnailSize + mControls.mPadding;
        float panel_width = ImGui::GetContentRegionAvail().x;
        int column_count = std::clamp(static_cast<int>(panel_width / cell_size), 1, static_cast<int>(panel_width));

        ImGuiWindowFlags child_window_flags = ImGuiWindowFlags_AlwaysUseWindowPadding;

        // Back Button
        if (mCurrentDirectory != std::filesystem::path(ASSETS_PATH))
        {
            bool back_pressed = ImGui::ImageButton("AssetsBack", editor_layer->GetIcon("BackButton"), { 16.f, 16.f });

            if (back_pressed)
            {
                mCurrentDirectory = mCurrentDirectory.parent_path();
                IS_CORE_TRACE("Returned to Directory: {}", mCurrentDirectory.string());
            }
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
                    std::string filename_string = relative_path.filename().string();
                    const bool is_directory = entry.is_directory();

                    ImGui::TableNextColumn();
                    ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
                    ImTextureID icon = editor_layer->GetIcon(is_directory ? "Folder" : "File");
                    ImGui::ImageButton(("##" + filename_string).c_str(), icon, { mControls.mThumbnailSize, mControls.mThumbnailSize });
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

    void AssetBrowserPanel::RenderImportedAssets()
    {
        auto& engine = InsightEngine::Instance();
        auto const editor = engine.GetSystem<Editor>("Editor");
        auto const asset = engine.GetSystem<AssetManager>("Asset");
        auto const editor_layer = editor->GetEditorLayer();
        auto const font_bold = ImGui::GetIO().Fonts->Fonts[FONT_TYPE_BOLD];

        float cell_size = mControls.mThumbnailSize + mControls.mPadding;
        float panel_width = ImGui::GetContentRegionAvail().x;
        int column_count = std::clamp(static_cast<int>(panel_width / cell_size), 1, static_cast<int>(panel_width));
        static bool show_alltextures = false;

        ImGuiWindowFlags child_window_flags = ImGuiWindowFlags_AlwaysUseWindowPadding;

        ImGui::SetNextWindowBgAlpha(.3f);

        ImGui::PushFont(font_bold);
        ImGui::TextUnformatted("Imported");
        ImGui::PopFont();

        if (show_alltextures)
        {
            if (ImGui::ImageButton("TexturesBack", editor_layer->GetIcon("BackButton"), { 16.f, 16.f }))
            {
                show_alltextures = false;
            }
            ImGui::SameLine();
        }

        if (ImGui::BeginChild("Imported Assets Browser", { 0.f, 0.f }, false, child_window_flags))
        {
            ImGuiTableFlags table_flags = ImGuiTableFlags_None;
            if (ImGui::BeginTable("Imported Assets Layout", column_count, table_flags))
            {
                ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
                if (!show_alltextures)
                {
                    ImGui::TableNextColumn();
                    ImGui::ImageButton("##Textures", editor_layer->GetIcon("Folder"), { mControls.mThumbnailSize, mControls.mThumbnailSize });
                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        show_alltextures = true;
                    }
                    ImGui::TextWrapped("Textures");
                }

                if (show_alltextures)
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
                            ImGui::SetDragDropPayload("ASSET_BROWSER_ITEM", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));

                            // Tooltip
                            const ImVec2 image_size = { 48.f * aspect_ratio, 48.f };
                            ImGui::Image(icon, image_size);

                            ImGui::EndDragDropSource();
                        }

                        ImGui::TextWrapped(path.stem().string().c_str());
                    }
                }

                ImGui::PopStyleColor();

                ImGui::EndTable(); // Imported Assets Layout
            }
        }

        ImGui::EndChild();
    }

} // end namespace IS
