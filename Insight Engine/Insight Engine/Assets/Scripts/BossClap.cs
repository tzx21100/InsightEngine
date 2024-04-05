
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System;
using System.Linq.Expressions;
using System.ComponentModel;
namespace IS
{
    class BossClap
    {
        static private int LEFT_HAND_ID;
        static private int RIGHT_HAND_ID;

        static private Vector2D clapping_pos = new Vector2D(99999f, 99999f);
        static private Vector2D clapping_scaling = new Vector2D(1516.03f, 885.34f);
        static private Vector2D clapping_vel = new Vector2D(100f, 100f);
        static private Vector2D left_hand_pos = new Vector2D(0f, 0f);
        static private Vector2D right_hand_pos = new Vector2D(0f, 0f);
        static private float two_hand_speed = 25f;
        static private bool is_two_hand_clapping = false;

        static private bool is_one_hand_waiting = false;
        static private uint random_hand_stay = 0;
        static private bool is_one_hand_ready = false;
        static private float one_hand_speed = 35f;

        static public bool is_attacking = false;
        static public bool is_destoryed = false;
        static private bool start_slap = false;

        static private float resting_timer_set = 0.5f;
        static private float resting_timer = 0.5f;

        private enum ClapAttackTypes : int
        {
            TwoHand = 0,
            OneHandWaiting = 1
        }

        static private ClapAttackTypes attack_type = 0;

        static public void Init()
        {
            LEFT_HAND_ID = InternalCalls.GetCurrentEntityID();
            RIGHT_HAND_ID = InternalCalls.CreateEntityPrefab("BossRightHand");

            // init scale and pos
            InternalCalls.TransformSetPositionEntity(-999999f, -999999f, LEFT_HAND_ID);
            InternalCalls.TransformSetPositionEntity(999999f, 999999f, RIGHT_HAND_ID);

            InternalCalls.TransformSetScaleEntity(-clapping_scaling.x, clapping_scaling.y, LEFT_HAND_ID);
            InternalCalls.TransformSetScaleEntity(clapping_scaling.x, clapping_scaling.y, RIGHT_HAND_ID);

            // reset
            is_two_hand_clapping = false;
            is_one_hand_waiting = false;
            is_attacking = false;
            is_destoryed = false;
            start_slap = false;
            clapping_pos = new Vector2D(99999f, 99999f);

            InternalCalls.ResetSpriteAnimationFrameEntity(LEFT_HAND_ID);
            InternalCalls.ResetSpriteAnimationFrameEntity(RIGHT_HAND_ID);

            attack_type = ClapAttackTypes.TwoHand;
        }

        static public void Update()
        {
            RandomAttackType();
            HandClapFSM();
        }

        static public void CleanUp()
        {

        }

        static private void HandClapFSM()
        {
            //attack_type = ClapAttackTypes.OneHandWaiting;
            switch (attack_type)
            {
                case ClapAttackTypes.TwoHand:
                    TwoHandClap();
                    TwoHandSlapPauseAnimation();
                    break;
                case ClapAttackTypes.OneHandWaiting:
                    OneHandWaitingForClapping();
                    OneHandSlapPauseAnimation();
                    break;
            }
        }

        static private void RandomAttackType() 
        {
            MyRandom rnd = new MyRandom((uint)(129243 * InternalCalls.GetRandomFloat()));
            uint random = rnd.Next(0, 2); // random from 0 to 1
            //random = 0;
            if (!is_attacking) {
                if (random == 0)
                {
                    attack_type = ClapAttackTypes.TwoHand;
                    //return;
                }
                if (random == 1)
                {
                    attack_type = ClapAttackTypes.OneHandWaiting;
                }
                is_attacking = true;
            }
        }

        static private void TwoHandClap()
        {

            if (!is_two_hand_clapping) // init clapping position, left and right hand spawn position
            {
                clapping_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(PlayerScript.PLAYER_ID));

                Vector2D offset = new Vector2D(2500f, 500f);

                left_hand_pos = new Vector2D(clapping_pos.x - offset.x, clapping_pos.y + offset.y);
                right_hand_pos = new Vector2D(clapping_pos.x + offset.x, clapping_pos.y + offset.y);
                is_two_hand_clapping = true;
            }

            Vector2D left_hand_direction = new Vector2D(clapping_pos.x - left_hand_pos.x, clapping_pos.y - left_hand_pos.y);
            left_hand_direction = left_hand_direction.Normalize();

            Vector2D right_hand_direction = new Vector2D(clapping_pos.x - right_hand_pos.x, clapping_pos.y - right_hand_pos.y);
            right_hand_direction = right_hand_direction.Normalize();
            
            left_hand_pos.x += left_hand_direction.x * two_hand_speed;
            left_hand_pos.y += left_hand_direction.y * two_hand_speed;
            right_hand_pos.x += right_hand_direction.x * two_hand_speed;
            right_hand_pos.y += right_hand_direction.y * two_hand_speed;

            // render hands flying particles
            LeftHandFlyingParticles(left_hand_direction);
            RightHandFlyingParticles(right_hand_direction);

            InternalCalls.TransformSetPositionEntity(left_hand_pos.x, left_hand_pos.y, LEFT_HAND_ID);
            InternalCalls.TransformSetPositionEntity(right_hand_pos.x, right_hand_pos.y, RIGHT_HAND_ID);

            // render clapping position particles
            RenderClappingPosParticles();

            CheckCollide();
        }

        static private void OneHandWaitingForClapping()
        {
            if (!is_one_hand_waiting)
            {
                MyRandom rnd = new MyRandom((uint)(129243 * InternalCalls.GetRandomFloat()));
                random_hand_stay = rnd.Next(0, 2); // random from 0 to 1
            }
            if (random_hand_stay == 0)
            {
                LeftHandStays();
            }
            if (random_hand_stay == 1)
            {
                RightHandStays();
            }
        }

        static private void LeftHandStays()
        {
            if (!is_one_hand_waiting)
            {
                clapping_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(PlayerScript.PLAYER_ID));
                clapping_pos.x -= 500f;
                clapping_pos.y -= 100f;

                Vector2D left_offset = new Vector2D(0f, 2500f);
                Vector2D right_offset = new Vector2D(4000f, 0f);

                left_hand_pos = new Vector2D(clapping_pos.x + left_offset.x, clapping_pos.y + left_offset.y);
                right_hand_pos = new Vector2D(clapping_pos.x + right_offset.x, clapping_pos.y + right_offset.y);
                is_one_hand_waiting = true;
                is_one_hand_ready = false;
            }

            // left hand get ready first
            if (!is_one_hand_ready)
            {
                Vector2D left_hand_direction = new Vector2D(clapping_pos.x - left_hand_pos.x, clapping_pos.y - left_hand_pos.y);
                if (MathF.Abs(left_hand_direction.x) < 50f && MathF.Abs(left_hand_direction.y) < 50f)
                {
                    is_one_hand_ready = true;
                }
                left_hand_direction = left_hand_direction.Normalize();

                left_hand_pos.x += left_hand_direction.x * one_hand_speed;
                left_hand_pos.y += left_hand_direction.y * one_hand_speed;
                
                // render hands flying particles
                LeftHandFlyingParticles(left_hand_direction);
                
            }
            
            // right hand coming over
            if (is_one_hand_ready)
            {
                Vector2D right_hand_direction = new Vector2D(clapping_pos.x - right_hand_pos.x, clapping_pos.y - right_hand_pos.y);
                right_hand_direction = right_hand_direction.Normalize();

                right_hand_pos.x += right_hand_direction.x * one_hand_speed * 1.5f;
                right_hand_pos.y += right_hand_direction.y * one_hand_speed * 1.5f;
                left_hand_pos.x += 0.5f;
                // render hands flying particles
                RightHandFlyingParticles(right_hand_direction);
            }

            InternalCalls.TransformSetPositionEntity(left_hand_pos.x, left_hand_pos.y, LEFT_HAND_ID);
            InternalCalls.TransformSetPositionEntity(right_hand_pos.x, right_hand_pos.y, RIGHT_HAND_ID);

            CheckCollide();

        }

        static private void RightHandStays()
        {
            if (!is_one_hand_waiting)
            {
                clapping_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(PlayerScript.PLAYER_ID));
                clapping_pos.x += 500f;
                clapping_pos.y -= 100f;

                Vector2D right_offset = new Vector2D(0f, 2500f);
                Vector2D left_offset = new Vector2D(-4000f, 0f);

                left_hand_pos = new Vector2D(clapping_pos.x + left_offset.x, clapping_pos.y + left_offset.y);
                right_hand_pos = new Vector2D(clapping_pos.x + right_offset.x, clapping_pos.y + right_offset.y);
                is_one_hand_waiting = true;
                is_one_hand_ready = false;
            }

            // right hand get ready first
            if (!is_one_hand_ready)
            {
                Vector2D right_hand_direction = new Vector2D(clapping_pos.x - right_hand_pos.x, clapping_pos.y - right_hand_pos.y);
                if (MathF.Abs(right_hand_direction.x) < 50f && MathF.Abs(right_hand_direction.y) < 50f)
                {
                    is_one_hand_ready = true;
                }
                right_hand_direction = right_hand_direction.Normalize();

                right_hand_pos.x += right_hand_direction.x * one_hand_speed;
                right_hand_pos.y += right_hand_direction.y * one_hand_speed;

                // render hands flying particles
                RightHandFlyingParticles(right_hand_direction);

            }
            //Console.WriteLine(is_one_hand_ready);
            if (is_one_hand_ready)
            {
                Vector2D left_hand_direction = new Vector2D(clapping_pos.x - left_hand_pos.x, clapping_pos.y - left_hand_pos.y);
                left_hand_direction = left_hand_direction.Normalize();

                left_hand_pos.x += left_hand_direction.x * one_hand_speed * 1.5f;
                left_hand_pos.y += left_hand_direction.y * one_hand_speed * 1.5f;
                right_hand_pos.x -= 0.5f;
                // render hands flying particles
                LeftHandFlyingParticles(left_hand_direction);
            }

            InternalCalls.TransformSetPositionEntity(left_hand_pos.x, left_hand_pos.y, LEFT_HAND_ID);
            InternalCalls.TransformSetPositionEntity(right_hand_pos.x, right_hand_pos.y, RIGHT_HAND_ID);

            CheckCollide();
        }

        static private void CheckCollide()
        {
            if (PlayerScript.invulnerable) // player is dashing
            {
                return;
            }

            // if left hand and right hand collide each other, destory them
            if (InternalCalls.CompareEntityCategory(LEFT_HAND_ID, "RightHandBoss"))
            {
                DestoryHands();
                return;
            }

            // if two hand collide with player, damage player, render particles and disappear
            if (InternalCalls.CompareEntityCategory(LEFT_HAND_ID, "Player") &&
                InternalCalls.CompareEntityCategory(RIGHT_HAND_ID, "Player"))
            {
                DestoryHands();

                // damage player
                PlayerScript.is_colliding_enemy = true;
                PlayerScript.colliding_enemy_id = LEFT_HAND_ID;
                PlayerScript.colliding_enemy_type = (int)EnemyType.BossHand;
                return;
            }
            

            // if one of the hands collides with player, it will push the player to the center
            if (InternalCalls.CompareEntityCategory(LEFT_HAND_ID, "Player"))
            {
                PlayerScript.isGrounded = false;
                PlayerScript.isFirstGrounded = false;
                if (PlayerScript.player_vel.x < 0f)
                {
                    InternalCalls.RigidBodySetVelocityEntity(0f, 0f, PlayerScript.PLAYER_ID);
                }
                
               // PlayerScript.AddForcesToPlayer(2000f, 5f, 0.25f);
                return;
            }
            else if (InternalCalls.CompareEntityCategory(RIGHT_HAND_ID, "Player"))
            {
                PlayerScript.isGrounded = false;
                PlayerScript.isFirstGrounded = false;
                if (PlayerScript.player_vel.x > 0f)
                {
                    InternalCalls.RigidBodySetVelocityEntity(0f, 0f, PlayerScript.PLAYER_ID);
                }
                //PlayerScript.AddForcesToPlayer(-1000f, 5f, 0.25f);
                return;
            }

        }

        static private void DestoryHands()
        {
            BossBattle.StateChanger();
            InternalCalls.DestroyEntity(RIGHT_HAND_ID);
            InternalCalls.DestroyEntity(LEFT_HAND_ID);

            // render particles
            HandClapParticles();

            // reset attack
            is_attacking = false;

            is_destoryed = true;
        }

        static private void HandClapParticles()
        {
            // load destory particles
            MyRandom my_rand = new MyRandom((uint)(129248189 * InternalCalls.GetRandomFloat()));
            int particle_count = (int)(my_rand.Next(50, 71)); // random from 50 to 70 particles
            for (int i = 0; i < particle_count; i++)
            {
                float rand = my_rand.NextFloat();
                float dir = 360 * rand;

                rand = my_rand.NextFloat();
                float size = 20f + 20f * rand; // initial size

                rand = my_rand.NextFloat();
                float size_scale = -30 * rand; // pariticles going smaller

                rand = my_rand.NextFloat();
                float alpha = 0.5f + 0.3f * rand;

                rand = my_rand.NextFloat();
                float lifetime = 0.5f + 0.5f * rand;

                rand = my_rand.NextFloat();
                float speed = 500f + 300f * rand;

                rand = my_rand.NextFloat();
                float x = (left_hand_pos.x + right_hand_pos.x) / 2f;
                //float x = enemy_pos.x;

                rand = my_rand.NextFloat();
                float y = (left_hand_pos.y + right_hand_pos.y) / 2f;

            InternalCalls.GameSpawnParticleExtra(
                    x, y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Boss Hand.txt"
                 );
            }
        }

        static private void RenderClappingPosParticles()
        {
            MyRandom my_rand = new MyRandom((uint)(129248189 * InternalCalls.GetRandomFloat()));
            int particle_count = (int)(my_rand.Next(2, 5)); // random from 2 to 4 particles
            for (int i = 0; i < particle_count; i++)
            {
                float rand = my_rand.NextFloat();
                float dir = 80 + 20 * rand;

                rand = my_rand.NextFloat();
                float size = 10f + 5f * rand; // initial size

                rand = my_rand.NextFloat();
                float size_scale = -20 * rand; // pariticles going smaller

                rand = my_rand.NextFloat();
                float alpha = 0.5f + 0.3f * rand;

                rand = my_rand.NextFloat();
                float lifetime = 0.5f + 0.5f * rand;

                rand = my_rand.NextFloat();
                float speed = 200f + 200f * rand;

                Vector2D scaling = PlayerScript.trans_scaling;
                rand = my_rand.NextFloat();
                float x = clapping_pos.x + scaling.x / 2f * (rand - 0.5f);
                //float x = enemy_pos.x;

                rand = my_rand.NextFloat();
                float y = (clapping_pos.y - scaling.y / 4f) + scaling.y / 4f * (rand);

                InternalCalls.GameSpawnParticleExtra(
                        x, y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Boss Hand.txt"
                     );
            }
        }

        

        static private void LeftHandFlyingParticles(Vector2D direction)
        {
            MyRandom my_rand = new MyRandom((uint)(129248189 * InternalCalls.GetRandomFloat()));
            int particle_count = (int)(my_rand.Next(5, 11)); // random from 5 to 10 particles
            for (int i = 0; i < particle_count; i++)
            {
                float rand = my_rand.NextFloat();
                float dir = MathF.Atan2(direction.y, direction.x) * (180 / MathF.PI) + (160 + 40 * rand);

                rand = my_rand.NextFloat();
                float size = 20f + 20f * rand; // initial size

                rand = my_rand.NextFloat();
                float size_scale = -20 * rand; // pariticles going smaller

                rand = my_rand.NextFloat();
                float alpha = 0.5f + 0.2f * rand;

                rand = my_rand.NextFloat();
                float lifetime = 0.5f + 0.5f * rand;

                rand = my_rand.NextFloat();
                float speed = 300f + 200f * rand;

                SimpleVector2D scaling = InternalCalls.GetTransformScalingEntity(LEFT_HAND_ID);
                rand = my_rand.NextFloat();
                float x = left_hand_pos.x + scaling.x / 2f * (rand - 0.5f);
                //float x = enemy_pos.x;

                rand = my_rand.NextFloat();
                //float y = (left_hand_pos.y - scaling.y / 4f) + scaling.y / 4f * (rand);
                float y = left_hand_pos.y + scaling.y / 2f * (rand - 0.5f); ;

                InternalCalls.GameSpawnParticleExtra(
                        x, y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Boss Hand.txt"
                     );
            }
        }

        static private void RightHandFlyingParticles(Vector2D direction)
        {
            MyRandom my_rand = new MyRandom((uint)(129248189 * InternalCalls.GetRandomFloat()));
            int particle_count = (int)(my_rand.Next(5, 11)); // random from 5 to 10 particles
            for (int i = 0; i < particle_count; i++)
            {
                float rand = my_rand.NextFloat();
                float dir = MathF.Atan2(direction.y, direction.x) * (180 / MathF.PI) + (160 + 40 * rand);

                rand = my_rand.NextFloat();
                float size = 20f + 20f * rand; // initial size

                rand = my_rand.NextFloat();
                float size_scale = -20 * rand; // pariticles going smaller

                rand = my_rand.NextFloat();
                float alpha = 0.5f + 0.2f * rand;

                rand = my_rand.NextFloat();
                float lifetime = 0.5f + 0.5f * rand;

                rand = my_rand.NextFloat();
                float speed = 300f + 200f * rand;

                SimpleVector2D scaling = InternalCalls.GetTransformScalingEntity(RIGHT_HAND_ID);
                rand = my_rand.NextFloat();
                float x = right_hand_pos.x + scaling.x / 2f * (rand - 0.5f);
                //float x = enemy_pos.x;

                rand = my_rand.NextFloat();
                //float y = (left_hand_pos.y - scaling.y / 4f) + scaling.y / 4f * (rand);
                float y = right_hand_pos.y + scaling.y / 2f * (rand - 0.5f); ;

                InternalCalls.GameSpawnParticleExtra(
                        x, y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Boss Hand.txt"
                     );
            }
        }

        static private void TwoHandSlapPauseAnimation()
        {
            if (is_destoryed)
            {
                return;
            }

            // limit left hand animation
            if (InternalCalls.GetCurrentAnimationEntity(LEFT_HAND_ID) >= 13)
            {
                InternalCalls.SetAnimationEntityPlaying(LEFT_HAND_ID, false);
            }

            // limit right hand animation
            if (InternalCalls.GetCurrentAnimationEntity(RIGHT_HAND_ID) >= 13)
            {
                InternalCalls.SetAnimationEntityPlaying(RIGHT_HAND_ID, false);
                return; // return check
            }

            // control animation player for left hand (right hand same indx)
            if (InternalCalls.GetCurrentAnimationEntity(LEFT_HAND_ID) == 3 && !start_slap)
            {
                start_slap = true;
            }

            if (start_slap)
            {
                float left_distance = clapping_pos.x - InternalCalls.GetTransformPositionEntity(LEFT_HAND_ID).x;
                float right_distance = clapping_pos.x - InternalCalls.GetTransformPositionEntity(RIGHT_HAND_ID).x;

                if (MathF.Abs(left_distance) < 900f || MathF.Abs(right_distance) < 900f)
                {
                    InternalCalls.SetAnimationEntityPlaying(LEFT_HAND_ID, true);
                    InternalCalls.SetAnimationEntityPlaying(RIGHT_HAND_ID, true);
                }
                else
                {
                    InternalCalls.SetAnimationEntityPlaying(LEFT_HAND_ID, false);
                    InternalCalls.SetAnimationEntityPlaying(RIGHT_HAND_ID, false);
                }
            }
        }

        static private void OneHandSlapPauseAnimation()
        {
            if (is_destoryed)
            {
                return;
            }

            // limit left hand animation
            if (InternalCalls.GetCurrentAnimationEntity(LEFT_HAND_ID) >= 13)
            {
                InternalCalls.SetAnimationEntityPlaying(LEFT_HAND_ID, false);
            }

            // limit right hand animation
            if (InternalCalls.GetCurrentAnimationEntity(RIGHT_HAND_ID) >= 13)
            {
                InternalCalls.SetAnimationEntityPlaying(RIGHT_HAND_ID, false);
                return; // return check
            }

            // control animation player for left hand (right hand same indx)
            if (InternalCalls.GetCurrentAnimationEntity(LEFT_HAND_ID) == 3 && !start_slap)
            {
                start_slap = true;
            }

            if (random_hand_stay == 0)
            {
                if (start_slap)
                {
                    //float left_distance = PlayerScript.player_pos.x - InternalCalls.GetTransformPositionEntity(LEFT_HAND_ID).x;
                    float right_distance = clapping_pos.x - InternalCalls.GetTransformPositionEntity(RIGHT_HAND_ID).x;

                    if (MathF.Abs(right_distance) < 1800f)
                    {
                        InternalCalls.SetAnimationEntityPlaying(LEFT_HAND_ID, true);
                        InternalCalls.SetAnimationEntityPlaying(RIGHT_HAND_ID, true);
                    }
                    else
                    {
                        InternalCalls.SetAnimationEntityPlaying(LEFT_HAND_ID, false);
                        InternalCalls.SetAnimationEntityPlaying(RIGHT_HAND_ID, false);
                    }
                }
            }
            else if (random_hand_stay == 1)
            {
                if (start_slap)
                {
                    float left_distance = clapping_pos.x - InternalCalls.GetTransformPositionEntity(LEFT_HAND_ID).x;
                    //float right_distance = PlayerScript.player_pos.x - InternalCalls.GetTransformPositionEntity(RIGHT_HAND_ID).x;

                    if (MathF.Abs(left_distance) < 1800f)
                    {
                        InternalCalls.SetAnimationEntityPlaying(LEFT_HAND_ID, true);
                        InternalCalls.SetAnimationEntityPlaying(RIGHT_HAND_ID, true);
                    }
                    else
                    {
                        InternalCalls.SetAnimationEntityPlaying(LEFT_HAND_ID, false);
                        InternalCalls.SetAnimationEntityPlaying(RIGHT_HAND_ID, false);
                    }
                }
            }
            
        }

    }
}
