/*!
 * \file GameLoop.h
 * \author NA Not applicable.
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 *
 * This is just a pseudo game loop to show off the functionalities of the engine.
 * Most of this will be converted to scripts once we integrate it with our ECS.
 *
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_GAMELOOP_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_GAMELOOP_H_
#include "CoreEngine.h"

namespace IS {
    class GameLoop :public ParentSystem {

        //imagine this is AE engine screen

        Entity entity_player{};
        Entity entity_test{};
        Entity entity_floor{};
        Entity entity_point{};
        Entity entity_line{};
        Entity entity_circle{};
        Entity entity_quad{};

        //singleton engine
        InsightEngine& engine = InsightEngine::Instance();
        std::shared_ptr<InputManager> input = InsightEngine::Instance().GetSystem<InputManager>("Input");
        std::shared_ptr<AssetManager> asset = InsightEngine::Instance().GetSystem<AssetManager>("Asset");

        Image backgroundTest{};
        Image black_background{};
        Image idle_animation{};
        Image walking_animation{};
        Image truck_animation{};

        // Animation objects
        /*Animation idle_ani;
        Animation walking_ani;
        Animation ice_cream_truck_ani;*/

        virtual void Initialize() override {
            //create a image
            backgroundTest = asset->GetImage("Assets/placeholder_background.png");
            black_background = asset->GetImage("Assets/black_background.png");
            idle_animation = asset->GetImage("Assets/player_idle.png");
            walking_animation = asset->GetImage("Assets/player_walking.png");
            truck_animation = asset->GetImage("Assets/icecream_truck.png");

            ISGraphics::textures.emplace(0, backgroundTest);
            ISGraphics::textures.emplace(1, black_background);
            ISGraphics::textures.emplace(2, idle_animation);
            ISGraphics::textures.emplace(3, walking_animation);
            ISGraphics::textures.emplace(4, truck_animation);

            //creating game object and their components
            entity_quad = engine.CreateEntityWithComponents<Sprite, Transform>("Background");
            entity_player = engine.CreateEntityWithComponents<Sprite, InputAffector, Transform, RigidBody>("Player");
            entity_test = engine.CreateEntityWithComponents<Sprite, InputAffector, Transform, RigidBody>("Test Texture");
            entity_floor = engine.CreateEntityWithComponents<Sprite, InputAffector, Transform, RigidBody>("Floor");
            entity_circle = engine.CreateEntityWithComponents<Sprite, Transform>("Clock Circle");
            entity_line = engine.CreateEntityWithComponents<Sprite, Transform>("Clock Line");
            entity_point = engine.CreateEntityWithComponents<Sprite, Transform>("Clock Point");

            auto& trans_quad = engine.GetComponent<Transform>(entity_quad);
            auto& trans_player = engine.GetComponent<Transform>(entity_player);
            auto& trans_test = engine.GetComponent<Transform>(entity_test);
            auto& trans_floor = engine.GetComponent<Transform>(entity_floor);
            auto& trans_circle = engine.GetComponent<Transform>(entity_circle);
            auto& trans_line = engine.GetComponent<Transform>(entity_line);
            auto& trans_point = engine.GetComponent<Transform>(entity_point);

            auto& body_player = engine.GetComponent<RigidBody>(entity_player);
            auto& body_floor = engine.GetComponent<RigidBody>(entity_floor);

            auto& sprite_background = engine.GetComponent<Sprite>(entity_quad);
            auto& sprite_player = engine.GetComponent<Sprite>(entity_player);
            auto& sprite_test = engine.GetComponent<Sprite>(entity_test);
            auto& sprite_circle = engine.GetComponent<Sprite>(entity_circle);
            auto& sprite_line = engine.GetComponent<Sprite>(entity_line);
            auto& sprite_point = engine.GetComponent<Sprite>(entity_point);

            sprite_background.texture_id = static_cast<uint8_t>(black_background.texture_id);
            sprite_background.texture_width = black_background.width;
            sprite_background.texture_height = black_background.height;

            sprite_player.name = "player_sprite";
            sprite_player.texture_id = static_cast<uint8_t>(idle_animation.texture_id);
            sprite_player.texture_width = idle_animation.width;
            sprite_player.texture_height = idle_animation.height;
            sprite_player.anim = ISGraphics::idle_ani;
            // sprite_player.anim_vect.emplace_back(ISGraphics::idle_ani);
            // sprite_player.anim_vect.emplace_back(ISGraphics::walking_ani);

            sprite_test.texture_id = static_cast<uint8_t>(backgroundTest.texture_id);
            sprite_test.texture_width = backgroundTest.width;
            sprite_test.texture_height = backgroundTest.height;

            trans_quad.setScaling(WIDTH, HEIGHT);
            trans_player.setScaling(95, 120);
            trans_player.setWorldPosition(0, 0);
            trans_test.setScaling(-100, 100);
            trans_test.setWorldPosition(-400, 0);
            trans_floor.setScaling(600, 100);
            trans_floor.setWorldPosition(0, -400);
            body_player.mAngularVelocity = 10.f;
            body_player.mRestitution = 0.1f;
            body_floor.mBodyType = BodyType::Static;
            body_floor.mMass = 99999.f;
            body_floor.mInvMass = 1.f / 99999.f;

            trans_circle.setWorldPosition(650.f, 300.f);
            trans_circle.setScaling(200.f, 200.f);
            sprite_circle.primitive_type = GL_TRIANGLE_FAN;


            trans_line.setWorldPosition(650.f, 300.f);
            trans_line.setScaling(200.f, 200.f);
            trans_line.setRotation(0.f, 40.f);
            sprite_line.primitive_type = GL_LINES;

            trans_point.setWorldPosition(650.f, 300.f);
            sprite_point.primitive_type = GL_POINTS;
        }

        virtual void Update(float delta) override {

            // Disable mouse/key event when GUI is using them
            auto const& gui = InsightEngine::Instance().GetSystem<Editor>("Editor");

            // Process Keyboard Events
            if (!gui->WantCaptureKeyboard()) {

                // Toggle fullscreen
                auto const& window = engine.GetSystem<WindowSystem>("Window");
                static bool fullscreen = window->IsFullScreen();
                if (input->IsKeyPressed(GLFW_KEY_F11)) {
                    fullscreen = !fullscreen;
                    window->SetFullScreen(fullscreen);
                    IS_CORE_DEBUG("{} mode", fullscreen ? "Fullscreen" : "Windowed");
                }

                // Enable/disable GUI
                if (input->IsKeyPressed(GLFW_KEY_TAB)) {
                    engine.mUsingGUI = !engine.mUsingGUI;
                    IS_CORE_DEBUG("GUI {}", engine.mUsingGUI ? "Enabled" : "Disabled");
                }

                // Offset mouse position
                if (!engine.mUsingGUI) {
                    auto const& [width, height] = fullscreen ? window->GetMonitorSize() : window->GetWindowSize();
                    input->setCenterPos(width / 2.f, height / 2.f);
                    input->setRatio(static_cast<float>(width), static_cast<float>(height));
                }

                //this controls the freeze frame
                engine.mContinueFrame = false;

                if (input->IsKeyPressed(GLFW_KEY_ENTER) && input->IsKeyHeld(GLFW_KEY_LEFT_SHIFT)) {
                    engine.mFreezeFrame = !engine.mFreezeFrame;
                    IS_CORE_DEBUG("Freeze frame {}!", engine.mFreezeFrame ? "enabled" : "disabled");
                }
                if (engine.mFreezeFrame) {
                    if (input->IsKeyPressed(GLFW_KEY_ENTER)) {
                        engine.mContinueFrame = true;
                        IS_CORE_DEBUG("Step frame");
                    }
                    if (!engine.mContinueFrame)
                        return;
                }

                if (engine.HasComponent<Sprite>(entity_player)) {
                    auto& sprite_player = engine.GetComponent<Sprite>(entity_player);
                    if (input->IsKeyHeld(GLFW_KEY_W) || input->IsKeyHeld(GLFW_KEY_A) || input->IsKeyHeld(GLFW_KEY_S) || input->IsKeyHeld(GLFW_KEY_D)) {
                        sprite_player.texture_id = static_cast<uint8_t>(walking_animation.texture_id);
                        sprite_player.texture_width = walking_animation.width;
                        sprite_player.texture_height = walking_animation.height;
                        sprite_player.animation_index = 1;
                    }
                    else {
                        sprite_player.texture_id = static_cast<uint8_t>(idle_animation.texture_id);
                        sprite_player.texture_width = idle_animation.width;
                        sprite_player.texture_height = idle_animation.height;
                        sprite_player.animation_index = 0;
                    }
                }

                if (engine.HasComponent<Transform>(entity_player)) {
                    auto& trans_player = engine.GetComponent<Transform>(entity_player);
                    auto& body_player = engine.GetComponent<RigidBody>(entity_player);

                    //auto& trans2 = engine.GetComponent<Transform>(myEntity);
                    int hori = input->IsKeyHeld(GLFW_KEY_D) - input->IsKeyHeld(GLFW_KEY_A);
                    int verti = input->IsKeyHeld(GLFW_KEY_W) - input->IsKeyHeld(GLFW_KEY_S);
                    Vector2D inputVelocity = Vector2D(hori * 10.f, verti * 10.f);
                    body_player.AddVelocity(inputVelocity);
                    // for jumping
                    if (input->IsKeyPressed(GLFW_KEY_SPACE)) {
                        //IS_CORE_INFO("{}", static_cast<short>(BodyState::GROUNDED));
                        if (body_player.mState == BodyState::GROUNDED && body_player.mVelocity.y <= 10.f) {
                            body_player.AddVelocity(Vector2D(0.f, 500.f));
                            body_player.mState = BodyState::JUMP;
                        }
                    }

                    // flip image
                    trans_player.scaling.x *= (input->IsKeyHeld(GLFW_KEY_A) && (trans_player.scaling.x > 0)) ||
                        (input->IsKeyHeld(GLFW_KEY_D) && (trans_player.scaling.x < 0)) ? -1 : 1;

                    float rotate = static_cast<float>(input->IsKeyHeld(GLFW_KEY_E) - input->IsKeyHeld(GLFW_KEY_Q));
                    trans_player.rotation += rotate * body_player.mAngularVelocity;
                    trans_player.rotation = trans_player.rotation < 0.f ? 360.f : fmod(trans_player.rotation, 360.f);
                }

                // rotate lines on clock
                if (engine.HasComponent<Transform>(entity_line)) {
                    auto& trans_lines = engine.GetComponent<Transform>(entity_line);
                    trans_lines.rotation += trans_lines.angle_speed * delta;
                }

                if (input->IsKeyPressed(GLFW_KEY_Z)) {
                    asset->PlaySoundByName("sound.MP3");
                }
                if (input->IsKeyPressed(GLFW_KEY_X)) {
                    asset->PlayMusicByName("music.wav");
                }
            }

            // Process Mouse Events
            if (!gui->WantCaptureMouse()) {
                if (input->IsMouseButtonHeld(GLFW_MOUSE_BUTTON_3)) {
                    for (int i = 0; i < 1; i++) {
                        Entity a = engine.CreateEntityWithComponents<Sprite, Transform>("Small Box");
                        auto& transl = engine.GetComponent<Transform>(a);
                        transl.setScaling(30, 38);
                        transl.setWorldPosition(static_cast<float>(input->GetMousePosition().first), static_cast<float>(input->GetMousePosition().second));
                        //add the image in
                        //spr.texture = backgroundTest.texture_id;
                    }

                }

                if (input->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2)) {
                    for (int i = 0; i < 1; i++) {
                        Entity a = engine.CreateEntityWithComponents<Sprite, Transform, RigidBody>("Ice Cream Truck");
                        auto& transl = engine.GetComponent<Transform>(a);
                        transl.setScaling(128, 128);
                        transl.setWorldPosition(static_cast<float>(input->GetMousePosition().first), static_cast<float>(input->GetMousePosition().second));
                        auto& spr = engine.GetComponent<Sprite>(a);
                        spr.name = "ice_cream_truck";
                        spr.texture_id = static_cast<uint8_t>(truck_animation.texture_id);
                        spr.texture_width = truck_animation.width;
                        spr.texture_height = truck_animation.height;
                        //spr.anim_vect.emplace_back(ISGraphics::ice_cream_truck_ani);
                        spr.anim = ISGraphics::ice_cream_truck_ani;
                        
                        //add the image in
                        //spr.texture = backgroundTest.texture_id;

                    }


                }
            }

            // rotate lines on clock
            if (engine.HasComponent<Transform>(entity_line)) {
                auto& transLines = engine.GetComponent<Transform>(entity_line);
                transLines.rotation += transLines.angle_speed * delta;
            }
        }

        virtual void Draw([[maybe_unused]] float delta) override {








        }

        std::string GetName() override {
            return "Game Loop";
        }

    };


}

#endif