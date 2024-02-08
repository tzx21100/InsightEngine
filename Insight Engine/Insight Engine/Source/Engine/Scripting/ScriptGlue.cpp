/* Start Header **************************************************************/
/*!
\file    ScriptGlue.cpp
\author  Matthew Ng, matthewdeen.ng@digipen.edu (35%)
         Tan Zheng Xun, t.zhengxun@digipen.edu  (40%)
         Guo Yiming, yiming.guo@digipen.edu (5%)
         Wu Zekai, zekai.wu@digipen.edu (20%)
\par     Course: CSD2401
\date    2-11-2023
\brief   This source file defines the implementation of the ScriptGlue class, which
         registers C++ functions to be accessible from C# scripts. 

\copyright
All content (C) 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*____________________________________________________________________________*/
/* End Header ****************************************************************/

/* includes */
#include "Pch.h"
#include "ScriptGlue.h"
#include "Engine/Systems/Button/Button.h"
#include "Engine/Systems/Input/Input.h"
#include "Graphics/Core/Graphics.h"
#include "Graphics/System/Text.h"
#include "Graphics/System/Light.h"
#include "Physics/Dynamics/Body.h"
#include "Physics/Collision/Collider.h"
#include "Physics/System/Physics.h"
#include "Graphics/System/ShaderEffects.h"

#include <vector>
#include <mono/metadata/object.h>
#include <algorithm>
#include <math.h>
#include "Engine/Systems/Category/Category.h"

namespace IS {
    // Macro to add internal calls for C# access
#define IS_ADD_INTERNAL_CALL(Name) mono_add_internal_call("IS.InternalCalls::" #Name, Name)


    /**
     * \brief Native function to log a message with an integer parameter.
     * \param name The message to log.
     * \param param The integer parameter to log.
     */
    static void NativeLog(MonoString* name, float param) {
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str); // Convert char* to C++ string
        mono_free(c_str); // Free the allocated char*
        IS_DEBUG("{}{}", str, param);
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

    static SimpleVector2D GetMousePosition() {
        auto input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        return SimpleVector2D((float)Transform::GetMousePosition().first, (float)Transform::GetMousePosition().second);
    }

    static void RigidBodyAddForceEntity(float x, float y, Entity entity_id) {
        auto& engine = InsightEngine::Instance();
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
    
    
    static void RigidBodySetForce(float x, float y) {
        auto& engine = InsightEngine::Instance();
        auto& body_component = engine.GetComponent<RigidBody>(engine.GetScriptCaller());
        Vector2D vec(x, y);
        //body_component.mVelocity=vec;
        body_component.SetVelocity(vec);
    }     
    
    static void RigidBodySetForceX(float x) {
        auto& engine = InsightEngine::Instance();
        auto& body_component = engine.GetComponent<RigidBody>(engine.GetScriptCaller());
        body_component.mVelocity.x = x;
    }    
    
    static void RigidBodySetVelocityEntity(float x, float y ,int entity) {
        auto& engine = InsightEngine::Instance();
        auto& body_component = engine.GetComponent<RigidBody>(entity);
        Vector2D vec(x, y);
        //body_component.mVelocity=vec;
        body_component.SetVelocity(vec);
    }

    static SimpleVector2D RigidBodyGetVelocity() {
        auto& engine = InsightEngine::Instance();
        auto& body_component = engine.GetComponent<RigidBody>(engine.GetScriptCaller());
        return SimpleVector2D(body_component.mVelocity.x,body_component.mVelocity.y);
    }

    static float RigidBodyGetVelocityX() {
        auto& engine = InsightEngine::Instance();
        auto& body_component = engine.GetComponent<RigidBody>(engine.GetScriptCaller());
        return body_component.mVelocity.x;
    }

    static float RigidBodyGetVelocityY() {
        auto& engine = InsightEngine::Instance();
        auto& body_component = engine.GetComponent<RigidBody>(engine.GetScriptCaller());
        return body_component.mVelocity.y;
    }

    static void TransformSetPosition(float x, float y) {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(engine.GetScriptCaller());
        trans_component.world_position.x=x;
        trans_component.world_position.y = y;
    }

    static void TransformSetPositionEntity(float x, float y, int entity) {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(entity);
        trans_component.world_position.x = x;
        trans_component.world_position.y = y;
    }


    static void TransformSetScale(float x, float y) {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(engine.GetScriptCaller());
        trans_component.scaling.x = x;
        trans_component.scaling.y = y;
    }

    static void TransformSetScaleEntity(float x, float y,int entity) {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(entity);
        trans_component.scaling.x = x;
        trans_component.scaling.y = y;
    }

    static void TransformSetRotation(float angle,float angle_speed) {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(engine.GetScriptCaller());
        trans_component.setRotation(angle,angle_speed);
    }

    static void TransformSetRotationEntity(float angle, float angle_speed, int entity) {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(entity);
        trans_component.setRotation(angle, angle_speed);
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

    static SimpleVector2D GetTransformPositionEntity(int entity) {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(entity);
        SimpleVector2D vec;
        vec.x = trans_component.getWorldPosition().x;
        vec.y = trans_component.getWorldPosition().y;
        return vec;
    }

    static SimpleVector2D GetTransformScalingEntity(int entity) {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(entity);
        SimpleVector2D vec;
        vec.x = trans_component.getScaling().x;
        vec.y = trans_component.getScaling().y;
        return vec;
    }

    static float GetTransformRotationEntity(int entity) {
        auto& engine = InsightEngine::Instance();
        auto& trans_component = engine.GetComponent<Transform>(entity);
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
        return static_cast<float>(InsightEngine::Instance().mFixedDeltaTime);
    }

    static float GetFixedDeltaTime() {
        return static_cast<float>(InsightEngine::Instance().mFixedDeltaTime);
    }

    static void SetSpriteImage(SimpleImage image) {
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(InsightEngine::Instance().GetScriptCaller());
        Image a = ConvertToImage(image);
        if (a.mFileName == sprite_component.img.mFileName) { return; }
        sprite_component.img = a;
    }    
    
    static void SetSpriteImageEntity(SimpleImage image,int entity) {
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(entity);
        Image a = ConvertToImage(image);
        if (a.mFileName == sprite_component.img.mFileName) { return; }
        sprite_component.img = a;
    }

    static void SetSpriteAlphaEntity(float alpha, int entity) {
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(entity);
        sprite_component.color.a = alpha;
    }

    static float GetSpriteAlphaEntity(int entity) {
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(entity);
        return sprite_component.color.a;
    }

    static void ResetSpriteAnimationFrameEntity(int entity) {
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(entity);
        sprite_component.anims[sprite_component.animation_index].resetAnimation();
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
        //if (simg && simg->texture_index >= 0 && simg->texture_index < 32) {
        //    script_sys->availableIndices.push(simg->texture_index);
        //}

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
    
    static void CreateAnimationFromSpriteEntity(int row, int columns, float animation_time,int entity) {
        Animation animation;
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(entity);
        animation.initAnimation(row, columns, animation_time);
        sprite_component.anims.emplace_back(animation);
        IS_CORE_DEBUG("Animsize: {}",sprite_component.anims.size());

    }

    static void ResetAnimations() {
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(InsightEngine::Instance().GetScriptCaller());
        sprite_component.anims.clear();
    }
    
    static void SetAnimationAlpha(float val) {
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(InsightEngine::Instance().GetScriptCaller());
        sprite_component.color = { 1.f, 1.f, 1.f, val };
    }
    
    static void AttachCamera(float xoffset, float yoffset) {
        auto& camera = ISGraphics::cameras3D[Camera3D::mActiveCamera];
        camera.SetPosition(xoffset, yoffset);
    }

    static void AudioPlaySound(MonoString* name,bool loop=0,float volume=1.f) {
        auto asset = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str);
        mono_free(c_str);
        asset->PlaySoundByName(str,loop,volume);
    }    
    
    static void AudioStopAllSounds() {
        auto audio = InsightEngine::Instance().GetSystem<ISAudio>("Audio");
        audio->StopAllAudio();
    }

    static void AudioPlayMusic(MonoString* name ,float volume) {
        auto asset = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str);
        mono_free(c_str);
        asset->PlayMusicByName(str ,1,volume,1);
    }

    static void AudioEmitterEnableEntity(bool enable , int entity) {
        auto& component = InsightEngine::Instance().GetComponent<AudioEmitter>(entity);
        if (enable) {
            component.isPlaying = true;
        }
        else {
            component.isPlaying = false;
        }
    }

    static void AudioSetMaster(float volume) {
        auto sys = InsightEngine::Instance().GetSystem<ISAudio>("Audio");
        sys->SetMasterVolume(volume);
    }

    static int GetButtonState() {
        auto& button=InsightEngine::Instance().GetComponent<ButtonComponent>(InsightEngine::Instance().GetScriptCaller());
        return button.mButtonState;
    }

    static int GetEntityButtonState(int entity)
    {
        auto& button = InsightEngine::Instance().GetComponent<ButtonComponent>(entity);
        return button.mButtonState;
    }

    static int CreateEntity(MonoString* name) {
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str);
        mono_free(c_str);
        Entity entity=InsightEngine::Instance().CreateEntity(str);
        InsightEngine::Instance().AddComponentAndUpdateSignature<Transform>(entity,Transform());
        //InsightEngine::Instance().AddComponentAndUpdateSignature<Sprite>(entity,Sprite());
        return static_cast<int>(entity);
    } 

    static int CloneEntity(int entity) {
        InsightEngine::Instance().CopyEntity(entity);
        return static_cast<int>(entity);
    }
    
    static int CreateEntitySprite(MonoString* name) {
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str);
        mono_free(c_str);
        Entity entity=InsightEngine::Instance().CreateEntity(str);
        InsightEngine::Instance().AddComponentAndUpdateSignature<Transform>(entity,Transform());
        InsightEngine::Instance().AddComponentAndUpdateSignature<Sprite>(entity,Sprite());
        return static_cast<int>(entity);
    }    
    
    static int CreateEntityVFX(MonoString* name, SimpleImage image) {
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str);
        mono_free(c_str);
        Entity entity=InsightEngine::Instance().CreateEntity(str);
        InsightEngine::Instance().AddComponentAndUpdateSignature<Transform>(entity,Transform());
        InsightEngine::Instance().AddComponentAndUpdateSignature<Sprite>(entity,Sprite());
        auto& sprite_component=InsightEngine::Instance().GetComponent<Sprite>(entity);
        sprite_component.layer = 1;
        sprite_component.img = ConvertToImage(image);
        ISGraphics::AddEntityToLayer(sprite_component.layer, entity);
        return static_cast<int>(entity);
    }

    static int CreateEntityUI(MonoString* name, SimpleImage image)
    {
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str);
        mono_free(c_str);
        Entity entity = InsightEngine::Instance().CreateEntity(str);
        InsightEngine::Instance().AddComponentAndUpdateSignature<Transform>(entity, Transform());
        InsightEngine::Instance().AddComponentAndUpdateSignature<Sprite>(entity, Sprite());
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(entity);
        sprite_component.layer = static_cast<int>(ISGraphics::mLayers.size()-1);
        sprite_component.img = ConvertToImage(image);
        ISGraphics::AddEntityToLayer(sprite_component.layer, entity);
        return static_cast<int>(entity);
    }

    static int CreateEntityButton(MonoString* name, SimpleImage image, MonoString* filename, MonoString* text)
    {
        auto& engine = InsightEngine::Instance();
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str);
        mono_free(c_str);
        Entity entity = engine.CreateEntity(str);
        engine.AddComponentAndUpdateSignature<Transform>(entity, Transform());
        engine.AddComponentAndUpdateSignature<Sprite>(entity, Sprite());
        engine.AddComponentAndUpdateSignature<ButtonComponent>(entity, ButtonComponent());
        auto& sprite_component = engine.GetComponent<Sprite>(entity);
        auto& button_component = engine.GetComponent<ButtonComponent>(entity);
        engine.AddComponentAndUpdateSignature<ScriptComponent>(entity, ScriptComponent());
        auto& script_component = engine.GetComponent<ScriptComponent>(entity);

        char* c_str2 = mono_string_to_utf8(filename);
        std::string str2(c_str2);
        mono_free(c_str2);
        script_component.mScriptName = str2;
        sprite_component.layer = static_cast<int>(ISGraphics::mLayers.size()-1);
        sprite_component.img = ConvertToImage(image);
        //button_component.followTransform();
        button_component.mIdleAlpha = 0.8f;
        button_component.mClickAlpha = 0.9f;
        button_component.mIdleScale = 0.9f;
        button_component.mHoverScale = 1.05f;

        char* c_str3 = mono_string_to_utf8(text);
        std::string str3(c_str3);
        mono_free(c_str3);
        button_component.mButtonText = str3;
        
        ISGraphics::AddEntityToLayer(sprite_component.layer, entity);
        return static_cast<int>(entity);
    }

    static int CreateEntityButtonNoText(MonoString* name, SimpleImage image, MonoString* filename)
    {
        auto& engine = InsightEngine::Instance();
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str);
        mono_free(c_str);
        Entity entity = engine.CreateEntity(str);
        engine.AddComponentAndUpdateSignature<Transform>(entity, Transform());
        engine.AddComponentAndUpdateSignature<Sprite>(entity, Sprite());
        engine.AddComponentAndUpdateSignature<ButtonComponent>(entity, ButtonComponent());
        auto& sprite_component = engine.GetComponent<Sprite>(entity);
        auto& button_component = engine.GetComponent<ButtonComponent>(entity);
        engine.AddComponentAndUpdateSignature<ScriptComponent>(entity, ScriptComponent());
        auto& script_component = engine.GetComponent<ScriptComponent>(entity);

        char* c_str2 = mono_string_to_utf8(filename);
        std::string str2(c_str2);
        mono_free(c_str2);
        script_component.mScriptName = str2;
        sprite_component.layer = static_cast<int>(ISGraphics::mLayers.size()-1);
        sprite_component.img = ConvertToImage(image);
        //button_component.followTransform();
        button_component.mIdleAlpha = 0.8f;
        button_component.mClickAlpha = 0.9f;
        button_component.mIdleScale = 0.9f;
        button_component.mHoverScale = 1.05f;

        ISGraphics::AddEntityToLayer(sprite_component.layer, entity);
        return static_cast<int>(entity);
    }

    static void DestroyEntity(int entity) {
        static_cast<Entity>(entity);
        InsightEngine::Instance().DeleteEntity(entity);
    }

    static bool EntityCheckCollide(int entity) {
        //auto system = InsightEngine::Instance().GetSystem<CollisionSystem>("CollisionSystem");
        //return system->CheckColliding(static_cast<Entity>(entity));
        auto const& component=InsightEngine::Instance().GetComponent<Collider>(entity);
        return component.mIsColliding;
    }

    //static void AddComponent(MonoString *name) {
    //    char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
    //    std::string str(c_str);
    //    mono_free(c_str);
    //
    //}

    static void AddCollider(int entity) {
        if (!InsightEngine::Instance().HasComponent<Collider>(entity))
        InsightEngine::Instance().AddComponentAndUpdateSignature<RigidBody>(entity, RigidBody());
        auto& body = InsightEngine::Instance().GetComponent<RigidBody>(entity);
        //auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
        body.mBodyType = BodyType::Ghost;
        InsightEngine::Instance().AddComponentAndUpdateSignature<Collider>(entity, Collider());
        auto& collider = InsightEngine::Instance().GetComponent<Collider>(entity);
        collider.mResponseEnable = false;
    }

    static void UpdateCategory(int entity, MonoString* category) {
        if (!InsightEngine::Instance().HasComponent<Category>(entity))
            InsightEngine::Instance().AddComponentAndUpdateSignature<Category>(entity, Category());
        auto& cate = InsightEngine::Instance().GetComponent<Category>(entity);
        char* c_str = mono_string_to_utf8(category); // Convert Mono string to char*
        std::string cate_name(c_str);
        mono_free(c_str);
        cate.mCategory = cate_name;
    }

    //static MonoString* GetCategory(int entity) {
    //    auto& cate = InsightEngine::Instance().GetComponent<Category>(entity);
    //    //return cate.mCategory;
    //    MonoString* str 
    //    //char* c_str = mono_string_to_utf8(category); // Convert Mono string to char*
    //    //std::string cate_name(c_str);
    //    //mono_free(c_str);
    //    //cate.mCategory = cate_name;
    //}

    static void ColliderNone(int entity) {
        auto& collider = InsightEngine::Instance().GetComponent<Collider>(entity);
        collider.mResponseEnable = false;
    }    
    
    static void ColliderComponentAdd(int entity , float scaleX,float scaleY) {
        if (!InsightEngine::Instance().HasComponent<Collider>(entity)) {
            InsightEngine::Instance().AddComponent<Collider>(entity, Collider());
            auto& collider = InsightEngine::Instance().GetComponent<Collider>(entity);
            collider.mBoxCollider.sizeScale.x = scaleX;
            collider.mBoxCollider.sizeScale.y = scaleY;
        }
    }    
    
    static void ColliderComponentRemove(int entity) {
        InsightEngine::Instance().RemoveComponent<Collider>(entity);
    }    
    
    static void RigidBodySetBodyTypeEntity(short body_type, int entity) {
        if (!InsightEngine::Instance().HasComponent<RigidBody>(entity)) {
            InsightEngine::Instance().AddComponent<RigidBody>(entity, RigidBody());
        }

        auto& component=InsightEngine::Instance().GetComponent<RigidBody>(entity);
        component.mBodyType = static_cast<BodyType>(body_type);
        if (body_type == 1) {
            InsightEngine::Instance().RemoveComponent<RigidBody>(entity);
            InsightEngine::Instance().AddComponent<RigidBody>(entity,RigidBody());
        }
    }

    static short RigidBodyGetBodyTypeEntity(int entity) {
    if (!InsightEngine::Instance().HasComponent<RigidBody>(entity)) {
        InsightEngine::Instance().AddComponent<RigidBody>(entity, RigidBody());
    }

    auto& component=InsightEngine::Instance().GetComponent<RigidBody>(entity);
    return static_cast<short>(component.mBodyType);
    }

    static void CameraSetZoom(float value) {
        ISGraphics::cameras3D[Camera3D::mActiveCamera].SetZoomLevel(value);
    }

    static float CameraGetZoom()
    {
       return ISGraphics::cameras3D[Camera3D::mActiveCamera].GetZoomLevel();
    }

    static int GetCurrentEntityID() {
        return InsightEngine::Instance().GetScriptCaller();
    }

    static float GetCollidedObjectAngle(int entity) {
        auto& collider = InsightEngine::Instance().GetComponent<Collider>(entity);
        return collider.mCollidedObjectAngle;
    }

    static void DrawLineBetweenPoints(float x1, float y1, float x2, float y2, std::tuple<float, float, float> color) {
        Vector2D point1 = Vector2D(x1, y1);
        Vector2D point2 = Vector2D(x2, y2);
        Vector2D dist = point2 - point1;
        ISVector2DNormalize(dist, dist);
        dist *= 500.f;
        Sprite::drawDebugLine(point1, point1+dist, color);
    }

    static void DrawCircle(float pos_x, float pos_y, float scale_x, float scale_y, std::tuple<float, float, float> color)
    {
        Sprite::drawDebugCircle(Vector2D(pos_x, pos_y), Vector2D(scale_x, scale_y), color);
    }    
    
    static void DrawSquare(float pos_x, float pos_y, float scale_x, float scale_y, float colx,float coly, float colz, float cola,int layer)
    {
        Sprite::draw_colored_quad(Vector2D(pos_x, pos_y), 0, Vector2D(scale_x, scale_y), { colx,coly,colz,cola }, layer);
    }

    static void DrawDarkCircle(float pos_x, float pos_y, float scale_x, float scale_y)
    {
        glLineWidth(200.f);
        Sprite::drawDebugCircle(Vector2D(pos_x, pos_y), Vector2D(scale_x, scale_y), { 0.f,0.f,0.f });
        glLineWidth(2.f);
    }

    static bool GetCollidingEntityCheck(int entity ,int entityToCheckAgainst) {
        auto& collider_component = InsightEngine::Instance().GetComponent<Collider>(entity);
        auto it = std::find(collider_component.mCollidingEntity.begin(), collider_component.mCollidingEntity.end(), static_cast<Entity>(entityToCheckAgainst));
        if (it != collider_component.mCollidingEntity.end()) { return true; }
        return false;
    }       
    
    static int GetCollidingEntity(int entity) {
        auto& collider_component = InsightEngine::Instance().GetComponent<Collider>(entity);
        return collider_component.mCollidingEntity.back();
    }

    static bool CheckEntityCategoryString(int entity_id, const char* category_to_check) {
        auto& engine = InsightEngine::Instance();
        //char* c_str = mono_string_to_utf8(category_to_check); // Convert Mono string to char*
        //std::string tag(c_str);
        //mono_free(c_str);
        auto& category = engine.GetComponent<Category>(entity_id);
        if (category.mCategory == category_to_check) {
            return true;
        }
        return false;
    }

    static int GetCollidingEnemyEntity(int entity) {
        int ret = -1;
        auto& collider_component = InsightEngine::Instance().GetComponent<Collider>(entity);
        for (int i = 0; i < collider_component.mCollidingEntity.size(); i++) {
            if (CheckEntityCategoryString(collider_component.mCollidingEntity[i], "Enemy")) {
                ret = collider_component.mCollidingEntity[i];
            }
        }
        return ret;
    }

    // not in use
    static std::vector<int> GetCollidingEntityList(int entity) {
        auto& collider_component = InsightEngine::Instance().GetComponent<Collider>(entity);
        std::vector<int> ret;
        for (int i = 0; i < collider_component.mCollidingEntity.size(); i++) {
            ret.push_back(collider_component.mCollidingEntity[i]);
        }
        return ret;
    }

    static void RemoveColliderComponentEntity(int entity) {
        InsightEngine::Instance().RemoveComponent<Collider>(entity);
    }

    static bool CollidingObjectIsStatic(int entity) {
        auto& body_component = InsightEngine::Instance().GetComponent<RigidBody>(entity);
        return body_component.mBodyType == BodyType::Static ? 1 : 0;
    }    
    
    static bool CollidingObjectIsSpikes(int entity) {
        auto& body_component = InsightEngine::Instance().GetComponent<RigidBody>(entity);
        return body_component.mBodyType == BodyType::Spikes ? 1 : 0;
    }        
    
    static bool CollidingObjectTypeIsWall(int entity) {
        auto& body_component = InsightEngine::Instance().GetComponent<RigidBody>(entity);
        return body_component.mBodyType == BodyType::WallClimb ? 1 : 0;
    }    
    
    static bool CollidingObjectTypeIsGhost(int entity) {
        auto& body_component = InsightEngine::Instance().GetComponent<RigidBody>(entity);
        return body_component.mBodyType == BodyType::Ghost ? 1 : 0;
    }    
    
    static bool CollidingObjectTypeIsIgnore(int entity) {
        auto& body_component = InsightEngine::Instance().GetComponent<RigidBody>(entity);
        return body_component.mBodyType == BodyType::Ignore ? true : false;
    }

    static void DrawImageAt(SimpleVector2D pos, float rotation, SimpleVector2D scale, SimpleImage image, float alpha, int layer=1) {
        
        Sprite::draw_textured_quad(Vector2D(pos.x,pos.y), rotation, Vector2D(scale.x,scale.y), ConvertToImage(image), alpha, layer);
    
    }

    static void DrawImageExtraAt(int rowIndex, int columnIndex, int totalRows, int totalCols, SimpleVector2D  pos, float rotation, SimpleVector2D  scale, SimpleImage texture, float alpha, int layer) {

       

        Sprite::drawSpritesheetFrame( rowIndex,  columnIndex,  totalRows,  totalCols, Vector2D(pos.x, pos.y),  rotation, Vector2D(scale.x, scale.y), ConvertToImage(texture),  alpha,  layer);

    }

    Animation free_anim;
    static void DrawNonEnityAnimation(float deltaTime, SimpleVector2D pos, float rotation, SimpleVector2D scale, SimpleImage  texture, float alpha, int layer=1) {
        free_anim.initAnimation(1, 6, 0.5f);
        free_anim.drawNonEntityAnimation( deltaTime, Vector2D(pos.x,pos.y),  rotation, Vector2D(scale.x,scale.y), ConvertToImage(texture), alpha, layer);
    }


    static void LoadScene(MonoString* file_name)
    {
        char* c_str = mono_string_to_utf8(file_name); // Convert Mono string to char*
        std::string str(c_str);
        SceneManager::Instance().LoadScene(str);
        mono_free(c_str);
        auto system = InsightEngine::Instance().GetSystem<ScriptManager>("ScriptManager");
        system->mLoadScene = true;
    }

    static void SwitchScene(int scene_id)
    {
        SceneManager::Instance().SwitchScene(static_cast<SceneID>(scene_id));
    }

    static void Exit()
    {
        InsightEngine::Instance().Exit();
    }

    static void GameSpawnParticle(float x, float y, MonoString* particle_name) {
        auto assey_sys = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
        auto system= InsightEngine::Instance().GetSystem<ParticleSystem>("Particle");
        char* c_str = mono_string_to_utf8(particle_name); // Convert Mono string to char*
        std::string part_name(c_str);
        mono_free(c_str);
        Particle particle = assey_sys->GetParticle(part_name);
        particle.mParticlePos = Vector2D(x, y);
        system->SpawnParticles(particle);
    }

    static void GameSpawnParticleExtra(float x, float y, float direction, float size, float size_scale,float alpha, float alpha_growth, float lifespan, float speed, MonoString* particle_name) {
        auto assey_sys = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
        auto system = InsightEngine::Instance().GetSystem<ParticleSystem>("Particle");
        char* c_str = mono_string_to_utf8(particle_name); // Convert Mono string to char*
        std::string part_name(c_str);
        mono_free(c_str);
        Particle particle = assey_sys->GetParticle(part_name);
        particle.mParticlePos = Vector2D(x, y);
        particle.mDirection = direction;
        particle.mScale = Vector2D(size, size);
        particle.mSizeGrowth = size_scale;
        particle.mAlpha = alpha;
        particle.mAlphaGrowth = alpha_growth;
        particle.mLifespan = lifespan;
        particle.mVelocity = Vector2D(speed,speed);
        system->SpawnParticles(particle);
    }

    static void GameSpawnParticleExtraFrames(float x, float y, float direction, float size, float size_scale, float alpha, float alpha_growth, float lifespan, float speed, MonoString* particle_name ,int col,int row) {
        auto assey_sys = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
        auto system = InsightEngine::Instance().GetSystem<ParticleSystem>("Particle");
        char* c_str = mono_string_to_utf8(particle_name); // Convert Mono string to char*
        std::string part_name(c_str);
        mono_free(c_str);
        Particle particle = assey_sys->GetParticle(part_name);
        particle.mParticlePos = Vector2D(x, y);
        particle.mDirection = direction;
        particle.mScale = Vector2D(size, size);
        particle.mSizeGrowth = size_scale;
        particle.mAlpha = alpha;
        particle.mAlphaGrowth = alpha_growth;
        particle.mLifespan = lifespan;
        particle.mVelocity = Vector2D(speed, speed);
        particle.mColIndex = col;
        particle.mRowIndex = row;
        system->SpawnParticles(particle);
    }

    static void GameSpawnParticleExtraImage(float x, float y, float direction, float sizex,float sizey, float size_scale, float alpha, float alpha_growth, float lifespan, float speed, MonoString* particle_name, MonoString* image_name) {
        auto assey_sys = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
        auto system = InsightEngine::Instance().GetSystem<ParticleSystem>("Particle");
        char* c_str = mono_string_to_utf8(particle_name); // Convert Mono string to char*
        std::string part_name(c_str);
        mono_free(c_str);
        c_str = mono_string_to_utf8(image_name); // Convert Mono string to char*
        std::string part_image_name(c_str);
        mono_free(c_str);
        Particle particle = assey_sys->GetParticle(part_name);
        particle.mParticlePos = Vector2D(x, y);
        particle.mDirection = direction;
        particle.mScale = Vector2D(sizex, sizey);
        particle.mSizeGrowth = size_scale;
        particle.mAlpha = alpha;
        particle.mAlphaGrowth = alpha_growth;
        particle.mLifespan = lifespan;
        particle.mVelocity = Vector2D(speed, speed);
        particle.mParticleType = pt_texture;
        particle.mImageName = part_image_name;
        system->SpawnParticles(particle);
    }

    static void GameSpawnParticleFrames(float x, float y,int col, int row, MonoString* particle_name) {
        auto assey_sys = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
        auto system = InsightEngine::Instance().GetSystem<ParticleSystem>("Particle");
        char* c_str = mono_string_to_utf8(particle_name); // Convert Mono string to char*
        std::string part_name(c_str);
        mono_free(c_str);
        Particle particle = assey_sys->GetParticle(part_name);
        particle.mParticlePos = Vector2D(x, y);
        particle = Particle::CreateParticleFrames(particle,col, row);
        system->SpawnParticles(particle);
    }


    static int GetCurrentAnimationEntity(int entity) {
        if (InsightEngine::Instance().HasComponent<Sprite>(entity)) {
            auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(entity);
            
            int frame_indexX = (int)sprite_component.anims[sprite_component.animation_index].frame_index.x;
            int frame_indexY = (int)sprite_component.anims[sprite_component.animation_index].frame_index.y;
            int x_frames = sprite_component.anims[sprite_component.animation_index].x_frames;

            return frame_indexY * x_frames + frame_indexX;
        }
        
        return 0;
    }

    static void GamePause(bool pause)
    {
        //auto physics = InsightEngine::Instance().GetSystem<Physics>("Physics");
        if (pause)
        {
            Physics::mGravity = Vector2D(0.f, 0.f);
        }
        else
        {
            Physics::mGravity = Vector2D(0.f, -981.f);
        }
    }

    static int GetTitleBarHeight()
    {
        return InsightEngine::Instance().GetTitleBarHeight();
    }

    static bool IsFullscreen()
    {
        return InsightEngine::Instance().IsFullScreen();
    }

    static void ToggleFullscreen()
    {
        auto& engine = InsightEngine::Instance();
        auto const& window = engine.GetSystem<WindowSystem>("Window");
        window->ToggleFullScreen();
    }

    static bool IsWindowFocused()
    {
        return InsightEngine::Instance().IsWindowFocused();
    }

    static void ShowCursor()
    {
        auto& engine = InsightEngine::Instance();
        auto const& window = engine.GetSystem<WindowSystem>("Window");
        window->ShowCursor();
    }

    static void HideCursor()
    {
        auto& engine = InsightEngine::Instance();
        auto const& window = engine.GetSystem<WindowSystem>("Window");
        window->HideCursor();
    }

    static void ButtonRenderText(int entity, float x, float y, float size, Vector3D color, MonoString* font)
    {
        auto& button_component = InsightEngine::Instance().GetComponent<ButtonComponent>(entity);
        char* f_str = mono_string_to_utf8(font);
        std::string font_name(f_str);
        mono_free(f_str);
        std::cout << "yeap";
        ISGraphics::mTexts[font_name].addTextRenderCall(button_component.mButtonText, x, y, size * button_component.mSizeScale, { color.x, color.y, color.z });
    }

    static void SetButtonSizeScale(int entity, float scale)
    {
        auto& button_component = InsightEngine::Instance().GetComponent<ButtonComponent>(entity);
        button_component.mSizeScale = scale;
    }

    static void SetButtonSize(int entity, SimpleVector2D size)
    {
        auto& button_component = InsightEngine::Instance().GetComponent<ButtonComponent>(entity);
        button_component.mSize = { size.x, size.y };
    }

    static float GetButtonHoverScale(int entity)
    {
        auto& button_component = InsightEngine::Instance().GetComponent<ButtonComponent>(entity);
        return button_component.mHoverScale;
    }

    static float GetButtonIdleScale(int entity)
    {
        auto& button_component = InsightEngine::Instance().GetComponent<ButtonComponent>(entity);
        return button_component.mIdleScale;
    }

    static void ButtonComponentAdd(int entity,float idle_alpha, float hover_alpha, float click_alpha, float idle_scale, float hover_scale) {
        if (!InsightEngine::Instance().HasComponent<ButtonComponent>(entity)) {
            InsightEngine::Instance().AddComponent<ButtonComponent>(entity, ButtonComponent());
            auto& button = InsightEngine::Instance().GetComponent<ButtonComponent>(entity);
            button.mSizeScale = 1.f;
            button.mIdleAlpha = idle_alpha;
            button.mHoverAlpha = hover_alpha;
            button.mClickAlpha = click_alpha;
            button.mIdleScale = idle_scale;
            button.mHoverScale = hover_scale;
        }
    }

    static bool ButtonComponentExists(int entity) {
        return InsightEngine::Instance().HasComponent<ButtonComponent>(entity);
    }
    
    static void ButtonComponentRemove(int entity) {
        InsightEngine::Instance().RemoveComponent<ButtonComponent>(entity);
    }

    static void RenderText(MonoString* text, float x, float y, float size, Vector3D color)
    {
        char* c_str = mono_string_to_utf8(text); // Convert Mono string to char*
        std::string part_name(c_str);
        mono_free(c_str);

        // Poiret will be default text font
        ISGraphics::mTexts["PoiretOne-Regular"].addTextRenderCall(c_str, x, y, size, { color.x, color.y, color.z });

        // ISGraphics::mTexts["Poiret_One_Regular"].renderText(c_str, x, y, size, { color.x,color.y,color.z });
    }

    static void RenderTextFont(MonoString* text, MonoString* font, float x, float y, float size, Vector3D color)
    {
        char* c_str = mono_string_to_utf8(text); // Convert Mono string to char*
        std::string part_name(c_str);
        mono_free(c_str);
        char* f_str = mono_string_to_utf8(font);
        std::string font_name(f_str);
        mono_free(f_str);
        ISGraphics::mTexts[font_name].addTextRenderCall(part_name, x, y, size, { color.x, color.y, color.z });

        //ISGraphics::mTexts[font_name].renderText(c_str, x, y, size, { color.x,color.y,color.z });
    }

    static int GetWindowWidth()
    {
        return InsightEngine::Instance().GetWindowWidth();
    }

    static int GetWindowHeight()
    {
        return InsightEngine::Instance().GetWindowHeight();
    }

    static SimpleVector2D GetCameraPos() {
        return SimpleVector2D(ISGraphics::cameras3D[Camera3D::mActiveCamera].mPosition.x, ISGraphics::cameras3D[Camera3D::mActiveCamera].mPosition.y);
    }

    static float GetCameraAspectRatio()
    {
        Camera3D& camera = ISGraphics::cameras3D[Camera3D::mActiveCamera];
        return camera.GetAspectRatio();
    }

    static void AttachLightComponentToEntity(int entity,float HueX,float HueY,float HueZ, float Intensity, float size) {
        InsightEngine::Instance().AddComponentAndUpdateSignature<Light>(entity, Light());
        auto& component = InsightEngine::Instance().GetComponent<Light>(entity);
        component.mHue = {HueX,HueY,HueZ};
        component.mIntensity = Intensity;
        component.mSize = size;
    }    
    
    static void AttachLightComponentToEntityWithOffset(int entity, float HueX, float HueY, float HueZ, float Intensity, float size, SimpleVector2D offset) {
        InsightEngine::Instance().AddComponentAndUpdateSignature<Light>(entity, Light());
        auto& component = InsightEngine::Instance().GetComponent<Light>(entity);
        component.mHue = { HueX,HueY,HueZ };
        component.mIntensity = Intensity;
        component.mSize = size;
        component.mOffset.x = offset.x;
        component.mOffset.y = offset.y;
    }

    static void SetLightComponentToEntity(int entity,float HueX,float HueY,float HueZ, float Intensity, float size) {
        if (InsightEngine::Instance().HasComponent<Light>(entity)) {
            auto& component = InsightEngine::Instance().GetComponent<Light>(entity);
            component.mHue = { HueX,HueY,HueZ };
            component.mIntensity = Intensity;
            component.mSize = size;
        }
    }    
    
    static void SetLightIntensityEntity(int entity, float Intensity){
        if (InsightEngine::Instance().HasComponent<Light>(entity)) {
            auto& component = InsightEngine::Instance().GetComponent<Light>(entity);
            component.mIntensity = Intensity;
        }
    }

    static float GetLightIntensityEntity(int entity)
    {
        if (auto& engine = InsightEngine::Instance(); engine.HasComponent<Light>(entity))
        {
            auto& light = engine.GetComponent<Light>(entity);
            return light.mIntensity;
        }
        return 0.f;
    }

    static void SetLightToggleEntity(int entity, bool toggle){
        if (InsightEngine::Instance().HasComponent<Light>(entity)) {
            auto& component = InsightEngine::Instance().GetComponent<Light>(entity);
            component.mRender = toggle;
        }
    }

    static void SetLightSizeEntity(int entity, float size)
    {
        if (auto& engine = InsightEngine::Instance(); engine.HasComponent<Light>(entity))
        {
            auto& light = engine.GetComponent<Light>(entity);
            light.mSize = size;
        }
    }

    static float GetLightSizeEntity(int entity)
    {
        if (auto& engine = InsightEngine::Instance(); engine.HasComponent<Light>(entity))
        {
            auto& light = engine.GetComponent<Light>(entity);
            return light.mSize;
        }
        return 0.f;
    }

    static bool IsLightRenderingEntity(int entity)
    {
        if (auto& engine = InsightEngine::Instance(); engine.HasComponent<Light>(entity))
        {
            auto& light = engine.GetComponent<Light>(entity);
            return light.mRender;
        }
        return false;
    }

    static void SetLightHueEntity(int entity, float r, float g, float b)
    {
        if (auto& engine = InsightEngine::Instance(); engine.HasComponent<Light>(entity))
        {
            auto& light = engine.GetComponent<Light>(entity);
            light.mHue.x = r;
            light.mHue.y = g;
            light.mHue.z = b;
        }
    }

    static Vector3D GetLightHueEntity(int entity)
    {
        if (auto& engine = InsightEngine::Instance(); engine.HasComponent<Light>(entity))
        {
            auto& light = engine.GetComponent<Light>(entity);
            return light.mHue;
        }
        return Vector3D();
    }

    static void FollowMouseCursorEntity(int entity)
    {
        if (auto& engine = InsightEngine::Instance(); engine.HasComponent<Transform>(entity))
        {
            auto& transform = engine.GetComponent<Transform>(entity);
            transform.world_position.x = static_cast<float>(Transform::GetMousePosition().first);
            transform.world_position.y = static_cast<float>(Transform::GetMousePosition().second);
        }
    }

    static int CreateEntityPrefab(MonoString* prefab_name)
    {
        char* c_str = mono_string_to_utf8(prefab_name);
        auto& engine = InsightEngine::Instance();
        auto const asset = engine.GetSystem<AssetManager>("Asset");
        Entity entity = engine.CreateEntity("Prefab");
        entity = engine.LoadFromPrefab(asset->GetPrefab(c_str), entity);
        mono_free(c_str);
        return entity;
    }

    static float GetRandomFloat()
    {
        return PRNG::Instance().generate();
    }

    static float MathAtan(float val) {
        return atan(val);
    }    
    static float MathCos(float val) {
        return cos(val);
    }    
    static float MathSin(float val) {
        return sin(val);
    }    
    static float MathAtan2(float x,float y) {
        return atan2(x,y);
    }

    static float MathSqrt(float val) {
        return sqrt(val);
    }

    static void GlitchEnable(bool set) {

        ISGraphics::mGlitched = set;
    }

    static void SetLightsToggle(bool toggle)
    {
        ISGraphics::mLightsOn = toggle;
    }

    static bool CompareCategory(MonoString* string) {
        auto& engine = InsightEngine::Instance();
        char* c_str = mono_string_to_utf8(string); // Convert Mono string to char*
        std::string tag(c_str);
        mono_free(c_str);
        auto& collider = engine.GetComponent<Collider>(engine.GetScriptCaller());
        for (Entity entity : collider.mCollidingEntity) {
            auto& category = engine.GetComponent<Category>(entity);
            if (category.mCategory == tag) {
                return true;
            }
        }
        return false;
    }

    static bool CompareEntityCategory(int entity_id, MonoString* string) {
        auto& engine = InsightEngine::Instance();
        char* c_str = mono_string_to_utf8(string); // Convert Mono string to char*
        std::string tag(c_str);
        mono_free(c_str);
        auto& collider = engine.GetComponent<Collider>(entity_id);
        for (Entity entity : collider.mCollidingEntity) {
            auto& category = engine.GetComponent<Category>(entity);
            if (category.mCategory == tag) {
                return true;
            }
        }
        return false;
    }

    static bool CheckEntityCategory(int entity_id, MonoString* category_to_check) {
        auto& engine = InsightEngine::Instance();
        char* c_str = mono_string_to_utf8(category_to_check); // Convert Mono string to char*
        std::string tag(c_str);
        mono_free(c_str);
        auto& category = engine.GetComponent<Category>(entity_id);
        if(category.mCategory==tag){
            return true;
        }
        return false;
    }

    static bool OnCollisionEnter() {
        auto& engine = InsightEngine::Instance();
        auto& collider = engine.GetComponent<Collider>(engine.GetScriptCaller());
        return collider.mIsColliding;
    }

    static bool OnEntityCollisionEnter(int entity_id) {
        auto& engine = InsightEngine::Instance();
        auto& collider = engine.GetComponent<Collider>(entity_id);
        return collider.mIsColliding;
    }

    static float GetGravityScale() {
        auto& engine = InsightEngine::Instance();
        auto& body = engine.GetComponent<RigidBody>(engine.GetScriptCaller());
        return body.mGravityScale;
    }

    static void SetGravityScale(float scale) {
        auto& engine = InsightEngine::Instance();
        auto& body = engine.GetComponent<RigidBody>(engine.GetScriptCaller());
        body.mGravityScale = scale;
    }

    static void SendCondition(bool bo) {
        RecievedScriptCondition = bo;
    }

    static void ChangeLightType(int lighttype) {
        ShaderEffect::currentShaderEffect = static_cast<ShaderEffect::shaderEffect>(lighttype);
    }


    // layers

    static int GetTopLayer() {
        return static_cast<int>(ISGraphics::mLayers.size());
    }

    static void SetLayerVisible(int layer, bool boolean) {
        ISGraphics::mLayers[layer].mLayerActive=boolean;
    }

    static void FadeOutAudio(float fade_time) {
        auto sys = InsightEngine::Instance().GetSystem<ISAudio>("Audio");
        sys->FadeOutAudio( fade_time);
    }


    /**
     * \brief Registers C++ functions to be accessible from C# scripts.
     */
    void ScriptGlue::RegisterFunctions()
    {
        IS_ADD_INTERNAL_CALL(ChangeLightType);

        IS_ADD_INTERNAL_CALL(NativeLog);
        IS_ADD_INTERNAL_CALL(NativeLogVector);

        //input functions
        IS_ADD_INTERNAL_CALL(KeyPressed);
        IS_ADD_INTERNAL_CALL(KeyHeld);
        IS_ADD_INTERNAL_CALL(KeyReleased);
        IS_ADD_INTERNAL_CALL(MousePressed);
        IS_ADD_INTERNAL_CALL(MouseHeld);
        IS_ADD_INTERNAL_CALL(MouseReleased);
        IS_ADD_INTERNAL_CALL(GetMousePosition);

        // Physics 
        IS_ADD_INTERNAL_CALL(RigidBodyAddForce);
        IS_ADD_INTERNAL_CALL(RigidBodySetForce);
        IS_ADD_INTERNAL_CALL(RigidBodySetForceX);
        IS_ADD_INTERNAL_CALL(RigidBodySetVelocityEntity);
        IS_ADD_INTERNAL_CALL(RigidBodyAddForceEntity);
        IS_ADD_INTERNAL_CALL(GetRigidBodyAngularVelocity);
        IS_ADD_INTERNAL_CALL(RigidBodyGetVelocity);
        IS_ADD_INTERNAL_CALL(RigidBodyGetVelocityX);
        IS_ADD_INTERNAL_CALL(RigidBodyGetVelocityY);
        IS_ADD_INTERNAL_CALL(RigidBodySetBodyTypeEntity);
        IS_ADD_INTERNAL_CALL(RigidBodyGetBodyTypeEntity);

        // Transform
        IS_ADD_INTERNAL_CALL(TransformSetPosition);
        IS_ADD_INTERNAL_CALL(TransformSetPositionEntity);
        IS_ADD_INTERNAL_CALL(TransformSetRotation);
        IS_ADD_INTERNAL_CALL(TransformSetRotationEntity);
        IS_ADD_INTERNAL_CALL(TransformSetScale);
        IS_ADD_INTERNAL_CALL(TransformSetScaleEntity);
        IS_ADD_INTERNAL_CALL(GetTransformPosition);
        IS_ADD_INTERNAL_CALL(GetTransformPositionEntity);
        IS_ADD_INTERNAL_CALL(GetTransformScaling);
        IS_ADD_INTERNAL_CALL(GetTransformScalingEntity);
        IS_ADD_INTERNAL_CALL(GetTransformRotation);
        IS_ADD_INTERNAL_CALL(GetTransformRotationEntity);
        IS_ADD_INTERNAL_CALL(GetDeltaTime);
        IS_ADD_INTERNAL_CALL(GetFixedDeltaTime);

        // Images and Sprite
        IS_ADD_INTERNAL_CALL(SetSpriteImage);
        IS_ADD_INTERNAL_CALL(SetSpriteImageEntity);
        IS_ADD_INTERNAL_CALL(GetSpriteImage);
        IS_ADD_INTERNAL_CALL(EmplaceImageToGraphics);
        IS_ADD_INTERNAL_CALL(SetSpriteAnimationIndex);
        IS_ADD_INTERNAL_CALL(SetSpriteAlphaEntity);
        IS_ADD_INTERNAL_CALL(GetSpriteAlphaEntity);
        IS_ADD_INTERNAL_CALL(FreeSpriteImage);
        IS_ADD_INTERNAL_CALL(CreateAnimationFromSprite);
        IS_ADD_INTERNAL_CALL(CreateAnimationFromSpriteEntity);
        IS_ADD_INTERNAL_CALL(ResetAnimations);
        IS_ADD_INTERNAL_CALL(ResetSpriteAnimationFrameEntity);
        IS_ADD_INTERNAL_CALL(GetCurrentAnimationEntity);
        IS_ADD_INTERNAL_CALL(SetAnimationAlpha);

        // Camera
        IS_ADD_INTERNAL_CALL(AttachCamera);
        IS_ADD_INTERNAL_CALL(CameraSetZoom);
        IS_ADD_INTERNAL_CALL(GetCameraPos);
        IS_ADD_INTERNAL_CALL(GetCameraAspectRatio);
        IS_ADD_INTERNAL_CALL(CameraGetZoom);

        // Audio
        IS_ADD_INTERNAL_CALL(AudioPlaySound);
        IS_ADD_INTERNAL_CALL(AudioPlayMusic);
        IS_ADD_INTERNAL_CALL(AudioEmitterEnableEntity);
        IS_ADD_INTERNAL_CALL(AudioStopAllSounds);
        IS_ADD_INTERNAL_CALL(AudioSetMaster);
        IS_ADD_INTERNAL_CALL(FadeOutAudio);

        // Button
        IS_ADD_INTERNAL_CALL(GetButtonState);
        IS_ADD_INTERNAL_CALL(ButtonComponentAdd);
        IS_ADD_INTERNAL_CALL(ButtonComponentRemove);
        IS_ADD_INTERNAL_CALL(ButtonComponentExists);

        //Entity Manipulations
        IS_ADD_INTERNAL_CALL(CreateEntity);
        IS_ADD_INTERNAL_CALL(CreateEntitySprite);
        IS_ADD_INTERNAL_CALL(CreateEntityVFX);
        IS_ADD_INTERNAL_CALL(DestroyEntity);
        IS_ADD_INTERNAL_CALL(AddCollider);
        IS_ADD_INTERNAL_CALL(CloneEntity);

        // Entity Collisions
        IS_ADD_INTERNAL_CALL(EntityCheckCollide);
        IS_ADD_INTERNAL_CALL(ColliderNone);
        IS_ADD_INTERNAL_CALL(ColliderComponentAdd);
        IS_ADD_INTERNAL_CALL(ColliderComponentRemove);
        IS_ADD_INTERNAL_CALL(GetCurrentEntityID);
        IS_ADD_INTERNAL_CALL(GetCollidedObjectAngle);
        IS_ADD_INTERNAL_CALL(GetCollidingEntity);
        IS_ADD_INTERNAL_CALL(GetCollidingEntityCheck);
        IS_ADD_INTERNAL_CALL(CollidingObjectIsStatic);
        IS_ADD_INTERNAL_CALL(CollidingObjectIsSpikes);
        IS_ADD_INTERNAL_CALL(CollidingObjectTypeIsWall);
        IS_ADD_INTERNAL_CALL(CollidingObjectTypeIsGhost);
        IS_ADD_INTERNAL_CALL(CollidingObjectTypeIsIgnore);

        //LIght
        IS_ADD_INTERNAL_CALL(AttachLightComponentToEntity);
        IS_ADD_INTERNAL_CALL(AttachLightComponentToEntityWithOffset);
        IS_ADD_INTERNAL_CALL(SetLightComponentToEntity);
        IS_ADD_INTERNAL_CALL(SetLightIntensityEntity);
        IS_ADD_INTERNAL_CALL(GetLightIntensityEntity);
        IS_ADD_INTERNAL_CALL(SetLightSizeEntity);
        IS_ADD_INTERNAL_CALL(SetLightToggleEntity);
        IS_ADD_INTERNAL_CALL(GetLightSizeEntity);
        IS_ADD_INTERNAL_CALL(FollowMouseCursorEntity);
        IS_ADD_INTERNAL_CALL(GetRandomFloat);
        IS_ADD_INTERNAL_CALL(IsLightRenderingEntity);
        IS_ADD_INTERNAL_CALL(SetLightHueEntity);
        IS_ADD_INTERNAL_CALL(GetLightHueEntity);


        // Drawing functions

        IS_ADD_INTERNAL_CALL(DrawLineBetweenPoints);
        IS_ADD_INTERNAL_CALL(DrawCircle);
        IS_ADD_INTERNAL_CALL(DrawDarkCircle);
        IS_ADD_INTERNAL_CALL(DrawImageAt);
        IS_ADD_INTERNAL_CALL(DrawImageExtraAt);
        IS_ADD_INTERNAL_CALL(DrawSquare);
        IS_ADD_INTERNAL_CALL(GlitchEnable);

        //Scene Manager
        IS_ADD_INTERNAL_CALL(LoadScene);
        IS_ADD_INTERNAL_CALL(SwitchScene);
        IS_ADD_INTERNAL_CALL(Exit);

        // Particle
        IS_ADD_INTERNAL_CALL(GameSpawnParticle);
        IS_ADD_INTERNAL_CALL(GameSpawnParticleFrames);
        IS_ADD_INTERNAL_CALL(GameSpawnParticleExtra);
        IS_ADD_INTERNAL_CALL(GameSpawnParticleExtraFrames);
        IS_ADD_INTERNAL_CALL(GameSpawnParticleExtraImage);


        // zk
        IS_ADD_INTERNAL_CALL(GamePause);
        IS_ADD_INTERNAL_CALL(CreateEntityButton);
        IS_ADD_INTERNAL_CALL(CreateEntityButtonNoText);
        IS_ADD_INTERNAL_CALL(CreateEntityUI);
        IS_ADD_INTERNAL_CALL(GetEntityButtonState);
        IS_ADD_INTERNAL_CALL(GetTitleBarHeight);
        IS_ADD_INTERNAL_CALL(ButtonRenderText);
        IS_ADD_INTERNAL_CALL(RenderText);
        IS_ADD_INTERNAL_CALL(RenderTextFont);
        IS_ADD_INTERNAL_CALL(SetButtonSizeScale);
        IS_ADD_INTERNAL_CALL(SetButtonSize);
        IS_ADD_INTERNAL_CALL(GetButtonHoverScale);
        IS_ADD_INTERNAL_CALL(GetButtonIdleScale);
        IS_ADD_INTERNAL_CALL(GetWindowWidth);
        IS_ADD_INTERNAL_CALL(GetWindowHeight);
        IS_ADD_INTERNAL_CALL(IsWindowFocused);
        IS_ADD_INTERNAL_CALL(SetLightsToggle);
        IS_ADD_INTERNAL_CALL(UpdateCategory);
        IS_ADD_INTERNAL_CALL(GetCollidingEntityList);
        IS_ADD_INTERNAL_CALL(RemoveColliderComponentEntity);


        IS_ADD_INTERNAL_CALL(CompareCategory);
        IS_ADD_INTERNAL_CALL(CompareEntityCategory);
        IS_ADD_INTERNAL_CALL(CheckEntityCategory);
        IS_ADD_INTERNAL_CALL(OnCollisionEnter);
        IS_ADD_INTERNAL_CALL(OnEntityCollisionEnter);
        IS_ADD_INTERNAL_CALL(GetGravityScale);
        IS_ADD_INTERNAL_CALL(SetGravityScale);
        IS_ADD_INTERNAL_CALL(GetCollidingEnemyEntity);


        // Yiming
        IS_ADD_INTERNAL_CALL(CreateEntityPrefab);
        IS_ADD_INTERNAL_CALL(IsFullscreen);
        IS_ADD_INTERNAL_CALL(ToggleFullscreen);
        IS_ADD_INTERNAL_CALL(ShowCursor);
        IS_ADD_INTERNAL_CALL(HideCursor);


        // Layers
        IS_ADD_INTERNAL_CALL(GetTopLayer);
        IS_ADD_INTERNAL_CALL(SetLayerVisible);


        //FSM Condition
        IS_ADD_INTERNAL_CALL(SendCondition);

        // IStrace
       // IS_ADD_INTERNAL_CALL(CallIS_Trace);

        // Math Function
        IS_ADD_INTERNAL_CALL(MathAtan);
        IS_ADD_INTERNAL_CALL(MathAtan2);
        IS_ADD_INTERNAL_CALL(MathSqrt);
        IS_ADD_INTERNAL_CALL(MathSin);
        IS_ADD_INTERNAL_CALL(MathCos);


        // Animation
        IS_ADD_INTERNAL_CALL(DrawNonEnityAnimation);

    }
}