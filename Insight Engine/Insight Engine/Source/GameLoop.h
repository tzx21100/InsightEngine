#pragma once
#include "CoreEngine.h"
namespace IS {
    class GameLoop :public ParentSystem {

        //imagine this is AE engine screen

        Entity myEntity; //create some entity
        Entity myEntity2; //create some entity
        Entity myEntity3; //create some entity
        Entity points;
        Entity lines;
        Entity circle;
        Entity quad;

        //singleton entities
        InsightEngine& engine = InsightEngine::Instance();
        InputManager& input = InputManager::Instance();
        AssetManager& asset = AssetManager::Instance();

        Image backgroundTest;
        Image idle_animation;
        Image zx_animation;
        Image greybackgroundTest = asset.ToSepia(backgroundTest);

        virtual void Initialize() override {
            //create a image
            backgroundTest = asset.ImageLoad("Assets/placeholder_background.png");
            idle_animation = asset.ImageLoad("Assets/player_idle.png");
            zx_animation = asset.ImageLoad("Assets/icecream_truck.png");

            
            //creating game object and their components
            myEntity = engine.CreateEntityWithComponents<Sprite, InputAffector, Transform, RigidBody>();
            myEntity2 = engine.CreateEntityWithComponents<Sprite, InputAffector, Transform, RigidBody>();
            myEntity3 = engine.CreateEntityWithComponents<Sprite, InputAffector, Transform, RigidBody>();
            circle = engine.CreateEntityWithComponents<Sprite, Transform>();
            lines = engine.CreateEntityWithComponents<Sprite, Transform, RigidBody>();
            points = engine.CreateEntityWithComponents<Sprite, Transform>();

            auto& trans = engine.GetComponent<Transform>(myEntity);
            auto& trans2 = engine.GetComponent<Transform>(myEntity2);
            auto& trans3 = engine.GetComponent<Transform>(myEntity3);
            auto& body = engine.GetComponent<RigidBody>(myEntity);
            auto& body3 = engine.GetComponent<RigidBody>(myEntity3);
            auto& body4 = engine.GetComponent<RigidBody>(lines);

            auto& transCircle = engine.GetComponent<Transform>(circle);
            auto& transLines = engine.GetComponent<Transform>(lines);
            auto& transPoints = engine.GetComponent<Transform>(points);

            auto& spriteMyEntity = engine.GetComponent<Sprite>(myEntity);
            auto& spriteMyEntity2 = engine.GetComponent<Sprite>(myEntity2);
            auto& spriteCircle = engine.GetComponent<Sprite>(circle);
            auto& spriteLines = engine.GetComponent<Sprite>(lines);
            auto& spritePoints = engine.GetComponent<Sprite>(points);

            spriteMyEntity.name = "textured_box";
            spriteMyEntity.texture = static_cast<uint8_t>(idle_animation.texture_data);
            spriteMyEntity.texture_width = idle_animation.width;
            spriteMyEntity.texture_height = idle_animation.height;

            spriteMyEntity2.texture = static_cast<uint8_t>(backgroundTest.texture_data);
            spriteMyEntity2.texture_width = backgroundTest.width;
            spriteMyEntity2.texture_height = backgroundTest.height;

            trans.setScaling(95, 120);
            trans.setWorldPosition(0, 0);
            trans2.setScaling(100, 100);
            trans2.setWorldPosition(-400, 0);
            trans3.setScaling(600, 100);
            trans3.setWorldPosition(0, -400);
            body.velocity = { 10.f, 10.f };
            body.angular_velocity = 10.f;
            body3.bodyType = BodyType::Static;
            body4.bodyType = BodyType::Static;
            body4.angular_velocity = 30.f;
            
            transCircle.setWorldPosition(650.f, 300.f);
            transCircle.setScaling(200.f, 200.f);
            spriteCircle.primitive_type = GL_TRIANGLE_FAN;
            

            transLines.setWorldPosition(650.f, 300.f);
            transLines.setScaling(200.f, 200.f);
            transLines.setRotation(0.f);
            spriteLines.primitive_type = GL_LINES;

            transPoints.setWorldPosition(650.f, 300.f);
            spritePoints.primitive_type = GL_POINTS;
        }

        virtual void Update(float delta) override {
            

            if (input.IsKeyPressed(GLFW_KEY_SPACE)) {
                engine.continueFrame = true;
            }
            if (input.IsKeyPressed(GLFW_KEY_SPACE) && input.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
                engine.freezeFrame = !engine.freezeFrame;
            }
            if (engine.freezeFrame) {
                if(!engine.continueFrame)
                return;
            }

            if (engine.HasComponent<Transform>(myEntity)) {
                auto& trans = engine.GetComponent<Transform>(myEntity);
                auto& rbody = engine.GetComponent<RigidBody>(myEntity);

                //auto& trans2 = engine.GetComponent<Transform>(myEntity);
                int hori = input.IsKeyHeld(GLFW_KEY_D) - input.IsKeyHeld(GLFW_KEY_A);
                int verti = input.IsKeyHeld(GLFW_KEY_W) - input.IsKeyHeld(GLFW_KEY_S);
                trans.world_position.x += hori * rbody.velocity.x;
                trans.world_position.y += verti * rbody.velocity.y;

                // flip image
                trans.scaling.x *= (input.IsKeyHeld(GLFW_KEY_A) && (trans.scaling.x > 0)) ||
                    (input.IsKeyHeld(GLFW_KEY_D) && (trans.scaling.x < 0)) ? -1 : 1;


                float rotate = static_cast<float>(input.IsKeyHeld(GLFW_KEY_E) - input.IsKeyHeld(GLFW_KEY_Q));
                trans.rotation += rotate * rbody.angular_velocity;
                trans.rotation = trans.rotation < 0.f ? 360.f : fmod(trans.rotation, 360.f);
            }
            if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) && input.GetMousePosition().first > -WIDTH / 2 && input.GetMousePosition().first < WIDTH / 2) {
                for (int i = 0; i < 1; i++) {
                    Entity a = engine.CreateEntityWithComponents<Sprite, Transform>();
                    auto& transl = engine.GetComponent<Transform>(a);
                    transl.setScaling(30, 38);
                    transl.setWorldPosition(static_cast<float>(input.GetMousePosition().first), static_cast<float>(input.GetMousePosition().second));
                    //add the image in
                    //spr.texture = backgroundTest.texture_data;
                }

            }

            if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2) && input.GetMousePosition().first>-WIDTH/2 && input.GetMousePosition().second<WIDTH/2) {
                for (int i = 0; i < 1; i++) {
                    Entity a = engine.CreateEntityWithComponents<Sprite, Transform, RigidBody>();
                    auto& transl = engine.GetComponent<Transform>(a);
                    transl.setScaling(128, 128);
                    transl.setWorldPosition(static_cast<float>(input.GetMousePosition().first), static_cast<float>(input.GetMousePosition().second));
                    auto& spr = engine.GetComponent<Sprite>(a);
                    spr.name = "textured_box2";  
                    spr.texture = static_cast<uint8_t>(zx_animation.texture_data);
                    spr.texture_width = zx_animation.width;
                    spr.texture_height = zx_animation.height;
                    //add the image in
                    //spr.texture = backgroundTest.texture_data;
                }

            }

            if (input.IsKeyPressed(GLFW_KEY_R)) {
                engine.DestroyEntity(myEntity);
            }

            auto& transLines = engine.GetComponent<Transform>(lines);
            auto& rbLines = engine.GetComponent<RigidBody>(lines);
            transLines.rotation += rbLines.angular_velocity * delta;
            transLines.rotation = transLines.rotation < 0.f ? 360.f : fmod(transLines.rotation, 360.f);
        }

        virtual void Draw([[maybe_unused]]float delta) override{








        }

        std::string getName() override {
            return "Game Loop";
        }

    };


}