#pragma once
#include "CoreEngine.h"
namespace IS {
    class GameLoop :public ParentSystem {

        //imagine this is AE engine screen

        Entity myEntity; //create some entity
        Entity myEntity2; //create some entity

        //singleton entities
        InsightEngine& engine = InsightEngine::Instance();
        InputManager& input = InputManager::Instance();

        virtual void Initialize() override {

            //creating game object and their components
            myEntity = engine.CreateEntityWithComponents<Sprite, InputAffector, Transform, RigidBody>();
            myEntity2 = engine.CreateEntityWithComponents<Sprite, InputAffector, Transform, RigidBody>();
            auto& trans = engine.GetComponent<Transform>(myEntity);
            //auto& rigidbody = engine.GetComponent<RigidBody>(myEntity);
            auto& trans2 = engine.GetComponent<Transform>(myEntity2);
            trans.setScaling(100, 100);
            trans.setWorldPosition(0, 0);
            trans2.setScaling(100, 100);
            trans2.setWorldPosition(-400, 0);
            //rigidbody.updateBoxBody();

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

            /*if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) {
                for (int i = 0; i < 1; i++) {
                    Entity a = engine.CreateEntityWithComponents<Sprite, Transform, RigidBody>();
                    auto& trans = engine.GetComponent<Transform>(a);
                    trans.setScaling(50-i, 50-i);
                    trans.setWorldPosition(input.GetMousePosition().first, input.GetMousePosition().second);
                }

            }*/
            //Entity a = engine.CreateEntityWithComponents<Sprite, Transform, RigidBody>();
            //auto& trans = engine.GetComponent<Transform>(a);
            //trans.setScaling(50 , 50 );
            //trans.setWorldPosition(input.GetMousePosition().first, input.GetMousePosition().second);
            /*
            if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2)) {
                IS_CORE_TRACE("Entities Count: {}", engine.EntitiesAlive());
            }*/

        }

        virtual void Draw(float delta) override{








        }

        std::string getName() override {
            return "Game Loop";
        }

    };


}