/*!
 * \file PlayerScript.cs
 * \author  Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This source file is meant for the player movement and camera.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
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
    public class PlayerScript
    {
        //death timers
        static private float respawn_timer = 0.6f;
        static private float respawn_timer_set = 0.6f;
        static public bool isDead = false;
        static public bool initialDeath = false;

        static private float camera_shake_duration = 0.2f;
        static private float camera_shake_duration_set = 0.2f;
        static private float camera_shake_timer = 0.02f;
        static private float camera_shake_set = 0.02f;
        static private Vector2D camera_shake_dir = new Vector2D(0, 0);
        static private float camera_shake_angle = 0f;

        //Powerup triggers
        static public bool Reward_DoubleJump = false;
        static public bool Reward_Dash = false;
        static public bool Reward_WallClimb = false;

        static public bool initialPowerUp = false;

        static public int Health = 3;
        static public int Max_Health = 3;

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

        //psuedo animations for images
        static private float animation_speed = 0.07f;
        static private float animation_speed_set = 0.07f;
        static private int animation_current_frame = 0;

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
        static private float jumpHeight = 1000f;

        //dashing 
        static public float bullet_time_timer = 1f;
        static private float bullet_time_set = 1f;
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
        static private float max_speed = 700f;
        static private float move_speed = 0f;

        static private int hori_movement;
        static private float gravity_scale;
        static private float fall_multiplier = 2f;

        // climbing
        static private bool isClimbing = false;
        static private float climbSpeed = 1500f;
        static private int entityWall;
        static int climbdir;


        //force calculations
        static private float Xforce = 0f;
        static private float Yforce = 0f;

        // player pos
        static public Vector2D player_pos = new Vector2D(0, 0);
        static private Vector2D trans_scaling = new Vector2D(0, 0);

        //camera pos
        static public Vector2D camera_pos = new Vector2D(0, 0);
        static private Vector2D target_pos = new Vector2D(0, 0);

        //window height
        static private int WindowHeight = 0;
        static private int WindowWidth = 0;


        public static int BoolToInt(bool boolValue)
        {
            return boolValue ? 1 : 0;
        }

        static public void Init()
        {
            WindowWidth= InternalCalls.GetWindowWidth();
            WindowHeight = InternalCalls.GetWindowHeight();

            Reward_DoubleJump = false;
            Reward_Dash = false;
            Reward_WallClimb = false;

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

            // Initialization code
            //InternalCalls.NativeLog("Entity Initialized", (int)entity);
            InternalCalls.ResetAnimations();
            InternalCalls.CreateAnimationFromSprite(1, 12, 1f);
            InternalCalls.CreateAnimationFromSprite(1, 12, 1f);
            InternalCalls.CreateAnimationFromSprite(1, 12, 1f);


            entity_feet = InternalCalls.CreateEntity("FeetCollider");
            entityWall = InternalCalls.CreateEntity("WallCollider");
            width = InternalCalls.GetTransformScaling().x / 3f;
            height = InternalCalls.GetTransformScaling().y;
            InternalCalls.AddCollider(entity_feet);
            InternalCalls.AddCollider(entityWall);


            InternalCalls.CameraSetZoom(1.2f);

            land_entity = InternalCalls.CreateEntityVFX("land", player_land);
            InternalCalls.CreateAnimationFromSpriteEntity(2, 7, 0.3f, land_entity);

            jump_entity = InternalCalls.CreateEntityVFX("jump", player_jump_vfx);
            InternalCalls.CreateAnimationFromSpriteEntity(3, 4, 0.3f, jump_entity);

            death_entity = InternalCalls.CreateEntityVFX("Death", player_death_vfx);
            InternalCalls.CreateAnimationFromSpriteEntity(1, 11, 0.9f, death_entity);

            light_entity = InternalCalls.CreateEntitySprite("Player Lighting");
            InternalCalls.AttachLightComponentToEntity(light_entity, 1, 0.43f, 0, 0.44f, 619);

            powerup_entity = InternalCalls.CreateEntityVFX("powerup",player_powerup_vfx);
            InternalCalls.CreateAnimationFromSpriteEntity(2, 6, 0.9f, powerup_entity);

            InternalCalls.TransformSetScale(200f, 180f);

            // init gravity scale
            gravity_scale = InternalCalls.GetGravityScale();

        }

        static public void Update()
        {

            if (initialPowerUp)
            {
                InternalCalls.ResetSpriteAnimationFrameEntity(powerup_entity);
                InternalCalls.TransformSetScaleEntity(trans_scaling.x * 1.5f, trans_scaling.y * 1.5f, powerup_entity);
                InternalCalls.TransformSetPositionEntity(player_pos.x, player_pos.y, powerup_entity);
                InternalCalls.TransformSetRotationEntity(InternalCalls.GetTransformRotation(), 0, powerup_entity);
                initialPowerUp = false;
            }

            SimpleVector2D scaler = new SimpleVector2D(230.5f, 230.5f);
            //DRAW IMAGES OF THE REWARDS!!!! the powerups which are named reward
            if (Reward_Dash)
            {
               
                SimpleVector2D pos = new SimpleVector2D(camera_pos.x+scaler.x,camera_pos.y-WindowHeight/2f - scaler.y/2f);
                InternalCalls.DrawImageAt
                (
                    pos,0,scaler,player_reward_dash,1f,4
                );

            }
            if(Reward_DoubleJump)
            {
                SimpleVector2D pos = new SimpleVector2D(camera_pos.x-scaler.x, camera_pos.y - WindowHeight / 2f - scaler.y/2f);
                InternalCalls.DrawImageAt
                (
                    pos, 0, scaler, player_reward_doublejump, 1f,4
                );

            }
            if(Reward_WallClimb)
            {
                SimpleVector2D pos = new SimpleVector2D(camera_pos.x, camera_pos.y - WindowHeight / 2f - scaler.y / 2f);
                InternalCalls.DrawImageAt
                (
                    pos, 0, scaler, player_reward_wallclimb, 1f,4
                );

            }



            if (GameManager.isGamePaused == true || PauseButtonScript.pause_enable == true) {
                InternalCalls.RigidBodySetForce(0f, 0f);
                return;
            }

            if (InternalCalls.KeyPressed((int)KeyCodes.LeftAlt)) {
                InternalCalls.TransformSetPosition(InternalCalls.GetMousePosition().x, InternalCalls.GetMousePosition().y);
            }

            if (InternalCalls.KeyPressed((int)KeyCodes.LeftControl))
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
            else {
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
                    InternalCalls.AudioPlaySound("DieSound.wav", false, 0.2f);

                    for (int i = 0; i < 36; i++) {
                        InternalCalls.GameSpawnParticleExtra(
                            player_pos.x, player_pos.y, i * 10, 8, -2, 0.9f, -0.1f, 1f, 300f, "Particle Test"
                         );
                    }


                    initialDeath = false;
                }
                else {

                    if (camera_shake_duration > 0)
                    {
                        camera_shake_duration -= InternalCalls.GetDeltaTime();

                        camera_shake_timer -= InternalCalls.GetDeltaTime();

                        if (camera_shake_timer <= 0)
                        {
                            camera_shake_dir = Vector2D.DirectionFromAngle(camera_shake_angle);
                            camera_shake_timer = camera_shake_set;
                            camera_shake_angle += CustomMath.PI / 4;
                        }
                        InternalCalls.AttachCamera(camera_pos.x + 20 * camera_shake_dir.x, camera_pos.y + 20 * camera_shake_dir.y);
                    }

                    respawn_timer -= InternalCalls.GetDeltaTime();
                    if (respawn_timer <= 0)
                    {
                        camera_shake_duration = camera_shake_duration_set;
                        camera_shake_angle = 0;

                        respawn_timer = respawn_timer_set;
                        isDead = false;
                        InternalCalls.CameraSetZoom(1.2f);

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



            InternalCalls.AttachCamera(camera_pos.x, camera_pos.y);

            PLAYER_ID = InternalCalls.GetCurrentEntityID();


            //movement
            hori_movement = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.D)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.A));
            player_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());

            // scaling transform with movement
            Vector2D trans_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());
            trans_scaling = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScaling());
            float trans_rotate = InternalCalls.GetTransformRotation();
            if (InternalCalls.KeyHeld((int)KeyCodes.A)) { if (trans_scaling.x < 0) { trans_scaling.x *= -1; } isFirstGrounded = false;/* update player ground pos */ }
            if (InternalCalls.KeyHeld((int)KeyCodes.D)) { if (trans_scaling.x > 0) { trans_scaling.x *= -1; } isFirstGrounded = false; }


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
                    move_speed += acceleration;
                    if (move_speed > max_speed) { move_speed = max_speed; }

                    InternalCalls.SetSpriteAnimationIndex(0);
                    InternalCalls.SetSpriteImage(player_walk);

                }
                else
                {
                    move_speed -= acceleration;
                    acceleration -= acceleration_increment;
                    if (acceleration < acceleration_base) { acceleration = acceleration_base; }
                    if (move_speed < 0) { move_speed = 0; }


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

            if (isClimbing) {


                InternalCalls.SetSpriteImage(player_transparent);
                InternalCalls.SetSpriteAnimationIndex(0);

                float collided_angle = InternalCalls.GetCollidedObjectAngle(entityWall) - (90 * hori_movement);

                InternalCalls.TransformSetRotation(collided_angle, 0);
                Vector2D force_from_angle = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(collided_angle * -hori_movement));

                InternalCalls.RigidBodySetForce(climbSpeed * force_from_angle.x * hori_movement, climbSpeed * force_from_angle.y);

                if (hori_movement != climbdir) {
                    isClimbing = false;
                    Xforce += hori_movement * 2000f;
                }


                float x_offset = 28 * hori_movement;
                SimpleVector2D pos = InternalCalls.GetTransformPosition();
                pos.x += x_offset;
                SimpleVector2D scale = new SimpleVector2D(InternalCalls.GetTransformScaling().x * 1.1f, InternalCalls.GetTransformScaling().y * 1.1f);

                SimpleImage curr_image = player_climb;
                int cur_col = 0; int cur_row = 0;
                switch (animation_current_frame) {
                    case 0:
                        cur_col = 0; cur_row = 0;
                        break; case 1:
                        cur_col = 1; cur_row = 0;
                        break; case 2:
                        cur_col = 2; cur_row = 0;
                        break; case 3:
                        cur_col = 0; cur_row = 1;
                        break; case 4:
                        cur_col = 1; cur_row = 1;
                        break; case 5:
                        cur_col = 2; cur_row = 1;
                        break; case 6:
                        cur_col = 0; cur_row = 2;
                        break; case 7:
                        cur_col = 1; cur_row = 2;
                        break; case 8:
                        cur_col = 2; cur_row = 2;
                        break; case 9:
                        cur_col = 0; cur_row = 3;
                        break; case 10:
                        cur_col = 1; cur_row = 3;
                        break; case 11:
                        cur_col = 2; cur_row = 3;
                        break;

                }

                animation_speed -= InternalCalls.GetDeltaTime();

                if (animation_speed <= 0) {

                    animation_speed = animation_speed_set;
                    animation_current_frame++;
                    if (animation_current_frame >= 10) {
                        animation_current_frame = 0;
                    }

                }


                InternalCalls.DrawImageExtraAt(cur_row,cur_col,4,3,pos, 0f, scale, curr_image, 1f, 1);

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
                if (!InternalCalls.KeyHeld((int)KeyCodes.A) && !InternalCalls.KeyHeld((int)KeyCodes.D) && !isJumping) { InternalCalls.TransformSetPosition(player_ground_pos.x, player_ground_pos.y); }
            }
            else
            {
                isGrounded = false;
                isFirstGrounded = false;
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
                        if (yvel < -3200f ) { vol= 1f; }
                       // InternalCalls.NativeLog("VOLUME: ", vol);
                        vol = CustomMath.max(vol, 0.1f);
                       // InternalCalls.NativeLog("VOLUME AFTER MAX: ", vol);
                        vol = CustomMath.min(vol, 1f);
                       // InternalCalls.NativeLog("VOLUME AFTER MIN: ", vol);
                        PlayRandomFootstep(vol);
                        initial_land = true;
                        InternalCalls.ResetSpriteAnimationFrameEntity(land_entity);
                        InternalCalls.TransformSetScaleEntity(CustomMath.max(vol*400, 200f), CustomMath.max(vol * 400, 200f), land_entity);
                        InternalCalls.TransformSetPositionEntity(player_pos.x, player_pos.y + (InternalCalls.GetTransformScalingEntity(land_entity).y - 200f) / 2f, land_entity);
                        InternalCalls.TransformSetRotationEntity(InternalCalls.GetTransformRotation(), 0, land_entity);
                    }

                    float aangle = InternalCalls.GetTransformRotation();
                    /*                if (hori_movement != 0) {aangle+=180f; }*/
                    Vector2D f_angle = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(aangle));
                    //set move speed when grounded

                    InternalCalls.RigidBodySetForce(hori_movement * (move_speed + ((BoolToInt(isDashing)) * dashSpeed) * f_angle.x * -1f), 0f);

                    // Set the rotation to be the same as the detected one
                    float collided_angle = InternalCalls.GetCollidedObjectAngle(entity_feet);
                    if ((collided_angle > 0 && collided_angle < 45) || (collided_angle > 315 && collided_angle < 360))
                    {
                        InternalCalls.TransformSetRotation(collided_angle, 0);
                    }
                    else
                    {
                        InternalCalls.TransformSetRotation(0, 0);
                    }



                    jump_amount = jump_amount_set;
                    canDash = true;

                    if (InternalCalls.KeyPressed((int)KeyCodes.Space))
                    {
                        Jump();
                        InternalCalls.SetSpriteImage(player_jump);
                        isJumping = true;
                    }
                    ApplyGravityChange();

                    int frame = InternalCalls.GetCurrentAnimationEntity(PLAYER_ID);
                    if ((frame==2 || frame == 8) && hori_movement != 0 && frame!=previous_footstep_frame) {
                        //InternalCalls.Anim
                       // InternalCalls.NativeLog("Footsteps called at frame: ", frame);
                        PlayRandomFootstep(0.2f);
                        previous_footstep_frame = frame;
                        

                    }


                }
            }
            else if (!isClimbing) { //while in the air

                trans_rotate = 0;
                InternalCalls.TransformSetRotation(trans_rotate, 0);

                InternalCalls.SetSpriteImage(player_jump);
                InternalCalls.SetSpriteAnimationIndex(2);

                if (jump_amount > 0 && Reward_DoubleJump)
                {
                    if (InternalCalls.KeyPressed((int)KeyCodes.Space))
                    {
                        InternalCalls.RigidBodySetForce(InternalCalls.RigidBodyGetVelocity().x, 0f);
                        Jump();
                        jump_amount--;
                    }
                }
                ApplyGravityChange();

                Xforce += hori_movement * move_speed;
                InternalCalls.RigidBodyAddForce(Xforce, Yforce);
                if (CustomMath.Abs(InternalCalls.RigidBodyGetVelocity().x) > 700)
                {
                    InternalCalls.RigidBodySetForceX(700 * hori_movement);
                }
                initial_land = false;
            }


            if (canDash && isDashing == false && Reward_Dash)
            {
                InternalCalls.GlitchEnable(false);
                if (InternalCalls.KeyPressed((int)KeyCodes.LeftShift)) {
                    isDashing = true;
                }

            }
            if (isDashing) {
                isGrounded = false;
                isFirstGrounded = false;
                if (bullet_time_timer > 0)
                {
                    bullet_time_timer -= InternalCalls.GetDeltaTime();
                    InternalCalls.RigidBodySetForce(0, 0);

                    //Get mouse
                    Vector2D mouse_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetMousePosition());

                    float angle = CustomMath.AngleBetweenPoints(player_pos, mouse_pos);

                    hori_movement = 0;
                    InternalCalls.SetSpriteImage(player_idle);

                    if (mouse_pos.x > player_pos.x) { if (trans_scaling.x < 0) { trans_scaling.x *= -1; } } else { if (trans_scaling.x > 0) { trans_scaling.x *= -1; } }

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



            FloorCheckerUpdate();
            WallCheckerUpdate();
            InternalCalls.TransformSetScale(trans_scaling.x, trans_scaling.y);//setting image flips

            Xforce = 0f; Yforce = 0f;



        }

        static public void CleanUp()
        {


        }


        static private void Dashing() {

            if (initialDash == true) {
                InternalCalls.AudioPlaySound("Sward-Whoosh_3.wav",false,0.3f);
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
                                                        0.0f, trans_scaling.x, trans_scaling.y, 1, alpha, 0.0f, 0.2f,
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

            InternalCalls.RigidBodySetForce(apply_force.x * dashSpeed * -1, apply_force.y * dashSpeed * -1);

        }

        static private void Jump()
        {
            InternalCalls.ResetSpriteAnimationFrameEntity(jump_entity);
            InternalCalls.TransformSetScaleEntity(100 * -hori_movement, 100, jump_entity);
            InternalCalls.TransformSetPositionEntity(player_pos.x, player_pos.y, jump_entity);
            InternalCalls.RigidBodyAddForce(0, jumpHeight);

            
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
                InternalCalls.RigidBodyGetVelocity().y > 10f && !(InternalCalls.KeyHeld((int)KeyCodes.Space)))
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
            float distanceBelow = height / 2.1f;

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
            if (hori_movement == 0) { InternalCalls.TransformSetPositionEntity(-999, -99999, entityWall); return; }
            xCoord = InternalCalls.GetTransformPosition().x;
            yCoord = InternalCalls.GetTransformPosition().y;
            float rotationAngle = InternalCalls.GetTransformRotation();
            float angleRadians = rotationAngle * (CustomMath.PI / 180.0f);
            float distanceLeft = width * hori_movement;

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
            Vector2D relativePosition = new Vector2D(distance_light, 0);

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

    } //player script

}

