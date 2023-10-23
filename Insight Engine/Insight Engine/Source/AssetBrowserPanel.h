#ifndef GAM200_INSIGHT_ENGINE_EDITOR_PANELS_ASSET_BROWSER_PANEL_H
#define GAM200_INSIGHT_ENGINE_EDITOR_PANELS_ASSET_BROWSER_PANEL_H

#include "Panel.h"
#include "Sprite.h"

#include <filesystem>

namespace IS {

    class AssetBrowserPanel : public Panel {
    public:
        AssetBrowserPanel();

        void RenderPanel() override;
        void RenderControls();

    private:
        static constexpr inline float MIN_THUMBNAIL_SIZE = 62.f;
        static constexpr inline float MIN_PADDING = 1.f;
        static constexpr inline float MAX_PADDING = 32.f;

        struct Controls {
            float mThumbnailSize = MIN_THUMBNAIL_SIZE;
            float mPadding = 16.f;
        };

        std::filesystem::path mCurrentDirectory;
        Controls mControls;

        std::unordered_map<std::string, std::shared_ptr<Image>> mIcons;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_PANELS_ASSET_BROWSER_PANEL_H
