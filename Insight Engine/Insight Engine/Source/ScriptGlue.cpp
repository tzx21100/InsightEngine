/* Start Header **************************************************************/
/*!
\file    ScriptGlue.cpp
\author  Matthew Ng, matthewdeen.ng@digipen.edu
\par     Course: CSD2401
\date    27-09-2023
\brief   This source file defines the implementation of the ScriptGlue class, which
         registers C++ functions to be accessible from C# scripts. It includes
         functions for logging messages and vectors.

\copyright
All content (C) 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*____________________________________________________________________________*/
/* End Header ****************************************************************/

#pragma once

/* includes */
#include "Pch.h"
#include "GameGui.h"
#include <mono/metadata/object.h>

namespace IS {
    // Macro to add internal calls for C# access
#define IS_ADD_INTERNAL_CALL(Name) mono_add_internal_call("IS.InternalCalls::" #Name, Name)


    /**
     * \brief Native function to log a message with an integer parameter.
     * \param name The message to log.
     * \param param The integer parameter to log.
     */
    static void NativeLog(MonoString* name, int param) {
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str); // Convert char* to C++ string
        mono_free(c_str); // Free the allocated char*
        std::cout << str << "," << param << std::endl;
    }

    /**
     * \brief Native function to log a glm::vec3 vector.
     * \param param The glm::vec3 vector to log.
     */
    static void NativeLogVector(glm::vec3* param) {
        auto values = *param; // Get values from the vector
        std::cout << values.x << "," << values.y << "," << values.z << std::endl;
    }


    // registering input functions

    static bool KeyPressed(int keycode) {
        std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        return input->IsKeyPressed(keycode);
    }

    static bool KeyHeld(int keycode) {
        std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        return input->IsKeyHeld(keycode);
    }

    static bool KeyReleased(int keycode) {
        std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        return input->IsKeyReleased(keycode);
    }

    static bool MousePressed(int mousebutton) {
        std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        return input->IsMouseButtonPressed(mousebutton);
    }

    static bool MouseHeld(int mousebutton) {
        std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        return input->IsMouseButtonHeld(mousebutton);
    }

    static bool MouseReleased(int mousebutton) {
        std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        return input->IsMouseButtonReleased(mousebutton);
    }

    static Entity GetCurrentEntity(MonoString* name) {
        auto& engine = InsightEngine::Instance();
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str); // Convert char* to C++ string
        mono_free(c_str); // Free the allocated char*
        return engine.GetEntityByName(str);
    }

    static void RigidBodyAddForceEntity(float x, float y, Entity entity_id) {
        auto &engine = InsightEngine::Instance();
        auto& body_component = engine.GetComponent<RigidBody>(entity_id);
        Vector2D vec(x, y);
        body_component.AddVelocity(vec);
    }

    static void RigidBodyAddForce(float x, float y) {
        auto& engine = InsightEngine::Instance();
        auto& body_component = engine.GetComponent<RigidBody>(engine.GetScriptCaller());
        Vector2D vec(x, y);
        body_component.AddVelocity(vec);
    }

    static void TransformSetPosition(float x, float y) {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(engine.GetScriptCaller());
        trans_component.world_position.x=x;
        trans_component.world_position.y = y;
    }

    static void TransformSetScale(float x, float y) {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(engine.GetScriptCaller());
        trans_component.scaling.x = x;
        trans_component.scaling.y = y;
    }

    static void TransformSetRotation(float angle,float angle_speed) {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(engine.GetScriptCaller());
        trans_component.setRotation(angle,angle_speed);
    }

    static SimpleVector2D GetTransformPosition() {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(engine.GetScriptCaller());
        SimpleVector2D vec;
        vec.x = trans_component.getWorldPosition().x;
        vec.y = trans_component.getWorldPosition().y;
        return vec;
    }

    static SimpleVector2D GetTransformScaling() {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(engine.GetScriptCaller());
        SimpleVector2D vec;
        vec.x = trans_component.getScaling().x;
        vec.y = trans_component.getScaling().y;
        return vec;
    }

    static float GetTransformRotation() {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(engine.GetScriptCaller());
        float vec;
        vec = trans_component.getRotation();
        return vec;
    }

    static float GetRigidBodyAngularVelocity() {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<RigidBody>(engine.GetScriptCaller());
        float vec;
        vec = trans_component.mAngularVelocity;
        return vec;
    }

    static float GetDeltaTime() {
        auto system = InsightEngine::Instance().GetSystem<ScriptManager>("ScriptManager");
         return 1.f / 60.f/*system->mScriptDeltaTime*/;
    }

    static void SetSpriteImage(SimpleImage image) {
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(InsightEngine::Instance().GetScriptCaller());
        Image a = ConvertToImage(image);
        sprite_component.img = a;
    }


    static SimpleImage GetSpriteImage(MonoString* name) {
        auto script_sys = InsightEngine::Instance().GetSystem<ScriptManager>("ScriptManager");
 
        auto system = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str);
        mono_free(c_str);
        Image* img = system->GetImage(str);

        img->mFileName = str;
       // IS_CORE_DEBUG("{}", img->mFileName);
        return ConvertToSimpleImage(img);
        
    }

    static void EmplaceImageToGraphics(SimpleImage image) {
        ISGraphics::textures.emplace_back(ConvertToImage(image));
    }

    static void SetSpriteAnimationIndex(int num) {
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(InsightEngine::Instance().GetScriptCaller());
        sprite_component.animation_index = num;
    }

    static void FreeSpriteImage(SimpleImage* simg)
    {
        auto script_sys = InsightEngine::Instance().GetSystem<ScriptManager>("ScriptManager");
        if (simg && simg->texture_index >= 0 && simg->texture_index < 32) {
            script_sys->availableIndices.push(simg->texture_index);
        }

        if (simg && simg->mFileName) {
            delete[] simg->mFileName;
            simg->mFileName = nullptr;
        }
    }

    static void CreateAnimationFromSprite(int row, int columns, float animation_time) {
        Animation animation;
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(InsightEngine::Instance().GetScriptCaller());
        animation.initAnimation(row, columns, animation_time);
        sprite_component.anims.emplace_back(animation);
        IS_CORE_DEBUG("Animsize: {}",sprite_component.anims.size());

    }

    static void ResetAnimations() {
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(InsightEngine::Instance().GetScriptCaller());
        sprite_component.anims.clear();
    }
    
    static void AttachCamera() {
        auto& transform_component = InsightEngine::Instance().GetComponent<Transform>(InsightEngine::Instance().GetScriptCaller());
        ISGraphics::cameras[Camera::mActiveCamera].UpdateCamPos(transform_component.world_position.x, transform_component.world_position.y);

    }

    static void AudioPlaySound(MonoString* name) {
        auto asset = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str);
        mono_free(c_str);
        asset->PlaySoundByName(str);
    }

    static void AudioPlayMusic(MonoString* name) {
        auto asset = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str);
        mono_free(c_str);
        asset->PlayMusicByName("music.wav");
    }

    static int GetButtonState() {
        auto& button=InsightEngine::Instance().GetComponent<ButtonComponent>(InsightEngine::Instance().GetScriptCaller());
        return button.mButtonState;
    }



    /**
     * \brief Registers C++ functions to be accessible from C# scripts.
     */
    void ScriptGlue::RegisterFunctions()
    {
        IS_ADD_INTERNAL_CALL(NativeLog);
        IS_ADD_INTERNAL_CALL(NativeLogVector);

        //input functions
        IS_ADD_INTERNAL_CALL(KeyPressed);
        IS_ADD_INTERNAL_CALL(KeyHeld);
        IS_ADD_INTERNAL_CALL(KeyReleased);
        IS_ADD_INTERNAL_CALL(MousePressed);
        IS_ADD_INTERNAL_CALL(MouseHeld);
        IS_ADD_INTERNAL_CALL(MouseReleased);

        // Get the entity by name
        IS_ADD_INTERNAL_CALL(GetCurrentEntity);

        // Physics 
        IS_ADD_INTERNAL_CALL(RigidBodyAddForce);
        IS_ADD_INTERNAL_CALL(RigidBodyAddForceEntity);
        IS_ADD_INTERNAL_CALL(GetRigidBodyAngularVelocity);

        // Transform
        IS_ADD_INTERNAL_CALL(TransformSetPosition);
        IS_ADD_INTERNAL_CALL(TransformSetRotation);
        IS_ADD_INTERNAL_CALL(TransformSetScale);
        IS_ADD_INTERNAL_CALL(GetTransformPosition);
        IS_ADD_INTERNAL_CALL(GetTransformScaling);
        IS_ADD_INTERNAL_CALL(GetTransformRotation);
        IS_ADD_INTERNAL_CALL(GetDeltaTime);

        // Images and Sprite
        IS_ADD_INTERNAL_CALL(SetSpriteImage);
        IS_ADD_INTERNAL_CALL(GetSpriteImage);
        IS_ADD_INTERNAL_CALL(EmplaceImageToGraphics);
        IS_ADD_INTERNAL_CALL(SetSpriteAnimationIndex);
        IS_ADD_INTERNAL_CALL(FreeSpriteImage);
        IS_ADD_INTERNAL_CALL(CreateAnimationFromSprite);
        IS_ADD_INTERNAL_CALL(ResetAnimations);

        // Camera
        IS_ADD_INTERNAL_CALL(AttachCamera);

        // Audio
        IS_ADD_INTERNAL_CALL(AudioPlaySound);
        IS_ADD_INTERNAL_CALL(AudioPlayMusic);

        // Button
        IS_ADD_INTERNAL_CALL(GetButtonState);

    }
}