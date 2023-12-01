/*!
 * \file Button.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 02-11-2023
 * \brief
 * This is a simple in-game GUI system that defines a single button component and the 
 * possible states that it could be in. To be updated in the future when text can be
 * drawn more freely.
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                   guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_BUTTON_H
#define GAM200_INSIGHT_ENGINE_SOURCE_BUTTON_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Engine/ECS/System.h"
#include "Engine/ECS/Component.h"
#include "Engine/Systems/Input/Input.h"
#include "Graphics/System/Sprite.h"


namespace IS {

    /*!
     * \enum ButtonTypes
     * \brief Enumerates the types of buttons.
     */
    enum ButtonTypes {
        Click,
        Slide
    };

    /*!
     * \enum ButtonStates
     * \brief Enumerates the possible states of a button.
     */ 
    enum ButtonStates {
        Idle,
        Hovered,
        Pressed
    };

    /*!
     * \class ButtonComponent
     * \brief Represents an entity as a button in the GUI.
     * \details
     * This component is used to represent an entity as a button with different states
     * in the GUI system.
     */
    //Components for GUI (to represent an entity as a button)
    class ButtonComponent : IComponent {
    public:
        static std::string GetType() {
            return "ButtonComponent";
        }
        std::string mButtonText;
        int mButtonType{};
        int mButtonState{};
        std::string ImageName{};
        Vector2D mSize{};
        float mSizeScale=1.f;
        float mIdleAlpha{ 1.f };
        float mHoverAlpha{ 1.f };
        float mClickAlpha{ 1.f };
        float mIdleScale{ 1.f };
        float mHoverScale{ 1.f };
        bool mTransformUpdate = false;

        ButtonComponent()
        {
            // empty by design
        }

        void followTransform(Transform const& trans) { mSize = trans.scaling; }

        Json::Value Serialize();

        void Deserialize(Json::Value data);
    };


    /*!
     * \class GuiSystem
     * \brief A system to manage the GUI components and interactions.
     * \details
     * This system manages the initialization, update, and interaction of GUI components
     * such as buttons.
     */
    class GuiSystem : public ParentSystem {
    public:        

        std::string GetName() {
            return "GuiSystem";
        }


        /*!
         * \brief Checks if a game button contains the mouse cursor.
         * \param mouse_x The x-coordinate of the mouse cursor.
         * \param mouse_y The y-coordinate of the mouse cursor.
         * \param entity The entity representing the button.
         * \return True if the mouse cursor is over the button, false otherwise.
         */
        bool GameButtonContainsMouse(float mouse_x, float mouse_y, Entity entity);

        void Initialize()
        {}

        /*!
         * \brief Updates the GUI system.
         * \param delta_time The time elapsed since the last frame.
         */
        void Update([[maybe_unused]] float delta_time);

    private:


    };

}
//didn't include a destructor because theres nothing to free.

#endif 