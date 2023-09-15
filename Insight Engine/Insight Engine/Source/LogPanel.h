#ifndef GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_LOGPANEL_H
#define GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_LOGPANEL_H

#include <imgui.h>

namespace IS {

    struct GUILog {
        ImGuiTextBuffer buffer;
        ImGuiTextFilter filter;
        ImVector<int>   line_offsets;   // index to line offset
        bool            auto_scroll;    // keep scrolling if already at the bottom

        GUILog();

        void clear();
        void addLog(const char* fmt, ...);
        void draw(const char* title);
    };

    void RenderLogConsolePanel();

} // end namespace

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_EDITOR_PANELS_LOGPANEL_H
