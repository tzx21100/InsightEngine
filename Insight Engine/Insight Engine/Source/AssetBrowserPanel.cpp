#include "Pch.h"
#include "AssetBrowserPanel.h"
#include "EditorUtils.h"

#include <regex>

#pragma warning(push)
#pragma warning(disable: 4005) // redefine APIENTRY && IS_WARN
#include <Windows.h> // for ShellExecute()
#pragma warning(pop)

namespace IS {

    static std::filesystem::path ASSETS_PATH = "Assets";

    AssetBrowserPanel::AssetBrowserPanel() : mCurrentDirectory(ASSETS_PATH) {
        InsightEngine& engine = InsightEngine::Instance();
        auto asset  = engine.GetSystem<AssetManager>("Asset");
        mFolderIcon = std::make_unique<Image>(asset->GetImage("Assets/Icons/folder_icon.png"));
        mFileIcon   = std::make_unique<Image>(asset->GetImage("Assets/Icons/file_icon.png"));
        mBackIcon   = std::make_unique<Image>(asset->GetImage("Assets/Icons/back_icon.png"));
    }

    void AssetBrowserPanel::RenderPanel() {
        ImGui::Begin("Asset Browser");

        RenderControls();

        // Draw the text on top of the background
        ImGui::BeginGroup();

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
        ImGui::EndGroup();

        float cell_size = mControls.mThumbnailSize + mControls.mPadding;
        float panel_width = ImGui::GetContentRegionAvail().x;
        int column_count = std::clamp(static_cast<int>(panel_width / cell_size), 1, static_cast<int>(panel_width));

        // Back Button
        if (mCurrentDirectory != std::filesystem::path(ASSETS_PATH)) {
            bool back_pressed = ImGui::ImageButton(EditorUtils::ConvertTextureID(mBackIcon->texture_id), { 16.f, 16.f });

            if (back_pressed) {
                mCurrentDirectory = mCurrentDirectory.parent_path();
                IS_CORE_TRACE("Returned to Directory: {}", mCurrentDirectory.string());
            }
            ImGui::SameLine();
        }

        ImGuiWindowFlags child_window_flags = ImGuiWindowFlags_AlwaysUseWindowPadding;
        ImGui::SetNextWindowBgAlpha(.3f);
        ImGui::BeginChild("Assets", { 0, 0 }, false, child_window_flags);
        ImGuiTableFlags table_flags = ImGuiTableFlags_None;
        if (ImGui::BeginTable("Assets Layout", column_count, table_flags)) {
            for (auto const& entry : std::filesystem::directory_iterator(mCurrentDirectory)) {
                auto const& path = entry.path();
                auto const& relative_path = std::filesystem::relative(path, ASSETS_PATH);
                std::string filename_string = relative_path.filename().string();
                const bool is_directory = entry.is_directory();

                ImGui::TableNextColumn();
                ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
                ImTextureID icon = EditorUtils::ConvertTextureID(is_directory ? mFolderIcon->texture_id : mFileIcon->texture_id);
                ImGui::ImageButton(("##" + filename_string).c_str(), icon, { mControls.mThumbnailSize, mControls.mThumbnailSize });
                ImGui::PopStyleColor();

                // Start dragging item
                if (ImGui::BeginDragDropSource()) {
                    std::string item_path = relative_path.string().c_str();
                    ImGui::SetDragDropPayload("ASSET_BROWSER_ITEM", item_path.c_str(), item_path.size() * sizeof(std::string));

                    ImVec2 image_size = { 48.f, 48.f };
                    ImGui::Image(icon, image_size);

                    ImGui::EndDragDropSource();
                }
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    if (is_directory) {
                        mCurrentDirectory /= path.filename();
                        IS_CORE_TRACE("Entered Directory: {}", mCurrentDirectory.string());
                    } else {
                        std::string filepath = mCurrentDirectory.string() + "\\" + path.filename().string();
                        ShellExecute(NULL, NULL, filepath.c_str(), NULL, NULL, SW_SHOW);
                    }
                }

                ImGui::TextWrapped(filename_string.c_str());
            }
            ImGui::EndTable(); // end table Assets Layout
        }

        ImGui::EndChild(); // end child window Assets

        ImGui::End(); // end window Asset Browser
    }

    void AssetBrowserPanel::RenderControls() {
        // Browser Controls
        float label_width = ImGui::CalcTextSize("Thumbnail Size").x + 2 * ImGui::GetStyle().FramePadding.x;
        if (ImGui::BeginTable("Browser Control", 2, 0, {}, label_width)) {
            ImGuiIO& io = ImGui::GetIO();
            ImFont* const font_bold = io.Fonts->Fonts[EditorUtils::FontTypeToInt(aFontType::FONT_TYPE_BOLD)];
            float panel_width = ImGui::GetContentRegionAvail().x;
            ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_WidthFixed;

            ImGui::TableSetupColumn("Browser Control", column_flags, label_width);

            ImGui::TableNextColumn();
            ImGui::PushFont(font_bold);
            ImGui::TextUnformatted("Thumbnail Size");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(100.f);
            ImGui::SliderFloat("##thumbnail size", &mControls.mThumbnailSize, MIN_THUMBNAIL_SIZE, panel_width, "%.2f");

            ImGui::TableNextColumn();
            ImGui::PushFont(font_bold);
            ImGui::TextUnformatted("Padding");
            ImGui::PopFont();
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(100.f);
            ImGui::SliderFloat("##padding", &mControls.mPadding, MIN_PADDING, MAX_PADDING, "%.2f");

            ImGui::EndTable();
        }
    }

} // end namespace IS
