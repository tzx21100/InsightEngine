/*!
 * \file SceneHierarchyPanel.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for class SceneHierarchyPanel,
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
#include "InspectorPanel.h"
#include "Entities.h"

// Dependencies
#include <memory> // for std::shared_ptr
#include <imgui.h>

namespace IS {

    /*!
     * \brief The SceneHierarchyPanel class provides a panel for managing scene entities.
     */
    class SceneHierarchyPanel : public Panel {
    public:
        using EntityPtr = std::shared_ptr<Entity>; ///< Type alias for a shared pointer to an Entity.

        /*!
         * \brief Constructs a SceneHierarchyPanel object.
         */
        SceneHierarchyPanel() = default;

        /*!
         * \brief Destroys the SceneHierarchyPanel object.
         */
        ~SceneHierarchyPanel() = default;

        void ProcessShortcuts();

        /*!
         * \brief Renders the panel for the scene hierarchy.
         */
        void RenderPanel() override;

        /*!
         * \brief Resets the current selection.
         */
        void ResetSelection();

        /*!
         * \brief Getter for entity selected currently.
         * \return Pointer to the entity selected currently.
         */
        EntityPtr GetSelectedEntity();

    private:
        ImGuiTextFilter mFilter; ///< Filter for scnehierarchy hierarchy.
        EntityPtr mSelectedEntity; ///< The selected Entity in the hierarchy.

        /*!
         * \brief Renders details about the active scene.
         */
        void RenderActiveSceneDetails();

        /*!
         * \brief Renders a node for the specified scene in the scene hierarchy.
         * \param scene The scene to be rendered.
         */
        void RenderSceneNode(SceneID scene);

        /*!
         * \brief Renders a node for the specified Entity in the scene hierarchy.
         * \param entity The Entity to be rendered.
         */
        void RenderEntityNode(Entity entity);

        /*!
         * \brief Renders a node for the specified Entity in the scene hierarchy.
         * \param entity The Entity to be rendered.
         */
        void ProcessSelectedEntityShortcuts();

        ///*!
        // * \brief Renders configuration for the specified Entity.
        // * \param entity The Entity to be deleted.
        // * \param show Boolean flag to determine where to show window.
        // */
        //void RenderConfirmDelete(Entity entity, bool& show);

        void CloneEntity(Entity entity);
        void DeleteEntity(Entity entity);

        static void RenderAddComponent(Entity entity);

        friend class InspectorPanel; ///< Grant InspectorPanel access to private data members.
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_PANELS_SCENE_HIERARCHY_PANEL_H
