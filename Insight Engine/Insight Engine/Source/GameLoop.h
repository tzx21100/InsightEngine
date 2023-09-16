#pragma once
#include "CoreEngine.h"
namespace IS {
    class GameLoop :public ParentSystem {

        //imagine this is AE engine screen

        Entity myEntity; //create some entity
        Entity myEntity2; //create some entity
        Entity points;
        Entity lines;
        Entity circle;

        //singleton entities
        InsightEngine& engine = InsightEngine::Instance();
        InputManager& input = InputManager::Instance();
        AssetManager& asset = AssetManager::Instance();

        Image backgroundTest;

        virtual void Initialize() override {
            //create a image
            backgroundTest = asset.ImageLoad("Assets/placeholder_background.png");

            //creating game object and their components
            myEntity = engine.CreateEntityWithComponents<Sprite, InputAffector, Transform, RigidBody>();
            myEntity2 = engine.CreateEntityWithComponents<Sprite, InputAffector, Transform, RigidBody>();
            circle = engine.CreateEntityWithComponents<Sprite, Transform>();
            lines = engine.CreateEntityWithComponents<Sprite, Transform>();
            points = engine.CreateEntityWithComponents<Sprite, Transform>();

            auto& trans = engine.GetComponent<Transform>(myEntity);
            auto& trans2 = engine.GetComponent<Transform>(myEntity2);

            auto& transCircle = engine.GetComponent<Transform>(circle);
            auto& transLines = engine.GetComponent<Transform>(lines);
            auto& transPoints = engine.GetComponent<Transform>(points);

            auto& spriteCircle = engine.GetComponent<Sprite>(circle);
            auto& spriteLines = engine.GetComponent<Sprite>(lines);
            auto& spritePoints = engine.GetComponent<Sprite>(points);

            trans.setScaling(100, 100);
            trans.setWorldPosition(0, 0);
            trans2.setScaling(100, 100);
            trans2.setWorldPosition(-400, 0);
            
            transCircle.setWorldPosition(650.f, 300.f);
            transCircle.setScaling(200.f, 200.f);
            spriteCircle.primitive_type = GL_TRIANGLE_FAN;
            

            transLines.setWorldPosition(650.f, 300.f);
            transLines.setScaling(200.f, 200.f);
            transLines.setOrientation(0.f, -30.f);
            spriteLines.primitive_type = GL_LINES;

            transPoints.setWorldPosition(650.f, 300.f);
            spritePoints.primitive_type = GL_POINTS;
        }

        virtual void Update(float delta) override {
            float speed = 10;
            auto& trans = engine.GetComponent<Transform>(myEntity);
            //auto& trans2 = engine.GetComponent<Transform>(myEntity);
            int hori = input.IsKeyHeld(GLFW_KEY_D) - input.IsKeyHeld(GLFW_KEY_A);
            int verti = input.IsKeyHeld(GLFW_KEY_W) - input.IsKeyHeld(GLFW_KEY_S);
            trans.world_position.x += hori * speed;
            trans.world_position.y += verti * speed;
            float rotate = input.IsKeyHeld(GLFW_KEY_E) - input.IsKeyHeld(GLFW_KEY_Q);
            trans.orientation.x += rotate * speed;
            if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) {
                for (int i = 0; i < 1; i++) {
                    Entity a = engine.CreateEntityWithComponents<Sprite, Transform>();
                    auto& transl = engine.GetComponent<Transform>(a);
                    transl.setScaling(50-i, 50-i);
                    transl.setWorldPosition(input.GetMousePosition().first, input.GetMousePosition().second);
                    auto& spr = engine.GetComponent<Sprite>(a);

                    //add the image in
                    spr.texture = backgroundTest.texture_data;
                }

            }



            auto& transLines = engine.GetComponent<Transform>(lines);
            transLines.orientation.x += transLines.orientation.y * delta;
        }

        virtual void Draw(float delta) override{








        }

        std::string getName() override {
            return "Game Loop";
        }

    };


}