/*!
 * \file Button.cpp
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2451
 * \date 02-11-2023
 * \brief
 * This is a simple in-game GUI system that defines a single button component and the
 * possible states that it could be in. To be updated in the future when text can be
 * drawn more freely.
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Button.h"

namespace IS {

    Json::Value ButtonComponent::Serialize()
    {
        Json::Value button_data;
        button_data["ButtonText"] = mButtonText;
        button_data["ButtonType"] = mButtonType;
        button_data["ButtonState"] = mButtonState;
        button_data["ButtonImageName"] = ImageName;
        Json::Value button_size;
        button_size["X"] = mSize.x;
        button_size["Y"] = mSize.y;
        button_data["ButtonSize"] = button_size;
        button_data["ButtonTextScale"] = mSizeScale;
        button_data["ButtonIdleAlpha"] = mIdleAlpha;
        button_data["ButtonHoverAlpha"] = mHoverAlpha;
        button_data["ButtonClickAlpha"] = mClickAlpha;
        button_data["ButtonIdleScale"] = mIdleScale;
        button_data["ButtonHoverScale"] = mHoverScale;
        return button_data;
    }

    void ButtonComponent::Deserialize(Json::Value data)
    {
        mButtonText = data["ButtonText"].asString();
        mButtonType = data["ButtonType"].asInt();
        mButtonState = data["ButtonState"].asInt();
        ImageName = data["ButtonImageName"].asString();
        mSize = { data["ButtonSize"]["X"].asFloat(), data["ButtonSize"]["Y"].asFloat() };
        mSizeScale = data["ButtonSizeScale"].asFloat();
        mIdleAlpha = data["ButtonIdleAlpha"].asFloat();
        mHoverAlpha = data["ButtonHoverAlpha"].asFloat();
        mClickAlpha = data["ButtonClickAlpha"].asFloat();
        mIdleScale = data["ButtonIdleScale"].asFloat();
        mHoverScale = data["ButtonHoverScale"].asFloat();
    }

    bool GuiSystem::GameButtonContainsMouse(float mouse_x, float mouse_y, Entity entity)
    {
        auto& engine = InsightEngine::Instance();
        auto& trans = engine.GetComponent<Transform>(entity);
        float button_x = trans.getWorldPosition().x;
        float button_y = trans.getWorldPosition().y;
        float button_left = button_x - trans.getScaling().x / 2;
        float button_right = button_left + trans.getScaling().x;
        float button_top = button_y + trans.getScaling().y / 2;
        float button_bottom = button_y - trans.getScaling().y / 2;

        if (mouse_x<button_left || mouse_x>button_right || mouse_y <button_bottom || mouse_y>button_top)
        {
            return false;
        }
        return true;
    }

    void GuiSystem::Update([[maybe_unused]] float delta_time)
    {

        if (InsightEngine::Instance().mRuntime == false)
        {
            return;
        }

        auto& engine = InsightEngine::Instance();
        //get the input system
        std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        float mouse_x = (float)Transform::GetMousePosition().first;
        float mouse_y = (float)Transform::GetMousePosition().second;
        for (auto& entity : mEntities)
        {
            //add the needed components if they do not have
            if (!engine.HasComponent<Transform>(entity))
            {
                engine.AddComponent<Transform>(entity, Transform());
            }
            if (!engine.HasComponent<Sprite>(entity))
            {
                engine.AddComponent<Sprite>(entity, Sprite());
            }
            auto& button_component = engine.GetComponent<ButtonComponent>(entity);
            auto& sprite_component = engine.GetComponent<Sprite>(entity);
            auto& trans_component = engine.GetComponent<Transform>(entity);
            if (!button_component.mTransformUpdate)
            {
                button_component.followTransform(trans_component);
                button_component.mTransformUpdate = true;
            }
            button_component.mSizeScale = button_component.mIdleScale; // Idle button size
            // mouse is hovered over button
            if (GameButtonContainsMouse(mouse_x, mouse_y, entity))
            {
                button_component.mButtonState = ButtonStates::Hovered;
                sprite_component.color.a = button_component.mHoverAlpha;
                button_component.mSizeScale = button_component.mHoverScale;
                //if clicks
                if (input->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
                {
                    button_component.mButtonState = ButtonStates::Pressed;
                    sprite_component.color.a = button_component.mClickAlpha;
                }
            } else
            {
                button_component.mButtonState = ButtonStates::Idle;
                sprite_component.color.a = button_component.mIdleAlpha;
                button_component.mSizeScale = button_component.mIdleScale;
            }
            trans_component.setScaling(button_component.mSize * button_component.mSizeScale);

            //Text::addTextRenderCall(button_component.mButtonText, 0.5f, 0.5f, 48.f, glm::vec3(0.f, 1.f, 0.f));
            /*float win_width = static_cast<float>(engine.IsFullScreen() ? engine.GetMonitorWidth() : engine.GetMonitorWidth());
            float win_height = static_cast<float>(engine.IsFullScreen() ? engine.GetMonitorHeight() : engine.GetMonitorHeight());
            float x = (trans_component.world_position.x + win_width / 2.f) / win_width;
            float y = (trans_component.world_position.y + win_height / 2.f) / win_height;

            Text::addTextRenderCall(button_component.mButtonText, x, y, 48.f, glm::vec3(1.f, 1.f, 1.f));*/
        }

    }

} // end namespace IS
