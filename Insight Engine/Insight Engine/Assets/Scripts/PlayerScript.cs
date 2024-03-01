/*!
 * \file PlayerScript.cs
 * \author  Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This source file is meant for the player movement and camera.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

using System.Numerics;
using System;
using System.Runtime.CompilerServices;
using System.ComponentModel;
using System.Diagnostics;
using System.Reflection.Metadata;

namespace IS
{
    enum PlayerAttackCombo : int
    {
        LightAttack  = 3,
        MediumAttack = 4,
        HeavyAttack  = 5
    }

    public class PlayerScript
    {
        //kill counter
        static public int ENEMY_SLAIN = 0;

        //death timers
        static private float respawn_timer = 0.6f;
        static private float respawn_timer_set = 0.6f;
        static public bool isDead = false;
        static public bool initialDeath = false;

        static private float camera_shake_duration = 0.2f;
        static private float camera_shake_duration_set = 0.2f;
        static private float camera_shake_timer = 0.02f;
        static private float camera_shake_set = 0.02f;
        //static private Vector2D camera_shake_dir = new Vector2D(0, 0);
        //static private float camera_shake_angle = 0f;

        //Powerup triggers
        static public bool Reward_DoubleJump = false;
        static public bool Reward_Dash = false;
        static public bool Reward_WallClimb = false;

        static public bool initialPowerUp = false;

        // health
        static public int Max_Health = 6;
        static public int Health = 6;
        static public Vector2D health_scaling = new Vector2D(150, 150);

        static public float respawn_x = 0f;
        static public float respawn_y = 0f;

        static public int SAVE_POINT_ID = 0;

        //public player variables

        static private int previous_footstep_frame = 0;
        static public bool isOnGrass = true;
        //static private bool play_footstep = false;

        //lighting

        static private int light_entity;
        static private float light_timer = 1f / 8f;
        static private float light_timer_set = 1 / 8f;
        static private float light_intensity;
        static private int light_changer = 1;

        //private entity

        static SimpleImage player_walk;
        static SimpleImage player_idle;
        static SimpleImage player_climb;
        static SimpleImage player_transparent;
        static SimpleImage player_land;
        static SimpleImage player_jump;
        static SimpleImage player_jump_vfx;
        static SimpleImage player_death_vfx;
        static SimpleImage player_powerup_vfx;
        static SimpleImage player_reward_dash;
        static SimpleImage player_reward_doublejump;
        static SimpleImage player_reward_wallclimb;
        //static SimpleImage player_attack;
        //static SimpleImage player_being_hit;
        static SimpleImage player_attack1;
        static SimpleImage player_attack2;
        static SimpleImage player_attack3;
        static SimpleImage player_health_empty;
        static SimpleImage player_health_full;
        static SimpleImage player_health_half;

        //psuedo animations for images
        static private float animation_speed = 0.07f;
        static private float animation_speed_set = 0.07f;
        static private int animation_current_frame = 0;

        static private float attack_animation_speed = 0.07f;
        static private float attack_animation_speed_set = 0.07f;
        static private int attack_animation_current_frame = 0;

        static private int land_entity;
        static private int jump_entity;
        static private int death_entity;
        static private int powerup_entity;

        static public int PLAYER_ID;

        //private variables for entity

        static private float xCoord;
        static private float yCoord;
        static private float width;
        static private float height;

        //double jump and above
        static private int jump_amount;
        static private int jump_amount_set = 1;
        static private float jump_timer = 0.2f;
        static private float jump_timer_set = 0.2f;
        static private bool isJumping = false;
        static private bool initial_land = false;
        static private float jumpHeight = 1100f;

        //coyote time
        static private float coyote_timer_duration = 0.15f;
        static private float coyote_timer;

        //dashing 
        static public float bullet_time_timer = 0.5f;
        static private float bullet_time_set = 0.5f;
        static private float dash_timer = 0.2f;
        static private float dash_set = 0.2f;
        static private bool canDash = false;
        static private bool initialDash = true;
        static public bool isDashing;

        static private float dashSpeed = 5000f;

        static public bool isGrounded;
        static public bool isFirstGrounded = false;
        static public Vector2D player_ground_pos = new Vector2D(0, 0);
        static private Vector2D apply_force = new Vector2D(0, 0);//dash dir


        static public int entity_feet;

        //movement
        static private float acceleration = 0f;
        static private float acceleration_base = 50f;
        static private float acceleration_increment = 10f;
        static private float max_acceleration = 100;
        static private float max_speed = 950f;
        static private float move_speed = 950f;

        static private int hori_movement;
        static private float gravity_scale;
        static private float fall_multiplier = 2f;

        // climbing
        static private bool isClimbing = false;
        static private float climbSpeed = 2500f;
        static private int entityWall;
        static int climbdir;

        // combat system
        static public int entity_attack;

        static private int combo_step = 0;
        static private int total_attack_in_one_combo = 3;
        static private float attack_interval = 0.5f; // same as the animation player
        static private float combo_interval = 1f;
        static private float attack_timer = 1f;
        static private bool isAttack = false;
        static private float attack_angle = 0f;
        static private bool initial_attack = false;
        static private bool hitting_enemy = false;
        static public int hitting_enemy_id;
        static public float attack_damage = 10f;
        static public Vector2D attack_range = new Vector2D(150f, 300f);
        static public float attack_stun_timer_duration = 0.2f;
        static public float attack_stun_timer = attack_stun_timer_duration;
        static public bool IsAttackStun = false;
        static public bool IsFirstAttackStun = false;

        static int random_attack_sound;

        // player getting hit
        static public float player_get_hit_timer_duration = 0.65f;
        static public float player_get_hit_timer = 0.65f;
        static public float being_hit_flicker_timer_duration = 0.2f;
        static public float being_hit_flicker_timer = 0.2f;

        // enemy info
        static public bool is_colliding_enemy;
        static public int colliding_enemy_id;
        static public Vector2D enemy_impulse = new Vector2D(3000f, 2000f);
        static private bool initial_get_hit = false;
        //static private string attack_type;

        //force calculations
        static private float Xforce = 0f;
        //static private float Yforce = 0f;

        // player pos
        static public Vector2D player_pos = new Vector2D(0, 0);
        static public Vector2D trans_scaling = new Vector2D(0, 0);
        static public Vector2D player_vel = new Vector2D(0, 0);
        //static public float collider_offset_x = 22f;

        //camera pos
        static public Vector2D camera_pos = new Vector2D(0, 0);
        static private Vector2D target_pos = new Vector2D(0, 0);
        static public float camera_zoom = 0.7f;

        //window height
        static private float WindowHeight = 0;
        static private float WindowWidth = 0;


        // stablize player
        static private float prev_x = 0f;
        static private float prev_rotation=0f;

        // Collectible

        static public int collection_count = 0;

        // hide health
        static public bool hideHealth = false;

        // light type
        static public int PLAYER_LIGHT = 0;

        public static int BoolToInt(bool boolValue)
        {
            return boolValue ? 1 : 0;
        }

        static public void Init()
        {
            //reset variables
            ENEMY_SLAIN = 0;

            WindowWidth = InternalCalls.GetWindowWidth();
            WindowHeight = InternalCalls.GetWindowHeight();

            //player_walk = InternalCalls.GetSpriteImage("Assets/Textures/player_walking.png");
            //player_idle = InternalCalls.GetSpriteImage("Assets/Textures/player_idle.png");

            player_walk = InternalCalls.GetSpriteImage("run_strip.png");
            player_idle = InternalCalls.GetSpriteImage("idle_strip.png");
            player_jump = InternalCalls.GetSpriteImage("jump_strip.png");
            player_climb = InternalCalls.GetSpriteImage("wall_climb_anim 4R3C.png");
            player_transparent = InternalCalls.GetSpriteImage("transparent.png");
            player_land = InternalCalls.GetSpriteImage("land_vfx 2R7C.png");
            player_jump_vfx = InternalCalls.GetSpriteImage("woosh_vfx 3R4C.png");
            player_death_vfx = InternalCalls.GetSpriteImage("Death Vfx R1 C11.png");
            player_powerup_vfx = InternalCalls.GetSpriteImage("collect_fragment_vfx 2R6C.png");

            player_reward_dash = InternalCalls.GetSpriteImage("Dash UI.png");
            player_reward_doublejump = InternalCalls.GetSpriteImage("Double Jump UI.png");
            player_reward_wallclimb = InternalCalls.GetSpriteImage("Wall Climb UI.png");
            //player_attack = InternalCalls.GetSpriteImage("dark_circle.png");
            //player_being_hit = InternalCalls.GetSpriteImage("Dash AfterImage.png");
            player_attack1 = InternalCalls.GetSpriteImage("player_attack1.png");
            player_attack2 = InternalCalls.GetSpriteImage("player_attack2.png");
            player_attack3 = InternalCalls.GetSpriteImage("player_attack3.png");
            player_health_empty = InternalCalls.GetSpriteImage("player_health_empty.png");
            player_health_full = InternalCalls.GetSpriteImage("player_health_full.png");
            player_health_half = InternalCalls.GetSpriteImage("player_health_half.png");
            //player_attack1 = InternalCalls.GetSpriteImage("player_attack1.png");

            // Initialization code
            //InternalCalls.NativeLog("Entity Initialized", (int)entity);
            InternalCalls.ResetAnimations();
            InternalCalls.CreateAnimationFromSprite(1, 12, 0.7f);
            InternalCalls.CreateAnimationFromSprite(1, 12, 0.7f);
            InternalCalls.CreateAnimationFromSprite(1, 12, 0.7f);

            // attack 1
            InternalCalls.CreateAnimationFromSprite(1, 6, 0.5f);

            // attack 2
            InternalCalls.CreateAnimationFromSprite(1, 8, 0.5f);

            // attack 3
            InternalCalls.CreateAnimationFromSprite(1, 8, 0.5f);

            // attack total
            InternalCalls.CreateAnimationFromSprite(1,22,3f);


            entity_feet = InternalCalls.CreateEntity("FeetCollider");
            entityWall = InternalCalls.CreateEntity("WallCollider");
            entity_attack = InternalCalls.CreateEntity("AttackCollider");
            width = InternalCalls.GetTransformScaling().x / 3f;
            height = InternalCalls.GetTransformScaling().y;
            InternalCalls.AddCollider(entity_feet);
            InternalCalls.AddCollider(entityWall);
            InternalCalls.AddCollider(entity_attack);
            InternalCalls.UpdateCategory(entity_attack, "Weapon");


            land_entity = InternalCalls.CreateEntityVFX("land", player_land);
            InternalCalls.CreateAnimationFromSpriteEntity(2, 7, 0.3f, land_entity);

            jump_entity = InternalCalls.CreateEntityVFX("jump", player_jump_vfx);
            InternalCalls.CreateAnimationFromSpriteEntity(3, 4, 0.3f, jump_entity);

            death_entity = InternalCalls.CreateEntityVFX("Death", player_death_vfx);
            InternalCalls.CreateAnimationFromSpriteEntity(1, 11, 0.9f, death_entity);

            light_entity = InternalCalls.CreateEntitySprite("Player Lighting");
            InternalCalls.AttachLightComponentToEntity(light_entity, 1, 0.43f, 0, 0.44f, 619);

            powerup_entity = InternalCalls.CreateEntityVFX("powerup", player_powerup_vfx);
            InternalCalls.CreateAnimationFromSpriteEntity(2, 6, 0.9f, powerup_entity);

            //InternalCalls.TransformSetScale(200f, 180f);

            // init gravity scale
            gravity_scale = InternalCalls.GetGravityScale();

            // hitting enemy info
            hitting_enemy_id = -1;

            // init enemy info
            is_colliding_enemy = false;

            CameraScript.camera_pos.x = player_pos.x;
            CameraScript.camera_pos.y = player_pos.y;
            InternalCalls.ChangeLightType(PLAYER_LIGHT);

        }

        static public void Update()
        {
            
            if (initialPowerUp)
            {
                InternalCalls.ResetSpriteAnimationFrameEntity(powerup_entity);
                InternalCalls.TransformSetScaleEntity(trans_scaling.x , trans_scaling.y , powerup_entity);
                InternalCalls.TransformSetPositionEntity(player_pos.x, player_pos.y, powerup_entity);
                InternalCalls.TransformSetRotationEntity(InternalCalls.GetTransformRotation(), 0, powerup_entity);
                initialPowerUp = false;
            }

            SimpleVector2D scaler = new SimpleVector2D(200f, 180f);
            //DRAW IMAGES OF THE REWARDS!!!! the powerups which are named reward
            if (Reward_Dash)
            {

                SimpleVector2D pos = new SimpleVector2D(CameraScript.camera_pos.x + scaler.x * 1.3f, CameraScript.camera_pos.y - WindowHeight / 1.3f + scaler.y);
                InternalCalls.DrawImageAt
                (
                    pos, 0, scaler, player_reward_dash, 1f, 4
                );

            }
            if (Reward_DoubleJump)
            {
                SimpleVector2D pos = new SimpleVector2D(CameraScript.camera_pos.x - scaler.x * 1.3f, CameraScript.camera_pos.y - WindowHeight / 1.3f + scaler.y);
                InternalCalls.DrawImageAt
                (
                    pos, 0, scaler, player_reward_doublejump, 1f, 4
                );

            }
            if (Reward_WallClimb)
            {
                SimpleVector2D pos = new SimpleVector2D(CameraScript.camera_pos.x, CameraScript.camera_pos.y - WindowHeight / 1.3f + scaler.y);
                InternalCalls.DrawImageAt
                (
                    pos, 0, scaler, player_reward_wallclimb, 1f, 4
                );

            }

            DrawHealthBar();



            if (GameManager.isGamePaused == true || PauseButtonScript.paused == true)
            {
                InternalCalls.RigidBodySetForce(0f, 0f);
                InternalCalls.TransformSetRotation(InternalCalls.GetTransformRotation(), 0f);
                return;
            }

            if (InternalCalls.KeyPressed((int)KeyCodes.LeftAlt)) // cheat code
            {
                player_ground_pos = new Vector2D(InternalCalls.GetMousePosition().x, InternalCalls.GetMousePosition().y);
                InternalCalls.TransformSetPosition(InternalCalls.GetMousePosition().x, InternalCalls.GetMousePosition().y);
            }

            if (InternalCalls.KeyPressed((int)KeyCodes.LeftControl)) // cheat code
            {
                Reward_Dash = true; Reward_DoubleJump = true; Reward_WallClimb = true;
            }


            //animation sets
            LightUpdate();

            if (InternalCalls.GetCurrentAnimationEntity(land_entity) >= 13)
            {
                InternalCalls.TransformSetScaleEntity(0, 0, land_entity);
                InternalCalls.TransformSetPositionEntity(-999, -9999, land_entity);
            }

            if (InternalCalls.GetCurrentAnimationEntity(jump_entity) >= 11)
            {
                InternalCalls.TransformSetScaleEntity(0, 0, jump_entity);
                InternalCalls.TransformSetPositionEntity(-999, -9999, jump_entity);
            }

            if (InternalCalls.GetCurrentAnimationEntity(death_entity) >= 10)
            {
                InternalCalls.TransformSetScaleEntity(0, 0, death_entity);
                InternalCalls.TransformSetPositionEntity(-999, -9999, death_entity);
            }

            if (InternalCalls.GetCurrentAnimationEntity(powerup_entity) >= 11)
            {
                InternalCalls.TransformSetScaleEntity(0, 0, powerup_entity);
                InternalCalls.TransformSetPositionEntity(-999, -9999, powerup_entity);
            }
            else
            {
                InternalCalls.TransformSetPositionEntity(player_pos.x, player_pos.y, powerup_entity);
            }


            // end of animation updates


            if (isDead)
            {

                if (initialDeath)
                {
                    InternalCalls.SetSpriteImage(player_transparent);
                    InternalCalls.TransformSetPositionEntity(player_pos.x, player_pos.y, death_entity);
                    InternalCalls.ResetSpriteAnimationFrameEntity(death_entity);
                    InternalCalls.TransformSetPosition(respawn_x, respawn_y);
                    InternalCalls.TransformSetScaleEntity(300, 300, death_entity);
                    isDashing = false;
                    dash_timer = dash_set;
                    bullet_time_timer = bullet_time_set;
                    InternalCalls.RigidBodySetForce(0, 0);
                    InternalCalls.AudioPlaySound("PlayerDeath.wav", false, 0.2f);
                    
                    for (int i = 0; i < 36; i++)
                    {
                        InternalCalls.GameSpawnParticleExtra(
                            player_pos.x, player_pos.y, i * 10, 8, -2, 0.9f, -0.1f, 1f, 300f, "Particle Test"
                         );
                    }


                    initialDeath = false;
                }
                else
                {
                    InternalCalls.TransformSetPosition(respawn_x, respawn_y);
                    CameraScript.camera_shake_duration = 0.1f;

                    respawn_timer -= InternalCalls.GetDeltaTime();
                    if (respawn_timer <= 0)
                    {
                        CameraScript.camera_shake_duration = camera_shake_duration;
                        InternalCalls.TransformSetPosition(respawn_x, respawn_y);
                        respawn_timer = respawn_timer_set;
                        isDead = false;
                        isJumping = true;

                    }

                }

                return;

            }

            initialDeath = true;

            if (Health <= 0)
            {

                Health = Max_Health;
                isDead = true;
            }

            //Attach Camera

            target_pos.x = player_pos.x + hori_movement * CustomMath.min(200f, CustomMath.Abs(InternalCalls.RigidBodyGetVelocity().x));
            target_pos.y = player_pos.y + CustomMath.min(100f, InternalCalls.RigidBodyGetVelocity().y / 20f);

            float interpolate_speed = 4f;

            /*            if (InternalCalls.RigidBodyGetVelocity().y < -1500)
                        {
                            interpolate_speed = 25f;
                        }*/

            camera_pos = Vector2D.Lerp(camera_pos, target_pos, interpolate_speed * InternalCalls.GetDeltaTime());

            CameraScript.camera_pos = camera_pos;

            PLAYER_ID = InternalCalls.GetCurrentEntityID();

            

            //movement
            hori_movement = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.D)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.A));
            player_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());

            // scaling transform with movement
            //Vector2D trans_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());
            trans_scaling = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScaling());
            float trans_rotate = InternalCalls.GetTransformRotation();
            if (InternalCalls.KeyHeld((int)KeyCodes.A)) { if (trans_scaling.x < 0) { trans_scaling.x *= -1; } isFirstGrounded = false;/* update player ground pos */ }
            if (InternalCalls.KeyHeld((int)KeyCodes.D)) { if (trans_scaling.x > 0) { trans_scaling.x *= -1; } isFirstGrounded = false; }

            // update player collider offset
            /*if (isGrounded)
            {
                InternalCalls.SetCircleColliderOffsetX(MathF.Sign(-trans_scaling.x) * collider_offset_x);
                
            }
            else
            {
                InternalCalls.SetCircleColliderOffsetX(0); // reset collider offset when climbing
            }*/

            /*            int rotate = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.Q)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.E));
                        trans_rotate += rotate * InternalCalls.GetRigidBodyAngularVelocity();*/
            /*            if (trans_rotate > 45 && trans_rotate < 270) { trans_rotate = 45; }
                        if (trans_rotate < -45) { trans_rotate = -45; }*/



            //SPRITE

            if (!isDashing && !isClimbing && !isJumping)
            {

                if (hori_movement != 0)
                {
                    acceleration += acceleration_increment;
                    if (acceleration > max_acceleration) { acceleration = max_acceleration; }
                    /*move_speed += acceleration;
                    if (move_speed > max_speed) { move_speed = max_speed; }*/

                    InternalCalls.SetSpriteAnimationIndex(0);
                    InternalCalls.SetSpriteImage(player_walk);

                }
                else
                {
                    //move_speed -= acceleration;
                    acceleration -= acceleration_increment;
                    if (acceleration < acceleration_base) { acceleration = acceleration_base; }
                    //if (move_speed < 0) { move_speed = 0; }


                    InternalCalls.SetSpriteAnimationIndex(1);
                    InternalCalls.SetSpriteImage(player_idle);

                }
            }



            //wall checking
            if (InternalCalls.EntityCheckCollide(entityWall) && hori_movement != 0 && InternalCalls.GetCollidingEntity(entityWall) != InternalCalls.GetCurrentEntityID()
                && InternalCalls.CollidingObjectTypeIsWall(InternalCalls.GetCollidingEntity(entityWall)) && Reward_WallClimb
                && !InternalCalls.CollidingObjectTypeIsIgnore(InternalCalls.GetCollidingEntity(entityWall)))
            {
                if (!isClimbing) { InternalCalls.RigidBodySetForce(0, 0); climbdir = hori_movement; }
                isClimbing = true; 
            }
            else { isClimbing = false; animation_current_frame = 0; animation_speed = animation_speed_set; }

            if (isClimbing)
            {
                //allow dashing when climbing
                canDash = true;
                isGrounded = true;
                jump_amount = jump_amount_set; //allow double dash

                InternalCalls.SetSpriteImage(player_transparent);
                InternalCalls.SetSpriteAnimationIndex(0);

                float collided_angle = InternalCalls.GetCollidedObjectAngle(entityWall) - (90 * hori_movement);

                InternalCalls.TransformSetRotation(collided_angle, 0);
                Vector2D force_from_angle = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(collided_angle * -hori_movement));

                InternalCalls.RigidBodySetForce(climbSpeed * force_from_angle.x * hori_movement, climbSpeed * force_from_angle.y);

                if (hori_movement != climbdir)
                {
                    isClimbing = false;
                    Xforce += hori_movement * 2000f;
                }


                float x_offset = 28 * hori_movement;
                SimpleVector2D pos = InternalCalls.GetTransformPosition();
                pos.x += x_offset;
                SimpleVector2D scale = new SimpleVector2D(InternalCalls.GetTransformScaling().x *0.7f, InternalCalls.GetTransformScaling().y * 0.7f);

                SimpleImage curr_image = player_climb;
                int cur_col = 0; int cur_row = 0;
                switch (animation_current_frame)
                {
                    case 0:
                        cur_col = 0; cur_row = 0;
                        break;
                    case 1:
                        cur_col = 1; cur_row = 0;
                        break;
                    case 2:
                        cur_col = 2; cur_row = 0;
                        break;
                    case 3:
                        cur_col = 0; cur_row = 1;
                        break;
                    case 4:
                        cur_col = 1; cur_row = 1;
                        break;
                    case 5:
                        cur_col = 2; cur_row = 1;
                        break;
                    case 6:
                        cur_col = 0; cur_row = 2;
                        break;
                    case 7:
                        cur_col = 1; cur_row = 2;
                        break;
                    case 8:
                        cur_col = 2; cur_row = 2;
                        break;
                    case 9:
                        cur_col = 0; cur_row = 3;
                        break;
                    case 10:
                        cur_col = 1; cur_row = 3;
                        break;
                    case 11:
                        cur_col = 2; cur_row = 3;
                        break;

                }

                animation_speed -= InternalCalls.GetDeltaTime();

                if (animation_speed <= 0)
                {

                    animation_speed = animation_speed_set;
                    animation_current_frame++;
                    if (animation_current_frame >= 10)
                    {
                        animation_current_frame = 0;
                    }

                }


                InternalCalls.DrawImageExtraAt(cur_row, cur_col, 4, 3, pos, 0f, scale, curr_image, 1f, 1);

            }
            else
            {
                InternalCalls.TransformSetScale(1f, 1f);
            }

            /*            if (InternalCalls.RigidBodyGetVelocity().y < -1600f)
                        {
                            initial_land = false;
                        }*/

            //jumping bool
            if (isJumping && jump_timer > 0f) { jump_timer -= InternalCalls.GetDeltaTime(); if (jump_timer <= 0) { isJumping = false; jump_timer = jump_timer_set; } }
            //if is grounded
            if (InternalCalls.EntityCheckCollide(entity_feet) && isClimbing == false && InternalCalls.GetCollidingEntity(entity_feet) != PLAYER_ID
                && !InternalCalls.CollidingObjectTypeIsIgnore(InternalCalls.GetCollidingEntity(entity_feet))
                && InternalCalls.GetCollidingEntity(entity_feet) != SAVE_POINT_ID && !InternalCalls.CollidingObjectIsSpikes(InternalCalls.GetCollidingEntity(entity_feet))
                 && !InternalCalls.CollidingObjectTypeIsGhost(InternalCalls.GetCollidingEntity(entity_feet)))
            {
                isGrounded = true;
                // make player not slide when on slope
                if (!isFirstGrounded)
                {
                    isFirstGrounded = true;
                    player_ground_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());
                }
                // let player rest and stop sliding when grounding
                if (!InternalCalls.KeyHeld((int)KeyCodes.A) && !InternalCalls.KeyHeld((int)KeyCodes.D) && !isJumping)
                {
                    InternalCalls.TransformSetPosition(player_ground_pos.x, player_ground_pos.y);
                    InternalCalls.RigidBodySetForce(0f, 0f);
                }
            }
            else
            {
                isGrounded = false;
                isFirstGrounded = false;
            }

            if (isGrounded && !isJumping) // check and update for coyote time
            {
                coyote_timer = coyote_timer_duration;
            }
            else
            {
                coyote_timer -= InternalCalls.GetDeltaTime();
            }
            
            if (isGrounded)
            {
                if (!isJumping)
                {

                    //landing
                    if (initial_land == false)
                    {
                        float yvel = InternalCalls.RigidBodyGetVelocityY();
                        float vol = -yvel / 3200f;
                        //InternalCalls.NativeLog("yvel: ", yvel);
                        if (yvel < -3200f) { vol = 1f; }
                        // InternalCalls.NativeLog("VOLUME: ", vol);
                        vol = CustomMath.max(vol, 0.1f);
                        // InternalCalls.NativeLog("VOLUME AFTER MAX: ", vol);
                        vol = CustomMath.min(vol, 1f);
                        // InternalCalls.NativeLog("VOLUME AFTER MIN: ", vol);
                        PlayRandomFootstep(vol);
                        initial_land = true;
                        InternalCalls.ResetSpriteAnimationFrameEntity(land_entity);
                        InternalCalls.TransformSetScaleEntity(CustomMath.max(vol * 400, 200f), CustomMath.max(vol * 400, 200f), land_entity);
                        InternalCalls.TransformSetPositionEntity(player_pos.x, player_pos.y + (InternalCalls.GetTransformScalingEntity(land_entity).y - 200f) / 2f, land_entity);
                        InternalCalls.TransformSetRotationEntity(InternalCalls.GetTransformRotation(), 0, land_entity);
                    }

                    float aangle = InternalCalls.GetTransformRotation();
                    /*                if (hori_movement != 0) {aangle+=180f; }*/
                    Vector2D f_angle = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(aangle));

                    //set move speed when grounded
                    // normal movement
                    InternalCalls.RigidBodySetForce(hori_movement * (move_speed + ((BoolToInt(isDashing)) * dashSpeed) * f_angle.x * -1f), f_angle.y * move_speed * hori_movement);
                    /*if (!isAttack)
                    {   
                    }
                    else
                    {
                        // moving due to attack
                        //InternalCalls.RigidBodySetVelocityEntity();
                    }*/
                    
                    //InternalCalls.RigidBodySetForce(hori_movement * (move_speed + ((BoolToInt(isDashing)) * dashSpeed) * f_angle.x * -1f), 0f);


                    // Set the rotation to be the same as the detected one
                    float collided_angle = InternalCalls.GetCollidedObjectAngle(entity_feet);
                    if ((collided_angle > 0 && collided_angle < 45) || (collided_angle > 315 && collided_angle < 360))
                    {
                        if(InternalCalls.GetTransformPosition().x!=prev_x)
                        {
                            InternalCalls.TransformSetRotation(collided_angle, 0);
                            prev_x = InternalCalls.GetTransformPosition().x;
                            prev_rotation = InternalCalls.GetTransformRotation();
                        }
                        else
                        {
                            InternalCalls.TransformSetRotation(prev_rotation, 0);
                        }

                    }
                    else
                    {
                        InternalCalls.TransformSetRotation(0, 0);
                    }



                    jump_amount = jump_amount_set;
                    canDash = true;

                    if (InternalCalls.KeyPressed((int)KeyCodes.Space))
                    {
                        InternalCalls.RigidBodySetForce(InternalCalls.RigidBodyGetVelocity().x, 0f);
                        Jump();
                        InternalCalls.SetSpriteImage(player_jump);
                        isJumping = true;
                    }
                    ApplyGravityChange();

                    int frame = InternalCalls.GetCurrentAnimationEntity(PLAYER_ID);
                    if ((frame == 2 || frame == 8) && hori_movement != 0 && frame != previous_footstep_frame)
                    {
                        //InternalCalls.Anim
                        // InternalCalls.NativeLog("Footsteps called at frame: ", frame);
                        PlayRandomFootstep(0.2f);
                        previous_footstep_frame = frame;


                    }


                }
            }
            else if (!isClimbing)
            { //while in the air

                trans_rotate = 0;
                InternalCalls.TransformSetRotation(trans_rotate, 0);

                InternalCalls.SetSpriteImage(player_jump);
                InternalCalls.SetSpriteAnimationIndex(2);
                
                if (coyote_timer > 0f && !isJumping) // coyote time for the first time 
                {
                    if (InternalCalls.KeyPressed((int)KeyCodes.Space))
                    {
                        InternalCalls.RigidBodySetForce(InternalCalls.RigidBodyGetVelocity().x, 0f);
                        Jump();
                        InternalCalls.SetSpriteImage(player_jump);
                        isJumping = true;
                    }
                }
                
                else if (jump_amount > 0 && Reward_DoubleJump) // double jump
                {
                    if (InternalCalls.KeyPressed((int)KeyCodes.Space))
                    {
                        InternalCalls.RigidBodySetForce(InternalCalls.RigidBodyGetVelocity().x, 0f);
                        Jump();
                        jump_amount--;
                        isJumping = true;
                    }
                }
                ApplyGravityChange();

                // no key input in the air
                if (!InternalCalls.KeyHeld((int)KeyCodes.A) && !InternalCalls.KeyHeld((int)KeyCodes.D) && !InternalCalls.KeyHeld((int)KeyCodes.Space) &&!isDashing )
                {
                    if (InternalCalls.RigidBodyGetVelocityY() > 10f) // if still going up, set vel y to 0 to make it falls
                    {
                        //InternalCalls.RigidBodySetForce(0f, 0f);
                        // do nothing to get rid of the weird bugs
                    }
                    else
                    {
                        InternalCalls.RigidBodySetForce(0f, InternalCalls.RigidBodyGetVelocityY());
                    }
                    
                }

                Xforce += hori_movement * move_speed;
                InternalCalls.RigidBodyAddForce(hori_movement * move_speed / 10, 0f);
                // limit the vel
                player_vel = Vector2D.FromSimpleVector2D(InternalCalls.RigidBodyGetVelocity());
                if (MathF.Abs(player_vel.x) > max_speed) { InternalCalls.RigidBodySetForce(MathF.Sign(player_vel.x) * max_speed, player_vel.y); }
                if (MathF.Abs(player_vel.y) > jumpHeight) { InternalCalls.RigidBodySetForce(player_vel.x, MathF.Sign(player_vel.y) * jumpHeight); }

                /*if (CustomMath.Abs(InternalCalls.RigidBodyGetVelocity().x) > max_speed)
                {
                    InternalCalls.RigidBodySetForceX(700 * hori_movement);
                }*/
                initial_land = false;
            }
            // limit the vel
            player_vel = Vector2D.FromSimpleVector2D(InternalCalls.RigidBodyGetVelocity());
            if (MathF.Abs(player_vel.x) > max_speed) { InternalCalls.RigidBodySetForce(MathF.Sign(player_vel.x) * max_speed, player_vel.y); }
            if (MathF.Abs(player_vel.y) > jumpHeight) { InternalCalls.RigidBodySetForce(player_vel.x, MathF.Sign(player_vel.y) * jumpHeight); }


            if (canDash && isDashing == false && Reward_Dash)
            {
                if (InternalCalls.KeyPressed((int)KeyCodes.LeftShift))
                {
                    isDashing = true;
                }

            }
            if (isDashing)
            {
                isGrounded = false;
                isFirstGrounded = false;
                if (bullet_time_timer > 0)
                {
                    bullet_time_timer -= InternalCalls.GetDeltaTime();
                    InternalCalls.SetGravityScale(gravity_scale * 0f);
                    InternalCalls.RigidBodySetForce(0, 0);

                    //Get mouse
                    Vector2D mouse_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetMousePosition());

                    float angle = CustomMath.AngleBetweenPoints(player_pos, mouse_pos);

                    hori_movement = 0;
                    InternalCalls.SetSpriteImage(player_idle);

                    if (mouse_pos.x < player_pos.x) { if (trans_scaling.x < 0) { trans_scaling.x *= -1; } } else { if (trans_scaling.x > 0) { trans_scaling.x *= -1; } }

                    apply_force = Vector2D.DirectionFromAngle(angle);
                    apply_force = apply_force.Normalize();
                    //InternalCalls.NativeLog("ANGLE IS: ", CustomMath.RadiansToDegrees(angle));
                    /*                    InternalCalls.NativeLog("DIR_X IS: ", apply_force.x);
                                        InternalCalls.NativeLog("DIR_Y IS: ", apply_force.y);*/
                    var color = (1f, 1f, 1f);
                    InternalCalls.DrawLineBetweenPoints(player_pos.x, player_pos.y, mouse_pos.x, mouse_pos.y, color);
                    // Render Circles
                    for (int i = 1; i <= 5; i++)
                    {
                        float scale_multiplier = (i / (float)bullet_time_set);
                        InternalCalls.DrawCircle(
                            player_pos.x, player_pos.y,
                            trans_scaling.x * 50f * (bullet_time_timer / scale_multiplier),
                            trans_scaling.y * 50f * (bullet_time_timer / scale_multiplier),
                            color
                        );
                    }

                }
                else
                {
                    Dashing();
                }
            }

            // check player being hit
            CheckPlayerGetHitAndFreeze();

            Attack();
            AttackAreaUpdate();
            AttackCameraShake(); // camera shake
            HitEnemy();
            EnemyAttack();
            

            FloorCheckerUpdate();
            WallCheckerUpdate();
            InternalCalls.TransformSetScale(trans_scaling.x, trans_scaling.y);//setting image flips

            Xforce = 0f; 
            //Yforce = 0f;
        }

        static public void CleanUp()
        {

            Health = Max_Health;
            Reward_DoubleJump = false;
            Reward_Dash = false;
            Reward_WallClimb = false;

        }


        static private void Dashing()
        {

            if (initialDash == true)
            {
                InternalCalls.AudioPlaySound("Sward-Whoosh_3.wav", false, 0.3f);
                initialDash = false;
            }


            for (int i = 0; i < 36; i++)
            {

                float direction = -1 ^ i * 10;
                float size = 20f * (bullet_time_timer / bullet_time_set);
                float scale = 2f;
                float dash_particle_alpha = 1f;
                float growth = -0.05f;
                float speed = 10000f;
                InternalCalls.GameSpawnParticleExtra(player_pos.x, player_pos.y,
                    direction, size, scale, dash_particle_alpha,
                    growth, bullet_time_set, speed, "Particle Test.txt");

            }

            float alpha = 1f - (dash_timer / dash_set);
            InternalCalls.GameSpawnParticleExtraImage(player_pos.x, player_pos.y,
                                                        0.0f, trans_scaling.x/2f, trans_scaling.y/2f, 1, alpha, 0.0f, 0.2f,
                                                        0, "Particle Empty.txt", "Dash AfterImage.png");
            canDash = false;
            isDashing = true;
            dash_timer -= InternalCalls.GetDeltaTime();

            if (dash_timer <= 0)
            {

                for (int i = 0; i < 72; i++)
                {

                    InternalCalls.GameSpawnParticleExtra(player_pos.x, player_pos.y, -1 ^ i * 5, 10, -1, 1, -0.005f, 2f, 1000, "Particle Test.txt");


                }

                isDashing = false;
                dash_timer = dash_set;
                bullet_time_timer = bullet_time_set;
                initialDash = true;
                InternalCalls.RigidBodySetForce(InternalCalls.RigidBodyGetVelocity().x / 3f, InternalCalls.RigidBodyGetVelocity().y / 3f);
                return;
            }

            InternalCalls.RigidBodySetForce(apply_force.x * dashSpeed, apply_force.y * dashSpeed);



        }

        static private void Jump()
        {
            InternalCalls.ResetSpriteAnimationFrameEntity(jump_entity);
            InternalCalls.TransformSetScaleEntity(100 * -hori_movement, 100, jump_entity);
            InternalCalls.TransformSetPositionEntity(player_pos.x, player_pos.y, jump_entity);
            InternalCalls.RigidBodyAddForce(0, jumpHeight);
            coyote_timer = 0f;
        }

        static private void ApplyGravityChange()
        {
            // update the player's gravity scale to make them fall faster
            InternalCalls.SetGravityScale(gravity_scale);
            if (InternalCalls.RigidBodyGetVelocity().y < 10f)
            {
                InternalCalls.SetGravityScale(gravity_scale * fall_multiplier * 1.1f); // higher jump
            }
            else if (jump_amount != 1 /*not apply to first jump only*/ ||
                InternalCalls.RigidBodyGetVelocity().y > 0f && !(InternalCalls.KeyHeld((int)KeyCodes.Space)))
            {
                InternalCalls.SetGravityScale(gravity_scale * fall_multiplier / 1.1f); // lower jump
            }
        }

        static private void FloorCheckerUpdate()
        {

            //Player x y coord
            xCoord = InternalCalls.GetTransformPosition().x;
            yCoord = InternalCalls.GetTransformPosition().y;
            float rotationAngle = InternalCalls.GetTransformRotation();
            float angleRadians = rotationAngle * (CustomMath.PI / 180.0f);
            float distanceBelow = height / 3.2f;

            Vector2D relativePosition = new Vector2D(0, distanceBelow);

            // Apply rotation to the relative position
            // Rotation matrix in 2D: [cos(theta), -sin(theta); sin(theta), cos(theta)]
            Vector2D rotatedRelativePosition = new Vector2D(
                (float)(relativePosition.x * CustomMath.Cos(angleRadians) + relativePosition.y * CustomMath.Sin(angleRadians)),
                (float)(relativePosition.x * CustomMath.Sin(angleRadians) - relativePosition.y * CustomMath.Cos(angleRadians))
                );


            // Calculate the absolute position for the floor checker
            Vector2D checkerPosition = new Vector2D(
                xCoord + rotatedRelativePosition.x,
                yCoord + rotatedRelativePosition.y
            );

            // Set the floor checker's position
            InternalCalls.TransformSetPositionEntity(checkerPosition.x, checkerPosition.y, entity_feet);
            InternalCalls.TransformSetRotationEntity(rotationAngle, 0, entity_feet);
            InternalCalls.TransformSetScaleEntity(width / 1.8f, 2f, entity_feet);

        }

        static private void WallCheckerUpdate()
        {
            if (hori_movement == 0) { InternalCalls.TransformSetPositionEntity(-99999, -99999, entityWall); return; }
            xCoord = InternalCalls.GetTransformPosition().x;
            yCoord = InternalCalls.GetTransformPosition().y;
            float rotationAngle = InternalCalls.GetTransformRotation();
            float angleRadians = rotationAngle * (CustomMath.PI / 180.0f);
            float distanceLeft = width * 0.68f * hori_movement;

            Vector2D relativePosition = new Vector2D(distanceLeft, 0);

            // Apply rotation to the relative position
            Vector2D rotatedRelativePosition = new Vector2D(
                (float)(relativePosition.x * CustomMath.Cos(angleRadians) + relativePosition.y * CustomMath.Sin(angleRadians)),
                (float)(relativePosition.x * CustomMath.Sin(angleRadians) - relativePosition.y * CustomMath.Cos(angleRadians))
            );

            // Calculate the absolute position for the wall checker
            Vector2D checkerPosition = new Vector2D(
                xCoord + rotatedRelativePosition.x,
                yCoord + rotatedRelativePosition.y
            );

            InternalCalls.TransformSetPositionEntity(checkerPosition.x, checkerPosition.y, entityWall);
            InternalCalls.TransformSetRotationEntity(rotationAngle, 0, entityWall);
            InternalCalls.TransformSetScaleEntity(2f, height / 2f, entityWall);
        }

        static float distance_light = width;
        static MyRandom randomlights = new MyRandom(12314);
        static private void LightUpdate()
        {
            light_timer -= InternalCalls.GetDeltaTime();
            if (light_timer <= 0)
            {

                light_changer *= -1;
                light_intensity = randomlights.NextFloat() * 0.6f;
                light_timer = light_timer_set;

                light_intensity = CustomMath.min(0.6f, light_intensity);
                light_intensity = CustomMath.max(0.4f, light_intensity);

            }


            InternalCalls.SetLightComponentToEntity(light_entity, 1, 0.43f, 0, light_intensity, 619);

            xCoord = InternalCalls.GetTransformPosition().x;
            yCoord = InternalCalls.GetTransformPosition().y;
            float rotationAngle = InternalCalls.GetTransformRotation();
            float angleRadians = rotationAngle * (CustomMath.PI / 180.0f);


            if (hori_movement != 0)
            {
                distance_light = width * hori_movement;

                if (isClimbing)
                {
                    distance_light = width * hori_movement * -1;
                }
                if (isDashing)
                {
                    distance_light = width * CustomMath.Normalize(trans_scaling.x);
                }


            }
            //Vector2D relativePosition = new Vector2D(distance_light, 0);
            Vector2D relativePosition = new Vector2D(0, 0);

            // Apply rotation to the relative position
            Vector2D rotatedRelativePosition = new Vector2D(
                (float)(relativePosition.x * CustomMath.Cos(angleRadians) + relativePosition.y * CustomMath.Sin(angleRadians)),
                (float)(relativePosition.x * CustomMath.Sin(angleRadians) - relativePosition.y * CustomMath.Cos(angleRadians))
            );

            // Calculate the absolute position for the wall checker
            Vector2D checkerPosition = new Vector2D(
                xCoord + rotatedRelativePosition.x,
                yCoord + rotatedRelativePosition.y
            );

            InternalCalls.TransformSetPositionEntity(checkerPosition.x, checkerPosition.y, light_entity);
            InternalCalls.TransformSetRotationEntity(rotationAngle, 0, light_entity);
            InternalCalls.TransformSetScaleEntity(1f, 1f, light_entity);



        }


        static MyRandom rando_footsteps = new MyRandom(12312415);
        static private void PlayRandomFootstep(float volume)
        {

            float num = rando_footsteps.NextFloat();
            if (isOnGrass)
            {
                if (num <= 0.1f)
                {
                    InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_1.wav", false, volume);
                    return;
                }
                if (num <= 0.2f)
                {
                    InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_2.wav", false, volume);
                    return;
                }
                if (num <= 0.3f)
                {
                    InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_3.wav", false, volume);
                    return;
                }
                if (num <= 0.4f)
                {
                    InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_4.wav", false, volume);
                    return;
                }
                if (num <= 0.5f)
                {
                    InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_5.wav", false, volume);
                    return;
                }
                if (num <= 0.6f)
                {
                    InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_6.wav", false, volume);
                    return;
                }
                if (num <= 0.7f)
                {
                    InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_7.wav", false, volume);
                    return;
                }
                if (num <= 0.8f)
                {
                    InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_8.wav", false, volume);
                    return;
                }
                if (num <= 0.85f)
                {
                    InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_9.wav", false, volume);
                    return;
                }
                if (num <= 0.9f)
                {
                    InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_10.wav", false, volume);
                    return;
                }
                if (num <= 0.95f)
                {
                    InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_11.wav", false, volume);
                    return;
                }
                if (num <= 1f)
                {
                    InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_12.wav", false, volume);
                    return;
                }


            }
            else
            {


                if (num <= 0.1f)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, volume);
                    return;
                }
                if (num <= 0.2f)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_2.wav", false, volume);
                    return;
                }
                if (num <= 0.3f)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_3.wav", false, volume);
                    return;
                }
                if (num <= 0.4f)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_4.wav", false, volume);
                    return;
                }
                if (num <= 0.5f)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_5.wav", false, volume);
                    return;
                }
                if (num <= 0.6f)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_6.wav", false, volume);
                    return;
                }
                if (num <= 0.7f)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_7.wav", false, volume);
                    return;
                }
                if (num <= 0.8f)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_8.wav", false, volume);
                    return;
                }
                if (num <= 0.9f)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_9.wav", false, volume);
                    return;
                }
                if (num <= 1f)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_10.wav", false, volume);
                    return;
                }


            }


            return;
        }

        static private void Attack()
        {
            //if (InternalCalls.MousePressed(0) && (!isAttack ))
            if (InternalCalls.MousePressed(0) && (!isAttack))
            {
                // play attack sound
                Random rnd = new Random();
                random_attack_sound = rnd.Next(0, 5);

                isAttack = true;
                //attack_type = "Light";
                combo_step++;
                switch (combo_step)
                {
                    case 1:
                        PlayAttack1Sound();
                        break;
                    case 2:
                        PlayAttack2Sound();
                        break;
                    case 3:
                        PlayAttack3Sound();
                        break;
                }
                if (combo_step > total_attack_in_one_combo)
                {
                    //attack_timer = combo_interval;
                    combo_step = 0; // reset attack step 
                    attack_timer = 0.99f; // reset timer
                }
                else
                {
                    //attack_timer = attack_interval;
                    attack_timer = 0f; // reset timer
                }

                // Reset all attack animation frames
                InternalCalls.ResetAnimationFrames(PLAYER_ID, (int)PlayerAttackCombo.LightAttack, (int)PlayerAttackCombo.HeavyAttack);

                //Get mouse and attack angle
                Vector2D mouse_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetMousePosition());
                attack_angle = CustomMath.AngleBetweenPoints(player_pos, mouse_pos);
                
            }
            if (attack_timer < combo_interval)
            {
                if (!IsAttackStun)
                {
                    attack_timer += InternalCalls.GetDeltaTime();
                }
                
                if (attack_timer < attack_interval) // play attack animation
                {
                    attack_animation_speed-=InternalCalls.GetDeltaTime();
                    if (attack_animation_current_frame > 21) { attack_animation_current_frame = 0; }
                    if (attack_animation_speed < 0) { attack_animation_speed=attack_animation_speed_set; }

                    // render attacking combo animation 123
                    switch (combo_step)
                    {
                        case 1:
                            //InternalCalls.SetSpriteAnimationIndex(0);

                            InternalCalls.SetSpriteImage(player_attack1);
                            InternalCalls.SetSpriteAnimationIndex((int)PlayerAttackCombo.LightAttack);

                            /*if (InternalCalls.GetCurrentAnimationEntity(PLAYER_ID) == 3 && !IsFirstAttackStun)
                            {
                                IsAttackStun = true;
                                IsFirstAttackStun = true;
                                attack_stun_timer = attack_stun_timer_duration;
                            }

                            if (IsAttackStun)
                            {
                                attack_stun_timer -= InternalCalls.GetDeltaTime();
                                InternalCalls.SetAnimationEntityPlaying(PLAYER_ID, false);
                                //InternalCalls.DrawImageExtraAt(0, 4, 1, 6, new SimpleVector2D(player_pos.x, player_pos.y), 0, new SimpleVector2D(trans_scaling.x, trans_scaling.y), player_attack1, 1);
                            }
                            else
                            {
                                InternalCalls.SetAnimationEntityPlaying(PLAYER_ID, true);
                            }

                            if (attack_stun_timer <= 0f)
                            {
                                IsAttackStun = false;
                            }*/


                            //Console.WriteLine(InternalCalls.GetCurrentAnimationEntity(PLAYER_ID));
                            //InternalCalls.DrawNonEnityAnimation(InternalCalls.GetDeltaTime(),new SimpleVector2D(player_pos.x,player_pos.y),0,new SimpleVector2D(InternalCalls.GetTransformScaling().x,InternalCalls.GetTransformScaling().y),player_attack1,1,2);
                            break;
                        case 2:
                            //InternalCalls.SetSpriteAnimationIndex(0);
                            InternalCalls.SetSpriteImage(player_attack2);
                            InternalCalls.SetSpriteAnimationIndex((int)PlayerAttackCombo.MediumAttack);
                            break;
                        case 3:
                            //InternalCalls.SetSpriteAnimationIndex(0);
                            InternalCalls.SetSpriteImage(player_attack3);
                            InternalCalls.SetSpriteAnimationIndex((int)PlayerAttackCombo.HeavyAttack);
                            break;
                    }
                    

                    // limit the player vel when doing attack
                    InternalCalls.RigidBodySetForce(InternalCalls.RigidBodyGetVelocityX() / 3f, InternalCalls.RigidBodyGetVelocityY() / 3f);
                }

                if (attack_timer >= attack_interval && combo_step != 0) // enable combo
                {
                    //attack_timer = 0f;
                    //combo_step = 0;
                    isAttack = false;
                    initial_attack = false;
                    IsFirstAttackStun = false;
                }
                if (attack_timer >= combo_interval) // reset sth when a combo ends
                {
                    combo_step = 0;
                    isAttack = false;
                    initial_attack = false;
                    IsFirstAttackStun = false;
                }
            }
            /*if (isAttack)
            {*/

            //}
        }

        static private void PlayAttack1Sound() // random attack1 sound
        {
            float volume = 0.2f;
            switch (random_attack_sound)
            {
                case 0:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_1_1.wav", false, volume);
                    break;
                case 1:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_2_1.wav", false, volume);
                    break;
                case 2:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_3_1.wav", false, volume);
                    break;
                case 3:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_4_1.wav", false, volume);
                    break;
                case 4:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_5_1.wav", false, volume);
                    break;
            }
        }

        static private void PlayAttack2Sound() // random attack2 sound
        {
            float volume = 0.2f;
            switch (random_attack_sound)
            {
                case 0:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_1_2.wav", false, volume);
                    break;
                case 1:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_2_2.wav", false, volume);
                    break;
                case 2:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_3_2.wav", false, volume);
                    break;
                case 3:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_4_2.wav", false, volume);
                    break;
                case 4:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_5_2.wav", false, volume);
                    break;
            }
        }

        static private void PlayAttack3Sound() // random attack3 sound
        {
            float volume = 0.2f;
            switch (random_attack_sound)
            {
                case 0:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_1_3.wav", false, volume);
                    break;
                case 1:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_2_3.wav", false, volume);
                    break;
                case 2:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_3_3.wav", false, volume);
                    break;
                case 3:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_4_3.wav", false, volume);
                    break;
                case 4:
                    InternalCalls.AudioPlaySound("Main Character Combo Attack_5_3.wav", false, volume);
                    break;
            }
        }

        static private void AttackAreaUpdate() // update attack area range
        {
            if (!isAttack) { InternalCalls.TransformSetPositionEntity(-999999, -999999, entity_attack); return; }
            
            CalibrateAttackAngle();
            Vector2D f_angle = Vector2D.DirectionFromAngle(attack_angle);
            f_angle = f_angle.Normalize();

            //float distanceLeft = 150f;

            Vector2D attack_pos = new Vector2D(
                player_pos.x + f_angle.x * attack_range.x,
                player_pos.y
            );
            //float angleDegree = attack_angle * (180.0f / CustomMath.PI);
            // flip player if neccessary
            if (attack_pos.x > player_pos.x) { if (trans_scaling.x > 0) { trans_scaling.x *= -1; } } else { if (trans_scaling.x < 0) { trans_scaling.x *= -1; } }

            //Vector2D attack_area_scaling = new Vector2D(150f, height / 2f);
            InternalCalls.TransformSetPositionEntity(attack_pos.x, attack_pos.y, entity_attack);
            InternalCalls.TransformSetRotationEntity(0, 0, entity_attack);
            InternalCalls.TransformSetScaleEntity(attack_range.x, attack_range.y, entity_attack);
            // draw attack range
            /*InternalCalls.DrawImageAt
                (
                    new SimpleVector2D(attack_pos.x, attack_pos.y), 0, new SimpleVector2D(attack_range.x, attack_range.y), player_attack, 0.5f, 6
                );*/
        }

        static private void CalibrateAttackAngle() // helper function
        {
            // calibrate the fangle to make attack area not towards to floor (when isGrounded)
            if (isGrounded)
            {
                if (-2.36f < attack_angle && attack_angle < -0.78) // between 3/4 PI and 1/4 PI
                {
                    if (attack_angle <= -1.57f) // calibrate to left
                    {
                        attack_angle = -2.36f;
                    }
                    else if (attack_angle > -1.57f) // lean to right
                    {
                        attack_angle = -0.78f;
                    }
                }
            }
        }

        static private void HitEnemy() // check weapon hit enemy
        {
            // check attack collider when colliding
            if (InternalCalls.OnEntityCollisionEnter(entity_attack))
            {
                // when colliding with enemy
                if (InternalCalls.CompareEntityCategory(entity_attack, "Enemy"))
                {
                    // enemy get hit
                    hitting_enemy = true;
                    if (isAttack && !initial_attack) // enemy get hit for the first time
                    {
                        Random rnd = new Random();
                        int random_hit_sound = rnd.Next(0, 2);
                        switch (random_hit_sound)
                        {
                            case 0:
                                InternalCalls.AudioPlaySound("HitSoft CTE01_80.2.wav", false, 0.5f);
                                break;
                            case 1:
                                InternalCalls.AudioPlaySound("HitSoft CTE01_80.1.wav", false, 0.5f);
                                break;
                        }

                        InternalCalls.ResetSpriteAnimationFrameEntity(land_entity);
                        // // get the id for enemy being attack (one only)
                        int attacking_enemy_id = InternalCalls.GetCollidingEnemyEntity(entity_attack);

                        if (InternalCalls.CheckEntityCategory(attacking_enemy_id, "Enemy"))
                        {
                            //Console.WriteLine(attacking_enemy_id);
                            //EachEnemy.BEING_ATTACK_ENEMY_ID = attacking_enemy_id;
                            //EachEnemy.GetHit(new Vector2D(-MathF.Sign(trans_scaling.x), 0f), attacking_enemy_id);
                            Enemy.enemies[attacking_enemy_id].GetHitByPlayer(new Vector2D(-MathF.Sign(trans_scaling.x), 0f), attack_damage);
                            initial_attack = true;
                        }
                    }
                    else if (isAttack) // still hitting the enemy
                    {
                        // drawing hitting enemy vfx
                        CalibrateAttackAngle();
                        Vector2D f_angle = Vector2D.DirectionFromAngle(attack_angle);
                        f_angle = f_angle.Normalize();

                        float distanceLeft = 250f;

                        Vector2D checkerPosition = new Vector2D(
                            player_pos.x + f_angle.x * distanceLeft,
                            //player_pos.y + f_angle.y * distanceLeft
                            player_pos.y
                        );
                        //float angleDegree = attack_angle * (180.0f / CustomMath.PI);

                        InternalCalls.TransformSetScaleEntity(MathF.Sign(-trans_scaling.x) * 257f, 183f, land_entity);
                        InternalCalls.TransformSetPositionEntity(checkerPosition.x, checkerPosition.y, land_entity);
                        //InternalCalls.TransformSetRotationEntity(angleDegree - 90f, 0, land_entity);
                        InternalCalls.TransformSetRotationEntity(90 * MathF.Sign(trans_scaling.x), 0, land_entity);

                    }
                    else
                    {
                        // not drawing hitting enemy vfx
                        InternalCalls.TransformSetScaleEntity(0, 0, land_entity);
                        InternalCalls.TransformSetPositionEntity(-99999, -99999, land_entity);
                    }
                }
                else
                {
                    hitting_enemy = false; // if attack area not collide with enemy
                }
            }
            else
            {
                hitting_enemy = false; // if attack area nbot collide with anything
            }

            // Apply Attack Stun Effect
            AttackStunEffect();
        }
        static public void AttackStunEffect()
        {
            if (InternalCalls.GetCurrentAnimationEntity(PLAYER_ID) == 3 && !IsFirstAttackStun && hitting_enemy)
            {
                IsAttackStun = true;
                IsFirstAttackStun = true;
                attack_stun_timer = attack_stun_timer_duration;
                if (combo_step == 3) // if is the last attack, make th attack stun longer
                {
                    attack_stun_timer = attack_stun_timer_duration * 1.5f;
                }
            }

            if (IsAttackStun)
            {
                attack_stun_timer -= InternalCalls.GetDeltaTime();
                InternalCalls.SetAnimationEntityPlaying(PLAYER_ID, false);
            }
            else
            {
                InternalCalls.SetAnimationEntityPlaying(PLAYER_ID, true);
            }

            if (attack_stun_timer <= 0f || !hitting_enemy)
            {
                IsAttackStun = false;
            }
        }

        static public void EnemyAttack() // check getting hit from enemy
        {
            if (is_colliding_enemy)
            {
                // player get hit back
                PlayerGetHit();
            }
            else
            {
                initial_get_hit = false;
            }
        }

        static public void CheckPlayerGetHitAndFreeze() // player flicker when get hit
        {
            if (is_colliding_enemy)
            {
                player_get_hit_timer -= InternalCalls.GetDeltaTime();
                if (player_get_hit_timer > 0f)
                {
                    // render get hit animation
                    /*InternalCalls.DrawImageAt
                    (
                        InternalCalls.GetTransformPosition(), 0, new SimpleVector2D(trans_scaling.x, trans_scaling.y), player_being_hit, 1f, 4
                    );*/
                    /*float alpha = 1f - (player_get_hit_timer / player_get_hit_timer_duration);
                    if (player_get_hit_timer > 0.1f)
                        InternalCalls.GameSpawnParticleExtraImage(player_pos.x, player_pos.y,
                                                                0.0f, trans_scaling.x, trans_scaling.y, 1, alpha, 0.0f, 0.2f,
                                                                0, "Particle Empty.txt", "Dash AfterImage.png");
                    */

                    //InternalCalls.SetAnimationAlpha(0.5f);
                    Render_Being_Hit_Flicker();

                    //return; // player cannot do anything
                }
                else
                {
                    InternalCalls.SetAnimationAlpha(1f); // reset
                    player_get_hit_timer = player_get_hit_timer_duration;

                    is_colliding_enemy = false;
                    colliding_enemy_id = -1;

                }
            }
        }

        static private void PlayerGetHit() // when get hit by enemy
        {
            if (!initial_get_hit)
            {
                CameraScript.camera_shake_duration = 0.2f;
                InternalCalls.AudioPlaySound("DieSound.wav", false, 0.2f);
                initial_get_hit = true;

                // getting enemy position
                Vector2D enemy_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(colliding_enemy_id));
                float dir = player_pos.x - enemy_pos.x;
                // make enemy facing player
                Enemy.enemies[colliding_enemy_id].direction.x = MathF.Sign(-dir);

                // damage the player
                Health -= Enemy.enemies[colliding_enemy_id].attack_damage;

                // player get hit back
                InternalCalls.TransformSetPosition(player_pos.x + MathF.Sign(dir) * 20f, player_pos.y + 50f);
                InternalCalls.RigidBodySetForce(MathF.Sign(dir) * enemy_impulse.x, enemy_impulse.y);
                //reset
                hori_movement = 0;
                InternalCalls.SetSpriteImage(player_idle);
            }
        }

        static private void Render_Being_Hit_Flicker() // render flicker when get hit
        {
            being_hit_flicker_timer -= InternalCalls.GetDeltaTime();
            if (being_hit_flicker_timer > 0.1f)
            {
                InternalCalls.SetAnimationAlpha(0.5f);
            }
            else if (being_hit_flicker_timer > 0f && being_hit_flicker_timer < 0.1f)
            {
                InternalCalls.SetAnimationAlpha(1f);
            }
            else
            {
                being_hit_flicker_timer = being_hit_flicker_timer_duration;
            }
        }

        static private void AttackCameraShake() // camera shake when attacking
        {
            if (isAttack && initial_attack) {
                if (camera_shake_duration > 0)
                {
                    camera_shake_duration -= InternalCalls.GetDeltaTime();

                    camera_shake_timer -= InternalCalls.GetDeltaTime();
                    Random rnd = new Random();
                    if (camera_shake_timer <= 0)
                    {
                        //camera_shake_dir = Vector2D.DirectionFromAngle(camera_shake_angle);
                        //camera_shake_angle += CustomMath.PI / 4;

                        float extent = 0.5f;
                        if (combo_step == 3)
                        {
                            extent = 1.5f;
                        }

                        CameraScript.camera_shake_dir.x = (float)rnd.NextDouble() - 0.5f; // random range from -0.5 to 0.5
                        CameraScript.camera_shake_dir.y = (float)rnd.NextDouble() - 0.5f;

                        CameraScript.camera_shake_timer = camera_shake_set;
                        CameraScript.camera_shake_duration = 0.1f;
                        CameraScript.camera_shake_intensity = extent;
                        Vector2D attack_dir = new Vector2D(0f, 0f);
                        attack_dir.x = InternalCalls.GetTransformPositionEntity(hitting_enemy_id).x - player_pos.x;
                        attack_dir.y = InternalCalls.GetTransformPositionEntity(hitting_enemy_id).y - player_pos.y;
                      //  if (hitting_enemy_id != -1)
/*                            CameraScript.camera_pos.x = CameraScript.camera_pos.x + 0.01f * attack_dir.x;
                        CameraScript.camera_pos.y = CameraScript.camera_pos.y + 0.01f * attack_dir.y;*/
                    }

                    /*attack_dir.x /= attack_dir.x;
                    attack_dir.y /= attack_dir.y;*/
                    //float rand = (float)rnd.NextDouble() - 0.5f; // random range from -0.5 to 0.5
                    //Console.WriteLine(Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(colliding_enemy_id)));
                    if (hitting_enemy_id != -1) { }
/*                    CameraScript.camera_shake_duration = 4f;
                    CameraScript.camera_pos.x=camera_pos.x + 0.01f * attack_dir.x;
                    CameraScript.camera_pos.y = camera_pos.y + 0.01f * attack_dir.y;*/
                }
            }
            else
            {
                camera_shake_duration = camera_shake_duration_set;
                //camera_shake_dir = new Vector2D(0, 0);
                //camera_shake_angle = 0;
            }
        }

        static private void DrawHealthBar() // draw health bar
        {
            if(hideHealth==true) return;

            SimpleVector2D pos = new SimpleVector2D(CameraScript.camera_pos.x - (WindowWidth / CameraScript.camera_zoom / 2.4f), CameraScript.camera_pos.y + WindowHeight / CameraScript.camera_zoom / 2.4f);
            SimpleVector2D scaling = new SimpleVector2D(health_scaling.x / CameraScript.camera_zoom, health_scaling.y / CameraScript.camera_zoom);
            float interval = scaling.x / 1.6f;
            // draw health bar
            switch (Health){
                case 1:
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x, pos.y), 0, scaling, player_health_half, 1f, 6);
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x + interval, pos.y), 0, scaling, player_health_empty, 1f, 6);
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x + interval * 2f, pos.y), 0, scaling, player_health_empty, 1f, 6);
                    break;
                case 2:
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x, pos.y), 0, scaling, player_health_full, 1f, 6);
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x + interval, pos.y), 0, scaling, player_health_empty, 1f, 6);
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x + interval * 2f, pos.y), 0, scaling, player_health_empty, 1f, 6);
                    break;
                case 3:
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x, pos.y), 0, scaling, player_health_full, 1f, 6);
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x + interval, pos.y), 0, scaling, player_health_half, 1f, 6);
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x + interval * 2f, pos.y), 0, scaling, player_health_empty, 1f, 6);
                    break;
                case 4:
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x, pos.y), 0, scaling, player_health_full, 1f, 6);
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x + interval, pos.y), 0, scaling, player_health_full, 1f, 6);
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x + interval * 2f, pos.y), 0, scaling, player_health_empty, 1f, 6);
                    break;
                case 5:
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x, pos.y), 0, scaling, player_health_full, 1f, 6);
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x + interval, pos.y), 0, scaling, player_health_full, 1f, 6);
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x + interval * 2f, pos.y), 0, scaling, player_health_half, 1f, 6);
                    break;
                case 6:
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x, pos.y), 0, scaling, player_health_full, 1f, 6);
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x + interval, pos.y), 0, scaling, player_health_full, 1f, 6);
                    InternalCalls.DrawImageAt(new SimpleVector2D(pos.x + interval * 2f, pos.y), 0, scaling, player_health_full, 1f, 6);
                    break;
            }
        }

    } //player script

}

