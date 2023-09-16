#pragma once
#include "CoreEngine.h"
namespace IS {
    class GameLoop :public ParentSystem {

        //imagine this is AE engine screen

        Entity myEntity; //create some entity

        //singleton entities
        InsightEngine& engine = InsightEngine::Instance();
        InputManager& input = InputManager::Instance();

        virtual void Initialize() override {

            //creating game object and their components
            myEntity = engine.CreateEntityWithComponents<Sprite, InputAffector, Transform>();
            auto& trans = engine.GetComponent<Transform>(myEntity);
            trans.setScaling(100, 100);
            trans.setWorldPosition(0, 0);

        }

        virtual void Update(float delta) override {
            float speed = 10;
            auto& trans = engine.GetComponent<Transform>(myEntity);
            int hori = input.IsKeyHeld(GLFW_KEY_D) - input.IsKeyHeld(GLFW_KEY_A);
            int verti = input.IsKeyHeld(GLFW_KEY_W) - input.IsKeyHeld(GLFW_KEY_S);
            trans.world_position.x += hori * speed;
            trans.world_position.y += verti * speed;

            /*if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) {
                for (int i = 0; i < 500; i++) {
                    Entity a = engine.CreateEntityWithComponents<Sprite, Transform>();
                    auto& trans = engine.GetComponent<Transform>(a);
                    trans.setScaling(120-i, 120-i);
                    trans.setWorldPosition(input.GetMousePosition().first/2, input.GetMousePosition().second/2);
                }
            }

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