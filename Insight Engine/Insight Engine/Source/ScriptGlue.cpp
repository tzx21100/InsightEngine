/* Start Header **************************************************************/
/*!
\file    ScriptGlue.cpp
\author  Matthew Ng, matthewdeen.ng@digipen.edu
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
#include "Body.h"
#include "Collider.h"
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

    static SimpleVector2D GetMousePosition() {
        auto input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        return SimpleVector2D((float)input->GetMousePosition().first, (float)input->GetMousePosition().second);
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

    static SimpleVector2D RigidBodyGetVelocity() {
        auto& engine = InsightEngine::Instance();
        auto& body_component = engine.GetComponent<RigidBody>(engine.GetScriptCaller());
        return SimpleVector2D(body_component.mVelocity.x,body_component.mVelocity.y);
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
        return (float)InsightEngine::Instance().mDeltaTime;
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
        auto& camera = ISGraphics::cameras3D[Camera3D::mActiveCamera];
        camera.SetPosition(transform_component.world_position.x, transform_component.world_position.y);
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
        asset->PlayMusicByName(str);
    }

    static int GetButtonState() {
        auto& button=InsightEngine::Instance().GetComponent<ButtonComponent>(InsightEngine::Instance().GetScriptCaller());
        return button.mButtonState;
    }

    static int CreateEntity(MonoString* name) {
        char* c_str = mono_string_to_utf8(name); // Convert Mono string to char*
        std::string str(c_str);
        mono_free(c_str);
        Entity entity=InsightEngine::Instance().CreateEntity(str);
        InsightEngine::Instance().AddComponentAndUpdateSignature<Transform>(entity,Transform());
        InsightEngine::Instance().AddComponentAndUpdateSignature<Sprite>(entity,Sprite());
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

    static void ColliderNone(int entity) {
        auto& collider = InsightEngine::Instance().GetComponent<Collider>(entity);
        collider.mResponseEnable = false;
        collider.mResponseEnable = false;
    }

    static void CameraSetZoom(float value) {
        ISGraphics::cameras3D[Camera3D::mActiveCamera].SetZoomLevel(value);
    }

    static int GetCurrentEntityID() {
        return InsightEngine::Instance().GetScriptCaller();
    }

    static float GetCollidedObjectAngle(int entity) {
        auto& collider = InsightEngine::Instance().GetComponent<Collider>(entity);
        return collider.mCollidedObjectAngle;
    }

    static void DrawLineBetweenPoints(float x1, float y1, float x2, float y2) {
        Vector2D point1 = Vector2D(x1, y1);
        Vector2D point2 = Vector2D(x2, y2);
        Vector2D dist = point1 - point2;
        Sprite::drawDebugLine(point1, point1+dist, {1.f, 0.f, 0.f});
    }

    static void DrawCircle(float x1, float y1, float x2, float y2) {
        Sprite::drawDebugCircle(Vector2D(x1, y1), Vector2D(x2, y2), { 1.f,0.f,0.f });
    }

    static int GetCollidingEntity(int entity) {
        auto& collider_component = InsightEngine::Instance().GetComponent<Collider>(entity);
        return collider_component.mCollidingEntity;
    }

    static bool CollidingObjectIsStatic(int entity) {
        auto& body_component = InsightEngine::Instance().GetComponent<RigidBody>(entity);
        return body_component.mBodyType == BodyType::Static ? 1 : 0;
    }

    static void DrawImageAt(SimpleVector2D pos, float rotation, SimpleVector2D scale, SimpleImage image , int layer=1) {
        
        Sprite::draw_textured_quad(Vector2D(pos.x,pos.y), rotation, Vector2D(scale.x,scale.y), ConvertToImage(image), layer);
    
    }

    static void LoadScene(MonoString* file_name)
    {
        char* c_str = mono_string_to_utf8(file_name); // Convert Mono string to char*
        SceneManager::Instance().LoadScene(c_str);
        mono_free(c_str);
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
        IS_ADD_INTERNAL_CALL(GetMousePosition);

        // Physics 
        IS_ADD_INTERNAL_CALL(RigidBodyAddForce);
        IS_ADD_INTERNAL_CALL(RigidBodySetForce);
        IS_ADD_INTERNAL_CALL(RigidBodyAddForceEntity);
        IS_ADD_INTERNAL_CALL(GetRigidBodyAngularVelocity);
        IS_ADD_INTERNAL_CALL(RigidBodyGetVelocity);

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
        IS_ADD_INTERNAL_CALL(CameraSetZoom);

        // Audio
        IS_ADD_INTERNAL_CALL(AudioPlaySound);
        IS_ADD_INTERNAL_CALL(AudioPlayMusic);

        // Button
        IS_ADD_INTERNAL_CALL(GetButtonState);

        //Entity Manipulations
        IS_ADD_INTERNAL_CALL(CreateEntity);
        IS_ADD_INTERNAL_CALL(DestroyEntity);
        IS_ADD_INTERNAL_CALL(AddCollider);

        // Entity Collisions
        IS_ADD_INTERNAL_CALL(EntityCheckCollide);
        IS_ADD_INTERNAL_CALL(ColliderNone);
        IS_ADD_INTERNAL_CALL(GetCurrentEntityID);
        IS_ADD_INTERNAL_CALL(GetCollidedObjectAngle);
        IS_ADD_INTERNAL_CALL(GetCollidingEntity);
        IS_ADD_INTERNAL_CALL(CollidingObjectIsStatic);

        //Debug
        //Debug
        //Debug
        //Debug
        //Debug
        //Debug
        //Debu














        IS_ADD_INTERNAL_CALL(DrawLineBetweenPoints);
        IS_ADD_INTERNAL_CALL(DrawCircle);
        IS_ADD_INTERNAL_CALL(DrawImageAt);

        //Scene Manager
        IS_ADD_INTERNAL_CALL(LoadScene);
        IS_ADD_INTERNAL_CALL(SwitchScene);
        IS_ADD_INTERNAL_CALL(Exit);

        // Particle
        IS_ADD_INTERNAL_CALL(GameSpawnParticle);
        IS_ADD_INTERNAL_CALL(GameSpawnParticleExtra);



        // IStrace
       // IS_ADD_INTERNAL_CALL(CallIS_Trace);


    }
}