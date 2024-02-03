/*!
 * \file YouWinScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-01-2024
 * \brief
 * This header file contains
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
using System.Runtime.CompilerServices;
using System;
using System.Collections.Generic;
using System.Linq.Expressions;
using System.ComponentModel;
namespace IS
{
    class Enemy
    {
        static public Dictionary<int, EachEnemy> enemies = new Dictionary<int, EachEnemy>();

        static public void Init()
        {
            int entity_id = InternalCalls.GetCurrentEntityID();
            enemies[entity_id] = new EachEnemy();
            enemies[entity_id].Init();
        }

        static public void Update()
        {
            int entity_id = InternalCalls.GetCurrentEntityID();
            enemies[entity_id].update(); // Call Update on each GlitchingPlatform instance

        }



        /*static public void CleanUp()
        {
            InternalCalls.SetSpriteImageEntity(InternalCalls.GetSpriteImage("glitched_platform_vfx 2R6C.png"), InternalCalls.GetCurrentEntityID());
        }*/
    }

    public enum EnemyState : short
    {
        PATROLLING,
        FOLLOWING_PLAYER,
        ATTACKING,
        BEING_HIT,
        DEAD
    }

    class EachEnemy
    {
        // common
        public float speed = 100f;
        private Vector2D direction = new Vector2D(0f, 0f);
        public Vector2D enemy_pos = new Vector2D(0f, 0f);
        private Vector2D scaling = new Vector2D(257f, 183f);
        private Vector2D enemy_vel = new Vector2D(0f, 0f);

        // get hit
        //public bool isHit;
        private bool initialHit = false;
        private float being_hit_timer = 0f;
        static public int ENEMY_ID;
        static public int BEING_ATTACK_ENEMY_ID;

        // attack
        SimpleImage enemy_attack1;
        public Vector2D view_port = new Vector2D(1000f, 500f);

        // image and vfx
        SimpleImage enemy_get_hit_vfx;
        SimpleImage check_point;

        // enemy patrol
        public Vector2D enemy_left_point = new Vector2D(0f, 0f);
        public Vector2D enemy_right_point = new Vector2D(0f, 0f);
        public Vector2D target_point = new Vector2D(0f, 0f);
        public float enemy_patrol_distance = 500f;
        public float enemy_rest_timer_duration = 2f;
        public float enemy_rest_timer = 0f;
        public bool going_left;

        // states
        EnemyState previous_state;
        EnemyState current_state;
        public bool isPatrolling;
        public bool isAttacking;
        public bool isFollowingPlayer;
        public bool isDead;

        //static private int get_hit_vfx_entity;
        public void Init()
        {
            ENEMY_ID = InternalCalls.GetCurrentEntityID();
            BEING_ATTACK_ENEMY_ID = -1;
            enemy_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(ENEMY_ID));
            InternalCalls.TransformSetScaleEntity(scaling.x, scaling.y, ENEMY_ID);
            direction.x = 1f; // init
            //InternalCalls.ColliderNone(ENEMY_ID);

            // image and vfx
            enemy_get_hit_vfx = InternalCalls.GetSpriteImage("land_vfx 2R7C.png");
            check_point = InternalCalls.GetSpriteImage("dark_circle.png");
            enemy_attack1 = InternalCalls.GetSpriteImage("enemy_attack1.png");

            //get_hit_vfx_entity = InternalCalls.CreateEntityVFX("enemy get hit", enemy_get_hit_vfx);
            //InternalCalls.CreateAnimationFromSpriteEntity(2, 7, 0.8f, get_hit_vfx_entity);

            // enemy patrol
            enemy_left_point = new Vector2D(enemy_pos.x - enemy_patrol_distance / 2f, enemy_pos.y);
            enemy_right_point = new Vector2D(enemy_pos.x + enemy_patrol_distance / 2f, enemy_pos.y);
            target_point = enemy_left_point;
            enemy_rest_timer = enemy_rest_timer_duration;
            going_left = true;

            // init states
            previous_state = (EnemyState)(-1);
            current_state = EnemyState.PATROLLING;
            isPatrolling = true;
            isAttacking = false;
            isFollowingPlayer = false;
            isDead = false;
        }

        public void update()
        {
            if (GameManager.isGamePaused == true || PauseButtonScript.paused == true)
            {
                InternalCalls.RigidBodySetForce(0f, 0f);
                InternalCalls.TransformSetRotation(InternalCalls.GetTransformRotation(), 0f);
                return;
            }
            ENEMY_ID = InternalCalls.GetCurrentEntityID();
            //InternalCalls.RigidBodyAddForce(0f, -9.8f);
            InternalCalls.TransformSetRotationEntity(0f, 0f, ENEMY_ID);
            enemy_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(ENEMY_ID));
            enemy_vel = Vector2D.FromSimpleVector2D(InternalCalls.RigidBodyGetVelocity());
            
            InternalCalls.TransformSetScaleEntity(scaling.x, scaling.y, ENEMY_ID);

            /*if (isHit)
            {
                //Console.WriteLine("getting enemy");
                float vel_x = direction.x * speed;
                //float vel_y = direction.y * speed;
                // first hit
                if (!initialHit)
                {
                    // draw vfx animation once get hit
                    //InternalCalls.ResetSpriteAnimationFrameEntity(get_hit_vfx_entity);
                    EnemyInitialGetHit();
                    initialHit = true;
                }
                //DrawGetHitVFX(); // update vfx 
                // enemy moving backwards abit
                InternalCalls.RigidBodySetVelocityEntity(vel_x, 0f, ENEMY_ID);
                being_hit_timer += InternalCalls.GetDeltaTime();
                if (being_hit_timer > 0.5f)
                {
                    isHit = false;
                    initialHit = false;
                    being_hit_timer = 0f;
                }
                else
                {
                    // being hit animation here
                }
            }
            else
            {
                //RemoveGetHitVFX();
            }*/
            UpdateEnemyDirection();
            //DrawPatrolPoint();
            //EnemyPatrolling();
            EnemyStateMechine();
            DrawHealthBar();
            EnemyCollidingPlayer();
        }


        public void CleanUp()
        {

        }

        public void GetHitByPlayer(Vector2D vec)
        {
            previous_state = current_state;
            current_state = EnemyState.BEING_HIT;
            direction = vec;
        }

        private void DrawGetHitVFX()
        {
            //InternalCalls.TransformSetScaleEntity(scaling.x, scaling.y, get_hit_vfx_entity);
            //InternalCalls.TransformSetPositionEntity(enemy_pos.x + MathF.Sign(scaling.x) * -100f, enemy_pos.y + (InternalCalls.GetTransformScalingEntity(get_hit_vfx_entity).y - 200f) / 2f, get_hit_vfx_entity);
            //InternalCalls.TransformSetRotationEntity(InternalCalls.GetTransformRotation() + MathF.Sign(-scaling.x) * 270f, 0, get_hit_vfx_entity);
        }

        private void RemoveGetHitVFX()
        {
            //InternalCalls.TransformSetScaleEntity(0, 0, get_hit_vfx_entity);
            //InternalCalls.TransformSetPositionEntity(-99999, -99999, get_hit_vfx_entity);
        }

        private void DrawPatrolPoint()
        {
            /*InternalCalls.TransformSetScaleEntity(scaling.x, scaling.y, get_hit_vfx_entity);
            InternalCalls.TransformSetPositionEntity(enemy_left_point.x, enemy_left_point.y, get_hit_vfx_entity);
            InternalCalls.TransformSetRotationEntity(0f, 0f, get_hit_vfx_entity);*/
            InternalCalls.DrawImageAt
                (
                    new SimpleVector2D(enemy_left_point.x, enemy_left_point.y), 0, new SimpleVector2D(200f, 200f), check_point, 1f, 4
                );
            InternalCalls.DrawImageAt
                (
                    new SimpleVector2D(enemy_right_point.x, enemy_right_point.y), 0, new SimpleVector2D(200f, 200f), check_point, 1f, 4
                );
        }

        private void UpdateEnemyDirection()
        {
            if (MathF.Sign(scaling.x) != MathF.Sign(direction.x))
            {
                scaling.x *= -1; // flip the enemy over
            }
        }

        static public void EnemyCollidingPlayer()
        {
            // check enemy colliding with enemy
            if (InternalCalls.OnCollisionEnter())
            {
                if (InternalCalls.CompareCategory("Player"))
                {
                    //Console.WriteLine(PlayerScript.colliding_enemy_id); 
                    PlayerScript.is_colliding_enemy = true;
                    PlayerScript.colliding_enemy_id = ENEMY_ID;
                }
                /*else
                {
                    PlayerScript.is_colliding_enemy = false;
                    PlayerScript.colliding_enemy_id = -1;
                }*/

                if (InternalCalls.CompareCategory("Weapon"))
                {
                    PlayerScript.hitting_enemy_id = ENEMY_ID;
                    BEING_ATTACK_ENEMY_ID = ENEMY_ID;
                }
                else
                {
                    PlayerScript.hitting_enemy_id = -1;
                    BEING_ATTACK_ENEMY_ID = -1;
                }
                //Console.WriteLine(PlayerScript.colliding_enemy_id);
            }
            
        }

        private void EnemyGetHit()
        {
            //Console.WriteLine("getting enemy");
            float vel_x = direction.x * speed;
            //float vel_y = direction.y * speed;
            // first hit
            if (!initialHit)
            {
                // draw vfx animation once get hit
                //InternalCalls.ResetSpriteAnimationFrameEntity(get_hit_vfx_entity);
                EnemyInitialGetHit();
                initialHit = true;
            }
            //DrawGetHitVFX(); // update vfx 
            // enemy moving backwards abit
            InternalCalls.RigidBodySetVelocityEntity(vel_x, 0f, ENEMY_ID);
            being_hit_timer += InternalCalls.GetDeltaTime();
            if (being_hit_timer > 0.5f)
            {
                //isHit = false;
                current_state = previous_state; // back to previous state
                initialHit = false;
                being_hit_timer = 0f;
            }
            else
            {
                // being hit animation here
            }
        }

        private void EnemyInitialGetHit()
        {
            InternalCalls.AudioPlaySound("DieSound.wav", false, 0.2f);

            // load bleeding particles
            Random rnd = new Random();
            MyRandom my_rand = new MyRandom(129248189);
            for (int i = 0; i < 30; i++)
            {
                float rand = (float)rnd.NextDouble();
                float dir_rand = my_rand.NextFloat();
                float dir = MathF.Sign(scaling.x) > 0 ? 330 + 30 * dir_rand /* 330 to 360 */: 180 + 30 * dir_rand/* 180 to 210 */;
                float size = 10f * rand;
                float size_scale = 10 * rand;
                float alpha = 0.8f * rand;
                InternalCalls.GameSpawnParticleExtra(
                    enemy_pos.x + scaling.x * (rand - 0.5f), enemy_pos.y + scaling.y * (rand - 0.5f), dir, size, size_scale, alpha, 0f, 0.6f, 500f * rand, "Particle Enemy Bleeding.txt"
                 );
            }
        }

        private void EnemyPatrolling()
        {
            /*if (isPatrolling)
            {*/

            //}

            float dist = target_point.x - enemy_pos.x;
            //Vector2D dir = new Vector2D(0f, 0f);

            if (MathF.Abs(dist) < 10f) // rest then turn around
            {
                if (enemy_rest_timer > 0f) // enemy in resting
                {
                    enemy_rest_timer -= InternalCalls.GetDeltaTime();
                    InternalCalls.RigidBodySetVelocityEntity(0f, enemy_vel.y, ENEMY_ID);
                    if (enemy_rest_timer <= 0f)
                    {
                        if (going_left)
                        {
                            target_point = enemy_right_point;
                        }
                        else
                        {
                            target_point = enemy_left_point;
                        }
                        going_left = !going_left; // change direction
                        enemy_rest_timer = enemy_rest_timer_duration; // rest timer
                    }
                    else
                    {
                        return;
                    }
                }

            }
            direction.x = target_point.x - enemy_pos.x;
            // set enemy vel
            enemy_vel.x = speed * MathF.Sign(direction.x);
            InternalCalls.RigidBodySetVelocityEntity(enemy_vel.x, enemy_vel.y, ENEMY_ID);
        }

        private void EnemyStateMechine()
        {
            EnemyChangeState();
            switch (current_state)
            {
                case EnemyState.PATROLLING:
                    EnemyPatrolling();
                    break;
                case EnemyState.FOLLOWING_PLAYER:
                    break;
                case EnemyState.ATTACKING:
                    EnemyAttacking();
                    break;
                case EnemyState.BEING_HIT:
                    EnemyGetHit();
                    break;
                case EnemyState.DEAD:
                    break;
            }
        }

        private void EnemyChangeState()
        {

        }

        private void EnemyFollowingPlayer()
        {

        }

        private void EnemyAttacking()
        {
            
        }

        private void DrawHealthBar()
        {
            InternalCalls.DrawSquare(enemy_pos.x, enemy_pos.y + scaling.y/2f, 200f, 50f, 0f, 1f, 0f, 0.6f, 6);
        }
    }
}
