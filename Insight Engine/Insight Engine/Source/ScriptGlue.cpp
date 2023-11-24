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
#include <algorithm>
#include "Physics.h"
#include <mono/metadata/object.h>
#include <math.h>

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
        IS_DEBUG("{}, {}", str, param);
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
    
    static void SetSpriteImageEntity(SimpleImage image,int entity) {
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(entity);
        Image a = ConvertToImage(image);
        sprite_component.img = a;
    }

    static void SetSpriteAlphaEntity(float alpha, int entity) {
        auto& sprite_component = InsightEngine::Instance().GetComponent<Sprite>(entity);
        sprite_component.color.a = alpha;
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
    
    static void AttachCamera(float xoffset, float yoffset) {
        auto& camera = ISGraphics::cameras3D[Camera3D::mActiveCamera];
        camera.SetPosition(xoffset, yoffset);
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
        sprite_component.layer = UI_LAYER;
        sprite_component.img = ConvertToImage(image);

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
        sprite_component.layer = UI_LAYER;
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
        Vector2D dist = point1 - point2;
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
        return (body_component.mBodyType == BodyType::Ignore) ? true : false;
    }

    static void DrawImageAt(SimpleVector2D pos, float rotation, SimpleVector2D scale, SimpleImage image, float alpha, int layer=1) {
        
        Sprite::draw_textured_quad(Vector2D(pos.x,pos.y), rotation, Vector2D(scale.x,scale.y), ConvertToImage(image), alpha, layer);
    
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

    static void ButtonRenderText(int entity, float x, float y, float size, Vector3D color)
    {
        auto& button_component = InsightEngine::Instance().GetComponent<ButtonComponent>(entity);
        Text::addTextRenderCall(button_component.mButtonText, x, y, size * button_component.mSizeScale, { color.x, color.y, color.z });
    }

    static void SetButtonSizeScale(int entity, float scale)
    {
        auto& button_component = InsightEngine::Instance().GetComponent<ButtonComponent>(entity);
        button_component.mSizeScale = scale;
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

    static void RenderText(MonoString* text, float x, float y, float size, Vector3D color)
    {
        char* c_str = mono_string_to_utf8(text); // Convert Mono string to char*
        std::string part_name(c_str);
        mono_free(c_str);
        Text::addTextRenderCall(c_str, x, y, size, { color.x, color.y, color.z });
    }

    static int GetWindowWidth()
    {
        InsightEngine& engine = InsightEngine::Instance();
        return engine.IsFullScreen() ? engine.GetMonitorWidth() : engine.GetWindowWidth();
    }

    static int GetWindowHeight()
    {
        InsightEngine& engine = InsightEngine::Instance();
        return engine.IsFullScreen() ? engine.GetMonitorHeight() : engine.GetWindowHeight();
    }

    static SimpleVector2D GetCameraPos() {
        return SimpleVector2D(ISGraphics::cameras3D[Camera3D::mActiveCamera].mPosition.x, ISGraphics::cameras3D[Camera3D::mActiveCamera].mPosition.y);
    }

    static void AttachLightComponentToEntity(int entity,float HueX,float HueY,float HueZ, float Intensity, float size) {
        InsightEngine::Instance().AddComponentAndUpdateSignature<Light>(entity, Light());
        auto& component = InsightEngine::Instance().GetComponent<Light>(entity);
        component.mHue = {HueX,HueY,HueZ};
        component.mIntensity = Intensity;
        component.mSize = size;
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
    static void SetLightToggleEntity(int entity, bool toggle){
        if (InsightEngine::Instance().HasComponent<Light>(entity)) {
            auto& component = InsightEngine::Instance().GetComponent<Light>(entity);
            component.mRender = toggle;
        }
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
        IS_ADD_INTERNAL_CALL(RigidBodySetVelocityEntity);
        IS_ADD_INTERNAL_CALL(RigidBodyAddForceEntity);
        IS_ADD_INTERNAL_CALL(GetRigidBodyAngularVelocity);
        IS_ADD_INTERNAL_CALL(RigidBodyGetVelocity);
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

        // Images and Sprite
        IS_ADD_INTERNAL_CALL(SetSpriteImage);
        IS_ADD_INTERNAL_CALL(SetSpriteImageEntity);
        IS_ADD_INTERNAL_CALL(GetSpriteImage);
        IS_ADD_INTERNAL_CALL(EmplaceImageToGraphics);
        IS_ADD_INTERNAL_CALL(SetSpriteAnimationIndex);
        IS_ADD_INTERNAL_CALL(SetSpriteAlphaEntity);
        IS_ADD_INTERNAL_CALL(FreeSpriteImage);
        IS_ADD_INTERNAL_CALL(CreateAnimationFromSprite);
        IS_ADD_INTERNAL_CALL(CreateAnimationFromSpriteEntity);
        IS_ADD_INTERNAL_CALL(ResetAnimations);
        IS_ADD_INTERNAL_CALL(ResetSpriteAnimationFrameEntity);
        IS_ADD_INTERNAL_CALL(GetCurrentAnimationEntity);

        // Camera
        IS_ADD_INTERNAL_CALL(AttachCamera);
        IS_ADD_INTERNAL_CALL(CameraSetZoom);
        IS_ADD_INTERNAL_CALL(GetCameraPos);

        // Audio
        IS_ADD_INTERNAL_CALL(AudioPlaySound);
        IS_ADD_INTERNAL_CALL(AudioPlayMusic);

        // Button
        IS_ADD_INTERNAL_CALL(GetButtonState);

        //Entity Manipulations
        IS_ADD_INTERNAL_CALL(CreateEntity);
        IS_ADD_INTERNAL_CALL(CreateEntitySprite);
        IS_ADD_INTERNAL_CALL(CreateEntityVFX);
        IS_ADD_INTERNAL_CALL(DestroyEntity);
        IS_ADD_INTERNAL_CALL(AddCollider);

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

        //Debug
        //Debug
        //Debug
        //Debug
        //Debug
        //Debug
        //Debu






        //LIght
        IS_ADD_INTERNAL_CALL(AttachLightComponentToEntity);
        IS_ADD_INTERNAL_CALL(SetLightComponentToEntity);
        IS_ADD_INTERNAL_CALL(SetLightIntensityEntity);
        IS_ADD_INTERNAL_CALL(SetLightToggleEntity);







        IS_ADD_INTERNAL_CALL(DrawLineBetweenPoints);
        IS_ADD_INTERNAL_CALL(DrawCircle);
        IS_ADD_INTERNAL_CALL(DrawDarkCircle);
        IS_ADD_INTERNAL_CALL(DrawImageAt);
        IS_ADD_INTERNAL_CALL(DrawSquare);
        IS_ADD_INTERNAL_CALL(GlitchEnable);

        //Scene Manager
        IS_ADD_INTERNAL_CALL(LoadScene);
        IS_ADD_INTERNAL_CALL(SwitchScene);
        IS_ADD_INTERNAL_CALL(Exit);

        // Particle
        IS_ADD_INTERNAL_CALL(GameSpawnParticle);
        IS_ADD_INTERNAL_CALL(GameSpawnParticleExtra);
        IS_ADD_INTERNAL_CALL(GameSpawnParticleExtraImage);


        // zk
        IS_ADD_INTERNAL_CALL(GamePause);
        IS_ADD_INTERNAL_CALL(CreateEntityButton);
        IS_ADD_INTERNAL_CALL(CreateEntityUI);
        IS_ADD_INTERNAL_CALL(GetEntityButtonState);
        IS_ADD_INTERNAL_CALL(GetTitleBarHeight);
        IS_ADD_INTERNAL_CALL(ButtonRenderText);
        IS_ADD_INTERNAL_CALL(RenderText);
        IS_ADD_INTERNAL_CALL(SetButtonSizeScale);
        IS_ADD_INTERNAL_CALL(GetButtonHoverScale);
        IS_ADD_INTERNAL_CALL(GetButtonIdleScale);
        IS_ADD_INTERNAL_CALL(GetWindowWidth);
        IS_ADD_INTERNAL_CALL(GetWindowHeight);


        // IStrace
       // IS_ADD_INTERNAL_CALL(CallIS_Trace);

        // Math Function
        IS_ADD_INTERNAL_CALL(MathAtan);
        IS_ADD_INTERNAL_CALL(MathAtan2);
        IS_ADD_INTERNAL_CALL(MathSqrt);
        IS_ADD_INTERNAL_CALL(MathSin);
        IS_ADD_INTERNAL_CALL(MathCos);


    }
}