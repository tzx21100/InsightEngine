#include "Pch.h"
#include "LogPanel.h"

namespace IS {

    GUILog::GUILog() : auto_scroll(true) { clear(); }

    void GUILog::clear() {
        buffer.clear();
        line_offsets.clear();
        line_offsets.push_back(0);
    }

    void GUILog::addLog(const char* fmt, ...) {
        int old_size = buffer.size();
        va_list args;
        va_start(args, fmt);
        buffer.appendfv(fmt, args);
        va_end(args);
        for (int new_size{ buffer.size() }; old_size < new_size; ++old_size) {
            if (buffer[old_size] == '\n')
                line_offsets.push_back(old_size + 1);
        }
    }

    void GUILog::draw(const char* title) {
        ImGui::Begin(title);

        // Options menu
        if (ImGui::BeginPopup("Options")) {
            ImGui::Checkbox("Auto-scroll", &auto_scroll);
            ImGui::EndPopup();
        }

        // Main window
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool clear_flag = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy_flag = ImGui::Button("Copy");
        ImGui::SameLine();
        filter.Draw("Filter", -100.f);

        ImGui::Separator();

        if (ImGui::BeginChild("scrolling", { 0, 0 }, false, ImGuiWindowFlags_HorizontalScrollbar)) {
            if (clear_flag)
                clear();
            if (copy_flag)
                ImGui::LogToClipboard();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
            const char* buf = buffer.begin();
            const char* buf_end = buffer.end();
            if (filter.IsActive()) {
                for (int line{}; line < line_offsets.size(); ++line) {
                    const char* line_start = buf + line_offsets[line];
                    const char* line_end = (line + 1 < line_offsets.size()) ? (buf + line_offsets[line + 1] - 1) : buf_end;
                    if (filter.PassFilter(line_start, line_end))
                        ImGui::TextUnformatted(line_start, line_end);
                }
            } else {
                ImGuiListClipper clipper;
                clipper.Begin(line_offsets.size());
                while (clipper.Step()) {
                    for (int line{ clipper.DisplayStart }; line < clipper.DisplayEnd; ++line) {
                        const char* line_start = buf + line_offsets[line];
                        const char* line_end = (line + 1 < line_offsets.size()) ? (buf + line_offsets[line + 1] - 1) : buf_end;
                        ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
            }
            ImGui::PopStyleVar();

            if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.f);
        }
        ImGui::EndChild();
        ImGui::End();
    }

    void RenderLogConsolePanel() {
        static GUILog log;

        ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
        ImGui::Begin("Log Console");
        if (ImGui::SmallButton("[Debug] Add 5 entries")) {
            static int counter = 0;
            const char* categories[3] = { "info", "warn", "error" };
            const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
            for (int n = 0; n < 5; n++) {
                const char* category = categories[counter % IM_ARRAYSIZE(categories)];
                const char* word = words[counter % IM_ARRAYSIZE(words)];
                log.addLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s' \n",
                           ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
                counter++;
            }
        }
        ImGui::End();

        // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
        log.draw("Log Console");
    }

} // end namespace IS
