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
#include "GameGui.h"
#include "Body.h"
#include "Collider.h"

namespace IS {
    class GameLoop :public ParentSystem {

        //imagine this is AE engine screen

        //Entity entity_player{};
        //Entity entity_test{};
        //Entity entity_floor{};
        //Entity entity_point{};
        //Entity entity_line{};
        //Entity entity_circle{};
        //Entity entity_quad{};
        //Entity entity_button{};
        Entity entity_ai{};

        //singleton engine
        InsightEngine& engine = InsightEngine::Instance();
        std::shared_ptr<InputManager> input = engine.GetSystem<InputManager>("Input");
        std::shared_ptr<AssetManager> asset = engine.GetSystem<AssetManager>("Asset");

        Image* backgroundTest{};
        Image* black_background{};
        Image* idle_anim_image{};
        Image* walking_anim_image{};
        Image* truck_anim_image{};

        //// Animation objects
        Animation idle_ani;
        Animation walking_ani;
        Animation ice_cream_truck_ani;

        // Camera::int mActiveCamera;
        
        virtual void Initialize() override {
            
            

            //create a image
            //backgroundTest = asset->GetImage("Assets/placeholder_background.png");
            //backgroundTest->texture_index = 0; // hard code first :)
            //black_background = asset->GetImage("Assets/black_background.png");
            //black_background->texture_index = 1;
            //idle_anim_image = asset->GetImage("Assets/player_idle.png");
            //idle_anim_image->texture_index = 2;
            //walking_anim_image = asset->GetImage("Assets/player_walking.png");
            //walking_anim_image->texture_index = 3;
            truck_anim_image = asset->GetImage("icecream_truck.png");
            //truck_anim_image->texture_index = 0;

            //walking_ani.initAnimation(1, 4, 1.f); // init 3 animations
            //idle_ani.initAnimation(1, 8, 3.f);
            ice_cream_truck_ani.initAnimation(1, 6, 2.f);

            //ISGraphics::textures.emplace_back(*backgroundTest);
            //ISGraphics::textures.emplace_back(*black_background);
            //ISGraphics::textures.emplace_back(*idle_anim_image);
            //ISGraphics::textures.emplace_back(*walking_anim_image);
            //ISGraphics::textures.emplace_back(*truck_anim_image);

            //creating game object and their components
            //entity_quad = engine.CreateEntityWithComponents<Sprite, Transform>("Background");
            //entity_player = engine.CreateEntityWithComponents<Sprite, InputAffector, Transform, RigidBody,ScriptComponent>("Player");
            //entity_test = engine.CreateEntityWithComponents<Sprite, InputAffector, Transform, RigidBody>("Test Texture");
            //entity_floor = engine.CreateEntityWithComponents<Sprite, InputAffector, Transform, RigidBody>("Floor");
            //entity_circle = engine.CreateEntityWithComponents<Sprite, Transform>("Clock Circle");
            //entity_line = engine.CreateEntityWithComponents<Sprite, Transform>("Clock Line");
            //entity_point = engine.CreateEntityWithComponents<Sprite, Transform>("Clock Point");
            //entity_button = engine.CreateEntityWithComponents<ButtonComponent>("Button");

            //auto& trans_quad = engine.GetComponent<Transform>(entity_quad);
            //auto& trans_player = engine.GetComponent<Transform>(entity_player);
            //auto& trans_test = engine.GetComponent<Transform>(entity_test);
            //auto& trans_floor = engine.GetComponent<Transform>(entity_floor);
            //auto& trans_circle = engine.GetComponent<Transform>(entity_circle);
            //auto& trans_line = engine.GetComponent<Transform>(entity_line);
            //auto& trans_point = engine.GetComponent<Transform>(entity_point);

            //auto& player_script = engine.GetComponent<ScriptComponent>(entity_player);
            //player_script.mScriptName = "PlayerScript";

            //auto& body_player = engine.GetComponent<RigidBody>(entity_player);
            //auto& body_floor = engine.GetComponent<RigidBody>(entity_floor);
            

            //auto& sprite_background = engine.GetComponent<Sprite>(entity_quad);
            //auto& sprite_player = engine.GetComponent<Sprite>(entity_player);
            //auto& sprite_test = engine.GetComponent<Sprite>(entity_test);
            //auto& sprite_circle = engine.GetComponent<Sprite>(entity_circle);
            //auto& sprite_line = engine.GetComponent<Sprite>(entity_line);
            //auto& sprite_point = engine.GetComponent<Sprite>(entity_point);

            //sprite_background.texture_width = black_background->width;
            //sprite_background.texture_height = black_background->height;
            //sprite_background.img = *black_background;
           /* sprite_ai.texture_width = black_background->width;
            sprite_ai.texture_height = black_background->height;*/
            

            //sprite_player.name = "player_sprite";
            //sprite_player.texture_width = idle_anim_image->width;
            //sprite_player.texture_height = idle_anim_image->height;
            //sprite_player.anims.emplace_back(idle_ani);
            //sprite_player.anims.emplace_back(walking_ani);
            //sprite_player.img = *idle_anim_image;
            //// sprite_player.anim_vect.emplace_back(ISGraphics::idle_ani);
            //// sprite_player.anim_vect.emplace_back(ISGraphics::walking_ani);

            //sprite_test.texture_width = backgroundTest->width;
            //sprite_test.texture_height = backgroundTest->height;
            //sprite_test.img = *backgroundTest;

            //int width = engine.GetWindowWidth();
            //int height = engine.GetWindowHeight();

            //trans_quad.setScaling(static_cast<float>(width), static_cast<float>(height));
            //trans_player.setScaling(width * 0.05375f, height * 0.13333333f);
            //trans_player.setWorldPosition(width * 0.f, height * 0.f);
       
            //trans_test.setScaling(width * -0.0625f, height * 0.11111111f);
            //trans_test.setWorldPosition(width * -0.25f, height * 0.f);
            //trans_floor.setScaling(width * 0.56034f, height * 0.10441f);
            //trans_floor.setWorldPosition(width * -0.00050056f, height * -0.443105f);
            //body_player.mAngularVelocity = 10.f;
            //body_player.mRestitution = 0.1f;
            
            //body_floor.mBodyType = BodyType::Static;
            //body_floor.mMass = 99999.f;
            //body_floor.mInvMass = 1.f / 99999.f;

            //trans_circle.setWorldPosition(width * 0.406f, height * 0.33333333f);
            //trans_circle.setScaling(width * 0.125f, height * 0.22222222f);
            //sprite_circle.primitive_type = GL_TRIANGLE_FAN;


            //trans_line.setWorldPosition(width * 0.406f, height * 0.33333333f);
            //trans_line.setScaling(width * 0.125f, height * 0.24972f);
            //trans_line.setRotation(0.f, 40.f);
            //sprite_line.primitive_type = GL_LINES;

            //trans_point.setWorldPosition(width * 0.406f, height * 0.33333333f);
            //sprite_point.primitive_type = GL_POINTS;
            
            //entity_ai = engine.CreateEntityWithComponents<Sprite, Transform, RigidBody, Pathfinder>("ai");
            //auto& trans_ai = engine.GetComponent<Transform>(entity_ai);
            //auto& body_ai = engine.GetComponent<RigidBody>(entity_ai);
            //auto& sprite_ai = engine.GetComponent<Sprite>(entity_ai);
            //sprite_ai.img = *truck_anim_image;
            //trans_ai.setScaling(width * 0.05375f, height * 0.13333333f);
            //trans_ai.setWorldPosition(200.f,-200.f);
            //body_ai.mAngularVelocity = 10.f;
            //body_ai.mRestitution = 0.1f;

            auto& scene_manager = SceneManager::Instance();
            scene_manager.LoadScene("Assets\\Scenes\\GameLevel.insight");
        }

        virtual void Update([[maybe_unused]] float delta) override {
            //if (InsightEngine::Instance().mRuntime == false) { return; }
            // Disable mouse/key event when GUI is using them
            auto const& gui = engine.GetImGuiLayer();
            auto [width, height] = engine.IsFullScreen() ? engine.GetMonitorSize() : engine.GetWindowSize();

            // transform camera
            ISGraphics::cameras[Camera::mActiveCamera].UpdateCamXform();

           // Sprite::draw_colored_quad({ 10,10 }, 0, { 400,400 }, { 1,1,1,1 },4);
            //Sprite::draw_instanced_3D_quads();
           
            auto system = engine.GetSystem<ParticleSystem>("Particle");
            Particle A = Particle::CreateParticle({ 0.f, 0.f }, 0.5f, { 5.2f, 5.2f }, 3.0f,
                color_red, 0.8f, 0.1f, 45.0f, 90.0f,
                pt_square, "", { (float)input->GetMousePosition().first,(float)input->GetMousePosition().second });
            system->SpawnParticles(A);



            //Entity player = engine.GetEntityByName("Player");
            //auto& body_player = engine.GetComponent<RigidBody>(player);
            //auto& collider_player = engine.GetComponent<Collider>(player);
            //auto& trans_player = engine.GetComponent<Transform>(player);
            //body_player.mMass = 20.f;
            //collider_player.mSelectedCollider.reset();
            //collider_player.mSelectedCollider.set(ColliderShape::BOX);
            //body_player.mInertia = 2.f;

            // limit angle of the player
            //float angle = trans_player.getRotation();
            //if (angle != 0.f && angle != 360.f) {
            //    if (angle < 90.f) {
            //        angle = (angle < 15.f) ? angle : 15.f;
            //    }
            //    else if (angle >= 90.f) {
            //        angle = (angle > 345.f) ? angle : 345.f;
            //    }
            //}
            //trans_player.setRotation(angle, body_player.mAngularVelocity);
            //body_player.mRestitution = 0.1f;
            //body_player.mStaticFriction = 1.2f;
            //body_player.mDynamicFriction = 1.1f;


            // Process Keyboard Events
            if (!gui->WantCaptureKeyboard()) {
                // Enable/disable GUI
                if (input->IsKeyPressed(GLFW_KEY_TAB)) {
                    engine.mRenderGUI = !engine.mRenderGUI;
                    IS_CORE_DEBUG("GUI {}", engine.mRenderGUI ? "Enabled" : "Disabled");
                }

                // Offset mouse position
                if (!engine.mRenderGUI) {
                    auto const& window_sys = engine.GetSystem<WindowSystem>("Window");
                    auto [width2, height2] = window_sys->GetWindowSize();
                    input->setCenterPos(width2 / 2.f, height2 / 2.f);
                    input->setRatio(static_cast<float>(width2), static_cast<float>(height2));
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

                // for jumping
                //if (input->IsKeyPressed(GLFW_KEY_SPACE)) {
                //    //IS_CORE_INFO("{}", static_cast<short>(BodyState::GROUNDED));
                //    if (body_player.mState == BodyState::GROUNDED && body_player.mVelocity.y <= 10.f) {
                //        body_player.AddVelocity(Vector2D(0.f, 800.f));
                //        body_player.mState = BodyState::JUMP;
                //    }
                //}

                //if (engine.HasComponent<Transform>(entity_player)) {
                //    auto& trans_player = engine.GetComponent<Transform>(entity_player);
                //    //ISgraphics::camera.UpdateCamPos(trans_player.world_position.x, trans_player.world_position.y);
                //    Camera::Instance().UpdateCamPos(trans_player.world_position.x, trans_player.world_position.y);
                //}

                //if (engine.HasComponent<Sprite>(entity_player)) {
                //    auto& sprite_player = engine.GetComponent<Sprite>(entity_player);
                //    if (input->IsKeyHeld(GLFW_KEY_W) || input->IsKeyHeld(GLFW_KEY_A) || input->IsKeyHeld(GLFW_KEY_S) || input->IsKeyHeld(GLFW_KEY_D)) {
                //        sprite_player.texture_width = walking_anim_image.width;
                //        sprite_player.texture_height = walking_anim_image.height;
                //        sprite_player.animation_index = 1;
                //        sprite_player.img = walking_anim_image;
                //    }
                //    else {
                //        sprite_player.texture_width = idle_anim_image.width;
                //        sprite_player.texture_height = idle_anim_image.height;
                //        sprite_player.animation_index = 0;
                //        sprite_player.img = idle_anim_image;
                //    }
                //}

                //if (engine.HasComponent<Transform>(entity_player)) {
                    //auto& trans_player = engine.GetComponent<Transform>(entity_player);
                    //auto& body_player = engine.GetComponent<RigidBody>(entity_player);

                //    //auto& trans2 = engine.GetComponent<Transform>(myEntity);
                //    int hori = input->IsKeyHeld(GLFW_KEY_D) - input->IsKeyHeld(GLFW_KEY_A);
                //    int verti = input->IsKeyHeld(GLFW_KEY_W) - input->IsKeyHeld(GLFW_KEY_S);
                //    Vector2D inputVelocity = Vector2D(hori * 10.f, verti * 10.f);
                //    body_player.AddVelocity(inputVelocity);
                //    // for jumping
                //    if (input->IsKeyPressed(GLFW_KEY_SPACE)) { 
                //        //IS_CORE_INFO("{}", static_cast<short>(BodyState::GROUNDED));
                //        if (body_player.mState == BodyState::GROUNDED && body_player.mVelocity.y <= 10.f) {
                //            body_player.AddVelocity(Vector2D(0.f, 500.f));
                //            body_player.mState = BodyState::JUMP;
                //        }
                //    }

                    //// flip image
                    //trans_player.scaling.x *= (input->IsKeyHeld(GLFW_KEY_A) && (trans_player.scaling.x > 0)) ||
                    //    (input->IsKeyHeld(GLFW_KEY_D) && (trans_player.scaling.x < 0)) ? -1 : 1;

                    //float rotate = static_cast<float>(input->IsKeyHeld(GLFW_KEY_E) - input->IsKeyHeld(GLFW_KEY_Q));
                    //trans_player.rotation += rotate * body_player.mAngularVelocity;
                    //trans_player.rotation = trans_player.rotation < 0.f ? 360.f : fmod(trans_player.rotation, 360.f);
                //}

                // rotate lines on clock
                //if (engine.HasComponent<Transform>(entity_line)) {
                //    auto& trans_lines = engine.GetComponent<Transform>(entity_line);
                //    trans_lines.rotation += trans_lines.angle_speed * delta;
                //}

                //if (input->IsKeyPressed(GLFW_KEY_Z)) {
                //    asset->PlaySoundByName("sound.MP3");
                //    SceneManager::Instance().CreateScene("zx_test");
                //}
                //if (input->IsKeyPressed(GLFW_KEY_X)) {
                //    asset->PlayMusicByName("Assets/Sounds/JOHN_Company History_Loop_120bpm.wav");
                //}
                //    SceneManager::Instance().CreateScene("zx_test2");
                //}
                //if (input->IsKeyPressed(GLFW_KEY_0)) {
                //    SceneManager::Instance().SwitchScene(0);
                //}
                //if (input->IsKeyPressed(GLFW_KEY_1)) {
                //    SceneManager::Instance().SwitchScene(1);
                //}


            }

            // input->PrintMouseWorldPos();

            // Process Mouse Events
            if (!gui->WantCaptureMouse()) {

                if (input->IsMouseButtonHeld(GLFW_MOUSE_BUTTON_1)) {
                    //std::vector<Entity> test;
                    //test = Physics::GetSelectedEntities({ static_cast<float>(input->GetMousePosition().first), static_cast<float>(input->GetMousePosition().second) }, mEntities);
                    //for (auto const& ent : test) {
                    //    //IS_CORE_INFO("entity selected: {}", ent);
                    //}
                    /*std::vector<Entity> test;
                    test = Physics::GetSelectedEntities({ static_cast<float>(input->GetMousePosition().first), static_cast<float>(input->GetMousePosition().second) }, mEntities);
                    for (auto const& ent : test) {
                        IS_CORE_INFO("(Zekai's algo) entity selected: {}", ent);
                    }*/
                    //IS_CORE_INFO("end of click");
                }

                //if (input->IsMouseButtonHeld(GLFW_MOUSE_BUTTON_3)) {
                //    for (int i = 0; i < 1; i++) {
                //        Entity a = engine.CreateEntityWithComponents<Sprite, Transform>("Small Box");
                //        auto& transl = engine.GetComponent<Transform>(a);
                //        transl.setScaling(width * 0.01875f, height * 0.042222222f);
                //        transl.setWorldPosition(static_cast<float>(input->GetMousePosition().first), static_cast<float>(input->GetMousePosition().second));
                //        //add the image in
                //        //spr.texture = backgroundTest.texture_id;
                //    }

                //}

                if (input->IsKeyPressed(GLFW_KEY_9)) {
                    for (int i = 0; i < 1; i++) {
                        Entity a = engine.CreateEntityWithComponents<Sprite, Transform, RigidBody, Collider,ScriptComponent>("Ice Cream Truck");
                        auto& transl = engine.GetComponent<Transform>(a);
                        transl.setScaling(width * 0.05f, height * 0.094222222f);
                        transl.setWorldPosition(static_cast<float>(input->GetMousePosition().first), static_cast<float>(input->GetMousePosition().second));
                        auto& spr = engine.GetComponent<Sprite>(a);
                        spr.name = "ice_cream_truck";
                        spr.img = *truck_anim_image;
                        //spr.anim_vect.emplace_back(ISGraphics::ice_cream_truck_ani);
                        spr.anims.emplace_back(ice_cream_truck_ani);
                        spr.animation_index = 0;
                        IS_CORE_DEBUG("Animsize: {}", spr.anims.size());
                        
                        //ISGraphics::cameras[Camera::mActiveCamera].UpdateCamPos(transl.world_position.x, transl.world_position.y);
                        //add the image in
                        //spr.texture = backgroundTest.texture_id;

                    }

                }

                //if (input->IsKeyPressed(GLFW_KEY_P)) {
                //    Entity a = engine.CreateEntityWithComponents<Sprite, Transform, RigidBody, ScriptComponent, Pathfinder>("Pathing Ice Cream Truck");
                //    auto& transl = engine.GetComponent<Transform>(a);
                //    transl.setScaling(width * 0.05f, height * 0.094222222f);
                //    transl.setWorldPosition(static_cast<float>(input->GetMousePosition().first), static_cast<float>(input->GetMousePosition().second));
                //    auto& spr = engine.GetComponent<Sprite>(a);
                //    spr.name = "ice_cream_truck";
                //    spr.img = *truck_anim_image;
                //    //spr.anim_vect.emplace_back(ISGraphics::ice_cream_truck_ani);
                //    spr.anims.emplace_back(ice_cream_truck_ani);
                //    spr.animation_index = 0;
                //    IS_CORE_DEBUG("PATH FINDING TRUCK SPAWNED");
                //    Waypoint target;
                //    auto& pathcomponent = engine.GetComponent<Pathfinder>(a);
                //    auto& transcomp = engine.GetComponent<Transform>(a);
                //    pathcomponent.mInitPos = Vector2D(transcomp.world_position);
                //    auto sys = engine.GetSystem<Pathfinding>("Pathfinding");

                //    pathcomponent.mEndPos = Vector2D((float)input->GetMousePosition().first, (float)input->GetMousePosition().second);
                //}



            }

            // rotate lines on clock
            //if (engine.HasComponent<Transform>(entity_line)) {
            //    auto& transLines = engine.GetComponent<Transform>(entity_line);
            //    transLines.rotation += transLines.angle_speed * delta;
            //}

            //if (engine.HasComponent<ButtonComponent>(entity_button)) {
            //    auto& buttoncomponent = engine.GetComponent<ButtonComponent>(entity_button);
            //    auto& buttonsprite = engine.GetComponent<Sprite>(entity_button);
            //    if (buttoncomponent.mButtonState == ButtonStates::Hovered) {
            //        buttonsprite.color= glm::vec3(1.f, 0.f, 1.f);
            //    }
            //    else { buttonsprite.color = glm::vec3(1.f, 1.f, 1.f); }
            //}


        }

        virtual void Draw([[maybe_unused]] float delta) override {








        }

        std::string GetName() override {
            return "Game Loop";
        }

    };


}

#endif