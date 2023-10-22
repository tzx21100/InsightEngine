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
        struct Controls {
            float mThumbnailSize = 128.f;
            float mPadding = 16.f;
        };

        std::filesystem::path mCurrentDirectory;
        Controls mControls;

        std::shared_ptr<Image> mFolderIcon;
        std::shared_ptr<Image> mFileIcon;
        std::shared_ptr<Image> mBackIcon;

        static constexpr inline float MIN_THUMBNAIL_SIZE = 62.f;
        static constexpr inline float MIN_PADDING = 0.f;
        static constexpr inline float MAX_PADDING = 32.f;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_PANELS_ASSET_BROWSER_PANEL_H
