/*!
 * \file AssetBrowserPanel.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 24-10-2023
 * \brief
 * This header file declares the interface for class AssetBrowserPanel which
 * encapsulates the functionalities of an asset browser panel.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_PANELS_ASSET_BROWSER_PANEL_H
#define GAM200_INSIGHT_ENGINE_EDITOR_PANELS_ASSET_BROWSER_PANEL_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Panel.h"
#include "Asset.h"

#include <filesystem>

namespace IS {

    /*! \class AssetBrowserPanel
     *
     * \brief A class representing an asset browser panel for managing and viewing assets.
     *
     * The \c AssetBrowserPanel class is derived from the \c Panel class and provides functionality
     * to browse and manage assets within a user interface. It includes features to render the panel
     * and various controls for configuring the asset browser's appearance and behavior.
     */
    class AssetBrowserPanel : public Panel
    {
    public:
        /*! \brief Constructor for the AssetBrowserPanel class.
         *
         * Initializes a new instance of the \c AssetBrowserPanel class.
         */
        AssetBrowserPanel();

        /*! \brief Renders the asset browser panel.
         *
         * This method is called to render the asset browser panel within the user interface.
         * It overrides the base class \c RenderPanel method.
         */
        void RenderPanel() override;

        /*!
         * \brief Renders the controls for configuring the asset browser's appearance and behavior.
         */
        void RenderControls();

    private:
        /// Minimum size for thumbnails displayed in the asset browser.
        static constexpr inline float MIN_THUMBNAIL_SIZE = 62.f;

        /// Minimum padding between thumbnails in the asset browser.
        static constexpr inline float MIN_PADDING = 1.f;

        /// Maximum padding between thumbnails in the asset browser.
        static constexpr inline float MAX_PADDING = 32.f;

        static std::filesystem::path ASSETS_PATH;

        /*! \struct Controls
         *
         * \brief A structure representing the configuration controls for the asset browser.
         *
         * The \c Controls struct includes properties for specifying the thumbnail size and padding.
         */
        struct Controls
        {
            float mThumbnailSize = MIN_THUMBNAIL_SIZE; ///< The size of thumbnails in the browser.
            float mPadding = 16.f; ///< The padding between thumbnails.
        };

        std::filesystem::path mCurrentDirectory; ///< The currently selected directory.
        Controls mControls; ///< Configuration controls for the asset browser.

        /*!
         * \brief Renders path from ASSETS_PATH as text.
         */
        void RenderPath();

        /*!
         * \brief Renders file browser.
         */
        void RenderAllAssetsBrowser();

        void RenderImportedAssets();
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_PANELS_ASSET_BROWSER_PANEL_H
