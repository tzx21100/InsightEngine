/*!
 * \file BrowserPanel.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2451
 * \date 24-10-2023
 * \brief
 * This header file declares the interface for class BrowserPanel which
 * encapsulates the functionalities of an asset browser panel.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
#ifdef USING_IMGUI

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_PANELS_ASSET_BROWSER_PANEL_H
#define GAM200_INSIGHT_ENGINE_EDITOR_PANELS_ASSET_BROWSER_PANEL_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Panel.h"
#include "Engine/Systems/Asset/Asset.h"
#include "Editor/Utils/FileUtils.h"

#include <filesystem>

namespace IS {

    /*! \class BrowserPanel
     *
     * \brief A class representing an asset browser panel for managing and viewing assets.
     *
     * The \c BrowserPanel class is derived from the \c Panel class and provides functionality
     * to browse and manage assets within a user interface. It includes features to render the panel
     * and various controls for configuring the asset browser's appearance and behavior.
     */
    class BrowserPanel : public Panel
    {
    public:

        /*! \brief Constructor for the BrowserPanel class.
         *
         * Initializes a new instance of the \c BrowserPanel class.
         */
        BrowserPanel(EditorLayer& editor_layer)
            : Panel("Browser", editor_layer), mCurrentDirectory(ASSETS_PATH), mSelectedImportedAsset(IMPORTED), mShowImportedAssets(false), mChildWindowHovered(false) {}

        void UpdatePanel() override;

        /*! \brief Renders the asset browser panel.
         *
         * This method is called to render the asset browser panel within the user interface.
         * It overrides the base class \c RenderPanel method.
         */
        void RenderPanel() override;

    private:
        /// Minimum size for thumbnails displayed in the asset browser.
        static constexpr inline float MIN_THUMBNAIL_SIZE = 62.f;

        /// Minimum padding between thumbnails in the asset browser.
        static constexpr inline float MIN_PADDING = 1.f;

        /// Maximum padding between thumbnails in the asset browser.
        static constexpr inline float MAX_PADDING = 32.f;

        static const std::string IMPORTED;

        static std::filesystem::path ASSETS_PATH;

        ImGuiTextFilter mFilter;

        /*! \struct Controls
         *
         * \brief A structure representing the configuration controls for the asset browser.
         *
         * The \c Controls struct includes properties for specifying the thumbnail size and padding.
         */
        struct Controls
        {
            float mThumbnailSize = MIN_THUMBNAIL_SIZE; ///< The size of thumbnails in the browser.
            float mPadding = MAX_PADDING * 0.5f; ///< The padding between thumbnails.
        };

        std::filesystem::path mCurrentDirectory; ///< The currently selected directory.
        std::string mSelectedImportedAsset; ///< The type of selected asset.
        bool mShowImportedAssets; ///< Boolean flags indicating to show imported assets.
        bool mChildWindowHovered;
        Controls mControls; ///< Configuration controls for the asset browser.

        /*!
         * \brief Renders path of imported assets.
         */
        void RenderImportedPath();

        /*!
         * \brief Renders assets browser for imported assets.
         */
        void RenderImportedAssets();

        /*!
         * \brief Renders assets tree browser for imported assets.
         */
        void RenderImportedAssetsTree();

        /*!
         * \brief Switches the current imported asset type.
         * 
         * \param asset The type of asset to switch into.
         */
        void SwitchImportedAsset(std::string const& asset);

        /*!
         * \brief Renders path from ASSETS_PATH as buttons.
         */
        void RenderPath();

        /*!
         * \brief Renders file browser.
         */
        void RenderAllAssetsBrowser();

        /*!
         * \brief Renders contents of a directory.
         * 
         * \param directory The directory to render.
         */
        void RenderDirectoryContents(std::filesystem::path const& directory);

        /*!
         * \brief Renders a directory as a tree node.
         * 
         * \param directory The directory to render.
         */
        void RenderDirectoryNode(std::filesystem::path const& directory);

        /*!
         * \brief Renders a directory as a tree leaf node.
         * 
         * \param directory The directory to render.
         */
        void RenderDirectoryLeafNode(std::filesystem::path const& directory);

        /*!
         * \brief Switches the current directory.
         * 
         * \param directory The directory to switch into.
         */
        void SwitchCurrentDirectory(std::filesystem::path const& directory);

        /*!
         * \brief Checks whether a directory has sub directory.
         * 
         * \param directory The directory to check for.
         * \return Boolean flag indicating whether a sub directory has been found.
         */
        bool HasSubDirectory(std::filesystem::path const& directory);

        /*!
         * \brief Renders the controls for configuring the asset browser's appearance and behavior.
         */
        void RenderControls();

        /*!
         * \brief Renders the folder of imported assets.
         *
         * This method is used to render a folder containing imported assets. It provides a visual representation
         * of the assets in the folder and allows for interaction with them.
         *
         * \param imported_assets An initializer list of strings representing the names of the imported assets.
         * \param selected_filename A reference to a string that will be updated with the name of the selected file.
         */
        void RenderImportedAssetFolder(std::initializer_list<std::string> imported_assets, std::string& selected_filename);

        /*!
         * \brief Renders an asset item.
         *
         * This method is used to render a single asset item. It provides a visual representation
         * of the asset and allows for interaction with it.
         *
         * \param item A string representing the name of the asset.
         * \param selected_filename A reference to a string that will be updated with the name of the selected file.
         * \param icon A reference to an ImTextureID that represents the icon of the asset.
         * \param aspect_ratio A float representing the aspect ratio of the asset. Default value is 1.f.
         */
        void RenderAssetItem(std::string const& item, std::string& selected_filename, ImTextureID& icon, float aspect_ratio = 1.f);

        /*!
         * \brief Renders the context menu for imported assets.
         *
         * This method is used to render a context menu for imported assets. It provides options for managing the assets.
         * If the 'Delete' option is selected, the asset is deleted from the hard disk and the container.
         *
         * \tparam Container The type of container holding the assets.
         * \tparam Iterator The type of iterator for the container.
         *
         * \param container Reference to the container holding the assets.
         * \param iterator Reference to the iterator pointing to the current asset.
         * \param filename The name of the file to delete.
         * \param directory The directory path from which to delete the file.
         */
        template <typename Container, typename Iterator>
        void RenderContextMenuImported(Container& container, Iterator& iterator, std::string const& filename, std::string const& directory)
        {
            if (ImGui::BeginPopupContextItem())
            {
                // Open file
                if (ImGui::MenuItem(ICON_LC_EXTERNAL_LINK "  Open File"))
                {
                    FileUtils::OpenFileFromDefaultApp(filename.c_str(), directory.c_str());
                }
                ImGui::SetItemTooltip("Opens file in its default application.");

                // Delete file
                if (ImGui::MenuItem(ICON_LC_TRASH_2 "  Delete File"))
                {
                    // Construct filepath
                    std::ostringstream oss;
                    oss << directory << filename;
                    if (directory == AssetManager::PREFAB_DIRECTORY)
                        oss << ".json";

                    // Delete from hard disk
                    if (FileUtils::FileDelete(oss.str()))
                    {
                        // Delete from container
                        iterator = container.erase(iterator);
                        if (iterator != container.begin())
                            iterator = std::prev(iterator);
                    }
                }
                ImGui::SetItemTooltip("Deletes file from local hard disk.");

                ImGui::EndPopup();
            }
        } // end RenderContextMenuImported()
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_PANELS_ASSET_BROWSER_PANEL_H

#endif // USING_IMGUI
