/*!
 * \file GameGui.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_GUI_H
#define GAM200_INSIGHT_ENGINE_SOURCE_GUI_H

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "System.h"
#include "Component.h"


namespace IS {

    enum ButtonTypes {
        Click,
        Slide
    };

    enum ButtonStates {
        Idle,
        Hovered,
        Pressed
    };

    //Components for GUI (to represent an entity as a button)
    class ButtonComponent : IComponent {
    public:
        static std::string GetType() {
            return "ButtonComponent";
        }
        std::string mButtonText;
        int mButtonType{};
        int mButtonState{};
        std::string ImageName;

        Json::Value Serialize() {
           Json::Value button_data;
           button_data["ButtonText"] = mButtonText;
           button_data["ButtonType"] = mButtonType;
           button_data["ButtonState"] = mButtonState;
           button_data["ButtonImageName"] = ImageName;
           return button_data;
        }

        void Deserialize(Json::Value data) {
            
            mButtonText = data["ButtonText"].asString();
            mButtonType = data["ButtonType"].asInt();
            mButtonState = data["ButtonState"].asInt();
            ImageName = data["ButtonImageName"].asString();
        }
    };


    class GuiSystem : public ParentSystem {
    public:

        

        std::string GetName() {
            return "GuiSystem";
        }

        bool GameButtonContainsMouse(float mouse_x,float mouse_y, Entity entity) {
            auto& engine = InsightEngine::Instance();
            auto& trans = engine.GetComponent<Transform>(entity);
            float button_x = trans.getWorldPosition().x;
            float button_y = trans.getWorldPosition().y;
            float button_left = button_x - trans.getScaling().x / 2;
            float button_right = button_left + trans.getScaling().x;
            float button_top = button_y + trans.getScaling().y / 2;
            float button_bottom = button_y - trans.getScaling().y / 2;

            if (mouse_x<button_left || mouse_x>button_right || mouse_y <button_bottom || mouse_y>button_top) {
                return false;
            }
            return true;
        }

        void Initialize() {}

        void Update([[maybe_unused]] float delta_time) {
            auto& engine = InsightEngine::Instance();
            //get the input system
            std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
            float mouse_x = (float)input->GetMousePosition().first;
            float mouse_y = (float)input->GetMousePosition().second;
            for (auto& entity : mEntities) {
                //add the needed components if they do not have
                if (!engine.HasComponent<Transform>(entity)) {
                    engine.AddComponent<Transform>(entity,Transform());
                }
                if (!engine.HasComponent<Sprite>(entity)) {
                    engine.AddComponent<Sprite>(entity, Sprite());
                }
                auto& button_component = engine.GetComponent<ButtonComponent>(entity);
                // mouse is hovered over button
                if (GameButtonContainsMouse(mouse_x, mouse_y, entity)) {
                    button_component.mButtonState = ButtonStates::Hovered;
                    //if clicks
                    if (input->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) {
                        button_component.mButtonState = ButtonStates::Pressed;
                    }
                }
                else {
                    button_component.mButtonState = ButtonStates::Idle;
                }



            }

        }

    private:


    };

}
//didn't include a destructor because theres nothing to free.

#endif // GAM200_INSIGHT_ENGINE_SOURCE_INPUT_H  