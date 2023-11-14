/*!
 * \file HierarchyPanel.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for class HierarchyPanel,
 * which encapsulates the functionalities of a scene hierarchy panel
 * akin to other game engines (i.e., Unity/Unreal Engine, etc.), which
 * manages the entities in a scene.
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H
#define GAM200_INSIGHT_ENGINE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Panel.h"
#include "Entities.h"
#include "SceneManager.h"

// Dependencies
#include <memory> // for std::shared_ptr
#include <imgui.h>

namespace IS {

    /*!
     * \brief The HierarchyPanel class provides a panel for managing scene entities.
     */
    class HierarchyPanel : public Panel {
    public:
        using EntityPtr = std::shared_ptr<Entity>; ///< Type alias for a shared pointer to an Entity.

        /*!
         * \brief Constructs a HierarchyPanel object.
         */
        HierarchyPanel(EditorLayer& editor_layer) : Panel("Hierarchy", editor_layer) {}

        void UpdatePanel() override {}

        /*!
         * \brief Renders the panel for the scene hierarchy.
         */
        void RenderPanel() override;

        /*!
         * \brief Renders configuration for entities.
         * 
         * Configurations include rename, save/load prefab,
         * add component, clone/delete entity.
         */
        void RenderEntityConfig(Entity entity);

        /*!
         * \brief Renders gui to add component to an entity.
         *
         * \param entity The Entity to add components to.
         */
        static void RenderAddComponent(Entity entity);

        /*!
         * \brief Gets the size of the scene hierarchy panel.
         * 
         * \return Size of the scene hierarchy panel.
         */
        Vec2 GetPanelSize() const;

    private:
        ImGuiTextFilter mFilter; ///< Filter for scene hierarchy.
        Vec2 mPanelSize; ///< The size of the scene hierarchy panel.

        /*!
         * \brief Renders details about the active scene.
         */
        void RenderActiveSceneDetails();

        /*!
         * \brief Renders a tree node displaying all layers.
         */
        void RenderLayerControls();

        /*!
         * \brief Renders a node for the specified scene in the scene hierarchy.
         * \param scene The scene to be rendered.
         * 
         */
        void RenderSceneNode(SceneID scene);

        /*!
         * \brief Renders a node for the specified Entity in the scene hierarchy.
         * 
         * \param entity The Entity to be rendered.
         */
        void RenderEntityNode(Entity entity);

        /*!
         * \brief Renders a node for the specified Entity in the scene hierarchy.
         * 
         * \param entity The Entity to be rendered.
         */
        void ProcessSelectedEntityShortcuts();

        ///*!
        // * \brief Renders configuration for the specified Entity.
        // 
        // * \param entity The Entity to be deleted.
        // * \param show Boolean flag to determine where to show window.
        // */
        //void RenderConfirmDelete(Entity entity, bool& show);

        /*!
         * \brief Clone an existing entity.
         * 
         * \param entity The Entity to be cloned.
         */
        void CloneEntity(Entity entity);

        /*!
         * \brief Delete an existing entity.
         * 
         * \param entity The Entity to be deleted.
         */
        void DeleteEntity(Entity entity);

        friend class EditorLayer; ///< Grant EditorLayer access to private data members.
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H
