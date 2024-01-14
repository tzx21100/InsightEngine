/*!
 * \file PlayerScript.cs
 * \author  Wu Zekai, zekai.wu@digipen.edu
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
    public class PlayerScript2
    {
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

        static public int player_id;

        static public bool isGrounded = false;

        //double jump and above
        static private int jump_amount;
        static private int jump_amount_set = 1;
        static private float jump_timer = 0.2f;
        static private float jump_timer_set = 0.2f;
        static private bool isJumping = false;
        static private bool initial_land = false;
        static private float jump_height = 1000f;
        static private float fall_multiplier = 2f;
        static private bool DoubleJumpEnable = false;
        static private int max_jumps = 2;
        static private int jumps_left;

        //movement
        static private float acceleration = 0f;
        static private float acceleration_base = 50f;
        static private float acceleration_increment = 10f;
        static private float max_acceleration = 100;
        static private float max_speed = 700f;
        static private float move_speed = 0f;

        static private int move_input;
        static private float gravity_scale;

        // player pos and dimension
        static public Vector2D player_pos = new Vector2D(0, 0);
        static public Vector2D player_vel = new Vector2D(0, 0);
        static private Vector2D trans_scaling = new Vector2D(0, 0);

        //camera pos
        static public Vector2D camera_pos = new Vector2D(0, 0);
        static private Vector2D target_pos = new Vector2D(0, 0);
        //static float camera_zoom = 0f;

        // Windows
        static Vector2D windows = new Vector2D(0, 0);
        static Vector2D orgin = new Vector2D(0, 0);

        static public void Init()
        {
            player_id = InternalCalls.GetCurrentEntityID();

            // Init Image
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

            jump_entity = InternalCalls.CreateEntityVFX("jump", player_jump_vfx);
            InternalCalls.CreateAnimationFromSpriteEntity(3, 4, 0.3f, jump_entity);

            if (InternalCalls.GetCurrentAnimationEntity(jump_entity) >= 11)
            {
                InternalCalls.TransformSetScaleEntity(0, 0, jump_entity);
                InternalCalls.TransformSetPositionEntity(-999, -9999, jump_entity);
            }

            //isGrounded = false;

            gravity_scale = InternalCalls.GetGravityScale();

            // camera
            InternalCalls.CameraSetZoom(1f);
        }

        static public void Update()
        {
            InternalCalls.TransformSetRotation(0f,0f);
            player_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());

            trans_scaling = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScaling());
            if (InternalCalls.KeyHeld((int)KeyCodes.A)) { if (trans_scaling.x < 0) { trans_scaling.x *= -1; } }
            else if (InternalCalls.KeyHeld((int)KeyCodes.D)) { if (trans_scaling.x > 0) { trans_scaling.x *= -1; } }
            else { InternalCalls.RigidBodySetForceX(player_vel.x/1.2f); } // let player slide for abit
            move_input = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.D)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.A));
            
            if (move_input != 0)
            {
                InternalCalls.SetSpriteAnimationIndex(0);
                InternalCalls.SetSpriteImage(player_walk);
            }
            else
            {
                InternalCalls.SetSpriteAnimationIndex(1);
                InternalCalls.SetSpriteImage(player_idle);
            }

            player_vel = Vector2D.FromSimpleVector2D(InternalCalls.RigidBodyGetVelocity());
            //transform.Translate(movement * 5 * Time.deltaTime);
            InternalCalls.RigidBodyAddForce(move_input * 30, 0);
            if (MathF.Abs(player_vel.x) > max_speed)
            {
                InternalCalls.RigidBodySetForce(MathF.Sign(player_vel.x) * max_speed, player_vel.y);
            }
            CollisionEnterCheck();
            if (isGrounded && !(InternalCalls.KeyHeld((int)KeyCodes.Space))) // if player on the ground
            {
                jumps_left = max_jumps;
                //DoubleJumpEnable = false;
                /*if (InternalCalls.KeyPressed((int)KeyCodes.Space))
                {
                    //Jump();
                    *//*InternalCalls.ResetSpriteAnimationFrameEntity(jump_entity);
                    InternalCalls.TransformSetScaleEntity(100 * -move_input, 100, jump_entity);
                    InternalCalls.TransformSetPositionEntity(player_pos.x, player_pos.y, jump_entity);
                    InternalCalls.SetSpriteImage(player_jump);*//*
                    InternalCalls.RigidBodyAddForce(0f, jump_height);
                    isGrounded = false;
                    DoubleJumpEnable = true;
                }*/
                //InternalCalls.SetGravityScale(gravity_scale * 0f);
            }

            if (InternalCalls.KeyPressed((int)KeyCodes.Space) && jumps_left > 0)
            {
               // if (isGrounded || DoubleJumpEnable)
                {
                    InternalCalls.RigidBodySetForce(0f, 0f); //reset the vel otherwise double jump will be higher
                    InternalCalls.RigidBodyAddForce(0f, jump_height);
                    isGrounded = false;
                    jumps_left -= 1;
                    //DoubleJumpEnable = !DoubleJumpEnable; // it change from false to true during first jumping, true to false during second jumping
                }
            }

            // when jumping in the air
            if (!isGrounded)
            {
                InternalCalls.SetSpriteAnimationIndex(2);
                InternalCalls.SetSpriteImage(player_jump);
                InternalCalls.SetGravityScale(gravity_scale);
                if (player_vel.y < 10f)
                {
                    InternalCalls.SetGravityScale(gravity_scale * fall_multiplier); // higher jump
                }
                else if(jumps_left != 1 /*not apply to first jump only*/ || 
                    player_vel.y > 10f && !(InternalCalls.KeyHeld((int)KeyCodes.Space)))
                {
                    InternalCalls.SetGravityScale(gravity_scale * fall_multiplier / 1.1f); // lower jump
                }
            }
            

            // camera
            target_pos.x = player_pos.x + move_input * CustomMath.min(200f, CustomMath.Abs(InternalCalls.RigidBodyGetVelocity().x));
            target_pos.y = player_pos.y + CustomMath.min(100f, InternalCalls.RigidBodyGetVelocity().y / 20f);

            float interpolate_speed = 4f;
            //camera_pos = Vector2D.Lerp(camera_pos, target_pos, interpolate_speed * InternalCalls.GetDeltaTime());

            camera_pos = player_pos;
            InternalCalls.AttachCamera(camera_pos.x, camera_pos.y);

            InternalCalls.TransformSetScale(trans_scaling.x, trans_scaling.y);//setting image flips
        }

        static public void CleanUp()
        {
            // Empty for now
        }

        public static int BoolToInt(bool boolValue)
        {
            return boolValue ? 1 : 0;
        }

        public static void CollisionEnterCheck()
        {
            if (InternalCalls.OnCollisionEnter())
            {
                if (InternalCalls.CompareCategory("Ground"))
                {
                    isGrounded = true;
                }
                else
                {
                    isGrounded = false;
                }
            }
        }
    } //player script

}

