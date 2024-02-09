/*!
 * \file Enemy.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2451
 * \date 08-02-2024
 * \brief
 * This header file contains the implementation of the Enemy class
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
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
        public float speed = 150f;
        public Vector2D direction = new Vector2D(0f, 0f);
        public Vector2D enemy_pos = new Vector2D(0f, 0f);
        private Vector2D scaling = new Vector2D(500f, 500f);
        private Vector2D enemy_vel = new Vector2D(0f, 0f);

        // get hit
        //public bool isHit;
        private bool initialHit = false;
        private float being_hit_timer = 0f;
        static public int ENEMY_ID;
        static public int BEING_ATTACK_ENEMY_ID;

        // attack
        SimpleImage enemy_attack1;
        SimpleImage enemy_attack2;
        public Vector2D view_port_pos = new Vector2D(0f, 0f);
        public Vector2D view_port_area = new Vector2D(500f, 200f);
        private float attack_timer_duration = 1f;
        private float attack_timer = 1f;
        private float attack_hit_timer = 0.5f;
        private bool initialAttack = false;
        private bool initialAttackSound = false;
        public Vector2D attack_pos = new Vector2D(0f, 0f);
        public Vector2D attack_area = new Vector2D(220f, 200f);
        public int attack_damage = 1;

        // random stuff
        private int random_attack;
        private int random_being_hit_sound;
        private int random_dead_sound;
        private int random_attack1_sound;
        private int random_attack2_sound;
        private int random_idle_sound;

        // sound
        private float volume = 0.2f;
        private float max_volume = 0.2f;
        private float volume_scale = 1f;

        // idle
        private float idle_sound_timer_duration = 3f;
        private float idle_sound_timer = 3f;
        private bool initialIdleSound = false;
        private float idle_sound_radius = 1000f;

        // image and vfx
        //SimpleImage enemy_get_hit_vfx;
        SimpleImage check_point;

        // health bar
        SimpleImage health_bar;
        private Vector2D health_bar_scaling = new Vector2D(200f, 50f);

        // enemy death
        SimpleImage enemy_death;
        private bool initialDeath = false;
        private float death_timer = 1f;

        // enemy patrol
        SimpleImage enemy_idle;
        public Vector2D enemy_left_point = new Vector2D(0f, 0f);
        public Vector2D enemy_right_point = new Vector2D(0f, 0f);
        public Vector2D target_point = new Vector2D(0f, 0f);
        public float enemy_patrol_distance = 500f;
        public float enemy_rest_timer_duration = 0.95f;
        public float enemy_rest_timer = 0f;
        public bool going_left;

        // states
        //EnemyState previous_state;
        EnemyState current_state;

        // enemy inof
        public float max_health = 100f;
        public float health = 100f;

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
            //enemy_get_hit_vfx = InternalCalls.GetSpriteImage("land_vfx 2R7C.png");
            check_point = InternalCalls.GetSpriteImage("dark_circle.png");
            enemy_attack1 = InternalCalls.GetSpriteImage("enemy_attack1.png");
            enemy_attack2 = InternalCalls.GetSpriteImage("enemy_attack2.png");
            enemy_death = InternalCalls.GetSpriteImage("enemy_death.png");
            enemy_idle = InternalCalls.GetSpriteImage("Enemy_Idle.png");
            health_bar = InternalCalls.GetSpriteImage("enemy_healthbar.png");

            //get_hit_vfx_entity = InternalCalls.CreateEntityVFX("enemy get hit", enemy_get_hit_vfx);
            //InternalCalls.CreateAnimationFromSpriteEntity(2, 7, 0.8f, get_hit_vfx_entity);

            // animation 
            InternalCalls.ResetAnimations();
            InternalCalls.CreateAnimationFromSprite(1, 12, 1f);
            InternalCalls.CreateAnimationFromSprite(1, 22, 1f);
            InternalCalls.CreateAnimationFromSprite(1, 21, 1f);

            // enemy patrol
            enemy_left_point = new Vector2D(enemy_pos.x - enemy_patrol_distance / 2f, enemy_pos.y);
            enemy_right_point = new Vector2D(enemy_pos.x + enemy_patrol_distance / 2f, enemy_pos.y);
            target_point = enemy_left_point;
            enemy_rest_timer = enemy_rest_timer_duration;
            going_left = true;

            // init states
            //previous_state = (EnemyState)(-1);
            current_state = EnemyState.PATROLLING;

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

            UpdateEnemyDirection();
            UpdateVolume();
            //DrawPatrolPoint();
            //EnemyPatrolling();
            EnemyStateMechine();
            DrawHealthBar();
            EnemyCollidingPlayer();
        }


        public void CleanUp()
        {

        }

        public void GetHitByPlayer(Vector2D vec, float damage)
        {
            //previous_state = current_state;
            current_state = EnemyState.BEING_HIT;
            direction = vec;
            health -= damage;
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
                    new SimpleVector2D(enemy_left_point.x, enemy_left_point.y), 0, new SimpleVector2D(100f, 100f), check_point, 1f, 6
                );
            InternalCalls.DrawImageAt
                (
                    new SimpleVector2D(enemy_right_point.x, enemy_right_point.y), 0, new SimpleVector2D(100f, 100f), check_point, 1f, 6
                );

            InternalCalls.DrawImageAt
                (
                    new SimpleVector2D(view_port_pos.x, view_port_pos.y), 0, new SimpleVector2D(view_port_area.x, view_port_area.y), check_point, 0.2f, 6
                );
            InternalCalls.DrawImageAt
                (
                    new SimpleVector2D(attack_pos.x, attack_pos.y), 0, new SimpleVector2D(attack_area.x, attack_area.y), check_point, 0.3f, 6
                );
        }

        private void UpdateEnemyDirection() // update enemy direction, view and attack position
        {
            if (MathF.Sign(scaling.x) != MathF.Sign(direction.x))
            {
                scaling.x *= -1; // flip the enemy over
            }
            view_port_pos = new Vector2D(enemy_pos.x + MathF.Sign(-direction.x) * view_port_area.x / 2, enemy_pos.y);
            attack_pos = new Vector2D(enemy_pos.x + MathF.Sign(-direction.x) * attack_area.x / 2, enemy_pos.y);
        }

        public void EnemyCollidingPlayer() // check if enemy colliding with player
        {
            // check enemy colliding with enemy
            if (InternalCalls.OnCollisionEnter() && current_state != EnemyState.DEAD)
            {
                if (InternalCalls.CompareCategory("Player"))
                {
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
            }

        }

        private void EnemyGetHit() // enemy get hit by player
        {
            float vel_x = direction.x * speed;
            //float vel_y = direction.y * speed;

            // first hit
            if (!initialHit)
            {
                // draw vfx animation once get hit
                //InternalCalls.ResetSpriteAnimationFrameEntity(get_hit_vfx_entity);
                Random rnd = new Random();
                random_being_hit_sound = rnd.Next(0, 9);
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
                //current_state = previous_state; // back to previous state
                current_state = EnemyState.FOLLOWING_PLAYER;
                initialHit = false;
                being_hit_timer = 0f;
            }
            else
            {
                // being hit animation here
            }
        }

        private void EnemyInitialGetHit() // enemy get hit sound and vfx
        {
            switch (random_being_hit_sound)
            {
                case 0:
                    InternalCalls.AudioPlaySound("Blobby Hurt_1.wav", false, volume);
                    break;
                case 1:
                    InternalCalls.AudioPlaySound("Blobby Hurt_2.wav", false, volume);
                    break;
                case 2:
                    InternalCalls.AudioPlaySound("Blobby Hurt_3.wav", false, volume);
                    break;
                case 3:
                    InternalCalls.AudioPlaySound("Blobby Hurt_4.wav", false, volume);
                    break;
                case 4:
                    InternalCalls.AudioPlaySound("Blobby Hurt_5.wav", false, volume);
                    break;
                case 5:
                    InternalCalls.AudioPlaySound("Blobby Hurt_6.wav", false, volume);
                    break;
                case 6:
                    InternalCalls.AudioPlaySound("Blobby Hurt_7.wav", false, volume);
                    break;
                case 7:
                    InternalCalls.AudioPlaySound("Blobby Hurt_8.wav", false, volume);
                    break;
                case 8:
                    InternalCalls.AudioPlaySound("Blobby Hurt_9.wav", false, volume);
                    break;
            }

            // load bleeding particles
            Random rnd = new Random();
            MyRandom my_rand = new MyRandom(129248189);
            for (int i = 0; i < 30; i++)
            {
                float rand = (float)rnd.NextDouble();
                float dir_rand = my_rand.NextFloat();
                float dir = MathF.Sign(scaling.x) > 0 ? 330 + 30 * dir_rand /* 330 to 360 */: 180 + 30 * dir_rand/* 180 to 210 */;
                float size = 10f * rand;
                float size_scale = 20 * rand;
                float alpha = 0.8f * rand;
                InternalCalls.GameSpawnParticleExtra(
                    enemy_pos.x + scaling.x * (rand - 0.5f), enemy_pos.y + scaling.y * (rand - 0.5f) / 6f, dir, size, size_scale, alpha, 0f, 0.6f, 500f * rand, "Particle Enemy Bleeding.txt"
                 );
            }
        }

        private void EnemyPatrolling() // enemy patrolling
        {
            Random rnd = new Random();
            if (idle_sound_timer > 0f)
            {
                idle_sound_timer -= InternalCalls.GetDeltaTime();

                if (idle_sound_timer < 1f)
                {
                    initialIdleSound = false; // reset
                }
            }
            else
            {
                if (!initialIdleSound && CheckPlayerNearby()) // play the idle sound once only
                {
                    random_idle_sound = rnd.Next(0, 10);
                    switch (random_idle_sound)
                    {
                        case 0:
                            InternalCalls.AudioPlaySound("Blobby Idle_1.wav", false, volume);
                            break;
                        case 1:
                            InternalCalls.AudioPlaySound("Blobby Idle_2.wav", false, volume);
                            break;
                        case 2:
                            InternalCalls.AudioPlaySound("Blobby Idle_3.wav", false, volume);
                            break;
                        case 3:
                            InternalCalls.AudioPlaySound("Blobby Idle_4.wav", false, volume);
                            break;
                        case 4:
                            InternalCalls.AudioPlaySound("Blobby Idle_5.wav", false, volume);
                            break;
                        case 5:
                            InternalCalls.AudioPlaySound("Blobby Idle_6.wav", false, volume);
                            break;
                        case 6:
                            InternalCalls.AudioPlaySound("Blobby Idle_7.wav", false, volume);
                            break;
                        case 7:
                            InternalCalls.AudioPlaySound("Blobby Idle_8.wav", false, volume);
                            break;
                        case 8:
                            InternalCalls.AudioPlaySound("Blobby Idle_9.wav", false, volume);
                            break;
                        case 9:
                            InternalCalls.AudioPlaySound("Blobby Idle_10.wav", false, volume);
                            break;
                    }
                    idle_sound_timer = idle_sound_timer_duration + (float)rnd.NextDouble() * 5f; // reset sound timer
                    initialIdleSound = true;
                }
                
            }

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
            direction.x = -direction.x;// going left is positive, right is negative
            InternalCalls.RigidBodySetVelocityEntity(enemy_vel.x, enemy_vel.y, ENEMY_ID);
        }

        private bool CheckPlayerNearby() // helper function check whether player is nearby
        {
            Vector2D distance = new Vector2D(PlayerScript.player_pos.x - enemy_pos.x, PlayerScript.player_pos.y - enemy_pos.y);
            float length = InternalCalls.MathSqrt(distance.x * distance.x + distance.y * distance.y);
            if (length < idle_sound_radius)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        private void UpdateVolume() // update overall volume
        {
            Vector2D distance = new Vector2D(PlayerScript.player_pos.x - enemy_pos.x, PlayerScript.player_pos.y - enemy_pos.y);
            float length = InternalCalls.MathSqrt(distance.x * distance.x + distance.y * distance.y);
            if (length < idle_sound_radius)
            {
                volume_scale = (idle_sound_radius - length) / idle_sound_radius;
            }
            else
            {
                volume_scale = 0f;
            }
            volume = max_volume * volume_scale;
        }

        private void EnemyStateMechine() // enemy state machine
        {
            EnemyChangeState();
            switch (current_state)
            {
                case EnemyState.PATROLLING:
                    //Console.WriteLine("patrolling");
                    EnemyPatrolling();
                    break;
                case EnemyState.FOLLOWING_PLAYER:
                    //Console.WriteLine("following player");
                    EnemyFollowingPlayer();
                    break;
                case EnemyState.ATTACKING:
                   // Console.WriteLine("attacking");
                    EnemyAttacking();
                    break;
                case EnemyState.BEING_HIT:
                   // Console.WriteLine("being hit");
                    EnemyGetHit();
                    break;
                case EnemyState.DEAD:
                    //Console.WriteLine("dead");
                    EnemyDead();
                    break;
            }
        }

        private void EnemyDead() // enemy dead
        {
            Random rnd = new Random();
            random_dead_sound = rnd.Next(0, 7);
            if (!initialDeath)
            {
                switch (random_dead_sound)
                {
                    case 0:
                        InternalCalls.AudioPlaySound("Blobby Death_1.wav", false, volume);
                        break;
                    case 1:
                        InternalCalls.AudioPlaySound("Blobby Death_2.wav", false, volume);
                        break;
                    case 2:
                        InternalCalls.AudioPlaySound("Blobby Death_3.wav", false, volume);
                        break;
                    case 3:
                        InternalCalls.AudioPlaySound("Blobby Death_4.wav", false, volume);
                        break;
                    case 4:
                        InternalCalls.AudioPlaySound("Blobby Death_5.wav", false, volume);
                        break;
                    case 5:
                        InternalCalls.AudioPlaySound("Blobby Death_6.wav", false, volume);
                        break;
                    case 6:
                        InternalCalls.AudioPlaySound("Blobby Death_7.wav", false, volume);
                        break;
                }
                initialDeath = true;
            }
            InternalCalls.RemoveColliderComponentEntity(ENEMY_ID);
            InternalCalls.SetSpriteImage(enemy_death);
            InternalCalls.SetSpriteAnimationIndex(2);
            death_timer -= InternalCalls.GetDeltaTime();
            if (death_timer <= 0f)
            {
                initialDeath = false;
                InternalCalls.DestroyEntity(ENEMY_ID);
            }
           
        }

        private void EnemyChangeState() // enemy change state
        {
            // if no health bar, then dead
            if (health <= 0f)
            {
                current_state = EnemyState.DEAD;
                return;
            }


            if (current_state == EnemyState.PATROLLING || current_state == EnemyState.FOLLOWING_PLAYER)
            {
                InternalCalls.SetSpriteImage(enemy_idle);
                InternalCalls.SetSpriteAnimationIndex(0);
            }

            switch (current_state)
            {
                case EnemyState.PATROLLING:

                    if (PlayerInSight())
                    {
                        current_state = EnemyState.FOLLOWING_PLAYER;
                    }

                    break;
                case EnemyState.FOLLOWING_PLAYER:
                    break;
                case EnemyState.ATTACKING:
                    break;
                case EnemyState.BEING_HIT:
                    break;
                case EnemyState.DEAD:
                    break;
            }
        }

        private void EnemyFollowingPlayer() // enemy following player
        {
            float dist = PlayerScript.player_pos.x - enemy_pos.x;

            if (MathF.Abs(dist) <= 220f) // attack player when getting close enough
            {
                Random rnd = new Random();
                random_attack = rnd.Next(0, 2);
                current_state = EnemyState.ATTACKING;
                return;
            }

            if (enemy_pos.x < enemy_left_point.x || enemy_pos.x > enemy_right_point.x || !PlayerInSight())
            {
                current_state = EnemyState.PATROLLING;
                //return;
            }

            direction.x = PlayerScript.player_pos.x - enemy_pos.x;
            // set enemy vel
            enemy_vel.x = speed * MathF.Sign(direction.x) * 1.8f;
            direction.x = -direction.x;// going left is positive, right is negative
            InternalCalls.RigidBodySetVelocityEntity(enemy_vel.x, enemy_vel.y, ENEMY_ID);
        }

        private void EnemyAttacking() // enemy attacking player
        {
            switch (random_attack)
            {
                case 0: // atack 1 animation
                    InternalCalls.SetSpriteImage(enemy_attack1);

                    break;
                case 1: // attack 2 animation
                    InternalCalls.SetSpriteImage(enemy_attack2);

                    break;
            }
            InternalCalls.SetSpriteAnimationIndex(1);

            Random rnd = new Random();
            // enemy attack sound
            if (!initialAttackSound)
            {
                // enemy attack sound
                switch (random_attack)
                {
                    case 0: // atack 1
                        random_attack1_sound = rnd.Next(0, 9);
                        switch (random_attack1_sound)
                        {
                            case 0:
                                InternalCalls.AudioPlaySound("Blobby Attack_1.wav", false, volume);
                                break;
                            case 1:
                                InternalCalls.AudioPlaySound("Blobby Attack_2.wav", false, volume);
                                break;
                            case 2:
                                InternalCalls.AudioPlaySound("Blobby Attack_3.wav", false, volume);
                                break;
                            case 3:
                                InternalCalls.AudioPlaySound("Blobby Attack_4.wav", false, volume);
                                break;
                            case 4:
                                InternalCalls.AudioPlaySound("Blobby Attack_5.wav", false, volume);
                                break;
                            case 5:
                                InternalCalls.AudioPlaySound("Blobby Attack_6.wav", false, volume);
                                break;
                            case 6:
                                InternalCalls.AudioPlaySound("Blobby Attack_7.wav", false, volume);
                                break;
                            case 7:
                                InternalCalls.AudioPlaySound("Blobby Attack_8.wav", false, volume);
                                break;
                            case 8:
                                InternalCalls.AudioPlaySound("Blobby Attack_9.wav", false, volume);
                                break;
                        }
                        break;
                    case 1: // attack 2
                        random_attack2_sound = rnd.Next(0, 6);

                        switch (random_attack2_sound)
                        {
                            case 0:
                                InternalCalls.AudioPlaySound("Blobby Attack-Swirl_1.wav", false, volume);
                                break;
                            case 1:
                                InternalCalls.AudioPlaySound("Blobby Attack-Swirl_2.wav", false, volume);
                                break;
                            case 2:
                                InternalCalls.AudioPlaySound("Blobby Attack-Swirl_3.wav", false, volume);
                                break;
                            case 3:
                                InternalCalls.AudioPlaySound("Blobby Attack-Swirl_4.wav", false, volume);
                                break;
                            case 4:
                                InternalCalls.AudioPlaySound("Blobby Attack-Swirl_5.wav", false, volume);
                                break;
                            case 5:
                                InternalCalls.AudioPlaySound("Blobby Attack-Swirl_6.wav", false, volume);
                                break;
                        }
                        break;
                }

                initialAttackSound = true;
            }
            attack_timer -= InternalCalls.GetDeltaTime();
            if (attack_timer < attack_hit_timer && attack_timer > 0.3f) // attack timing 0.5s to 0.3s
            {
                if (!initialAttack && PlayerInAttackRange())
                {
                    PlayerScript.is_colliding_enemy = true;
                    PlayerScript.colliding_enemy_id = ENEMY_ID;

                    initialAttack = true;
                }
            }
            
            if (attack_timer <= 0f)
            {
                attack_timer = attack_timer_duration;
                initialAttack = false;
                initialAttackSound = false;
                current_state = EnemyState.FOLLOWING_PLAYER;
            }
        }

        private bool PlayerInSight() // check if player in sight of enemy
        {
            // check if enemy has see player
            Vector2D min = new Vector2D(view_port_pos.x - view_port_area.x / 2, view_port_pos.y - view_port_area.y / 2);
            Vector2D max = new Vector2D(view_port_pos.x + view_port_area.x / 2, view_port_pos.y + view_port_area.y / 2);
            if (PlayerScript.player_pos.x > min.x && PlayerScript.player_pos.x < max.x &&
                PlayerScript.player_pos.y > min.y && PlayerScript.player_pos.y < max.y)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        private bool PlayerInAttackRange() // check if player in attack range
        {
            // check if player in attack range
            Vector2D min = new Vector2D(attack_pos.x - attack_area.x / 2, attack_pos.y - attack_area.y / 2);
            Vector2D max = new Vector2D(attack_pos.x + attack_area.x / 2, attack_pos.y + attack_area.y / 2);
            if (PlayerScript.player_pos.x > min.x && PlayerScript.player_pos.x < max.x &&
                               PlayerScript.player_pos.y > min.y && PlayerScript.player_pos.y < max.y)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        private void DrawHealthBar() // draw health bar
        {
            Vector3 color = new Vector3(0f, 0f, 0f);
            if (health > 50f)
            {
                color = new Vector3(0f, 1f, 0f);
            }
            else if (health > 20f)
            {
                color = new Vector3(1f, 1f, 0f);
            }
            else
            {
                color = new Vector3(1f, 0f, 0f);
            }
            int layer = InternalCalls.GetTopLayer();
            float health_wdith = (((health > 0) ? health : 0f) / max_health) * 155f; // width lenght of the health bar
            float health_pos_x = enemy_pos.x - ((max_health - health) / max_health) * 155f / 2f; // render position append of the health bar
            Vector2D health_bar_length = new Vector2D(health_wdith, 20f);

            // draw health bar only when the health is lesser
            if (health < max_health) {
                // draw health blood
                InternalCalls.DrawSquare(health_pos_x, enemy_pos.y + scaling.y / 6f, health_bar_length.x, health_bar_length.y, color.x, color.y, color.z, 0.7f, layer);

                // draw health bar UI
                SimpleVector2D pos = new SimpleVector2D(enemy_pos.x, enemy_pos.y + scaling.y / 6f);
                InternalCalls.DrawImageAt
                    (
                        pos, 0, new SimpleVector2D(health_bar_scaling.x, health_bar_scaling.y), health_bar, 1f, layer
                    );
            }
        }
    }
}
