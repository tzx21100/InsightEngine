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

        }

        virtual void Draw(float delta) override{








        }

    };


}