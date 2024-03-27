
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
        static private Vector2D clapping_scaling = new Vector2D(500f, 500f);
        static private Vector2D clapping_vel = new Vector2D(100f, 100f);
        static private Vector2D left_hand_pos = new Vector2D(0f, 0f);
        static private Vector2D right_hand_pos = new Vector2D(0f, 0f);
        static private float two_hand_speed = 25f;
        static private bool is_two_hand_clapping = false;

        static private bool is_one_hand_waiting = false;
        static private uint random_hand_stay = 0;
        static private bool is_one_hand_ready = false;
        static private float one_hand_speed = 50f;

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

            InternalCalls.TransformSetScaleEntity(clapping_scaling.x, clapping_scaling.y, LEFT_HAND_ID);
            InternalCalls.TransformSetScaleEntity(-clapping_scaling.x, clapping_scaling.y, RIGHT_HAND_ID);

            // reset
            is_two_hand_clapping = false;
            is_one_hand_waiting = false;
            clapping_pos = new Vector2D(99999f, 99999f);
        }

        static public void Update()
        {
            //RandomAttackType();
            HandClapFSM();
        }

        static public void CleanUp()
        {

        }

        static private void HandClapFSM()
        {
            attack_type = ClapAttackTypes.OneHandWaiting;
            switch (attack_type)
            {
                case ClapAttackTypes.TwoHand:
                    TwoHandClap();
                    break;
                case ClapAttackTypes.OneHandWaiting:
                    OneHandWaitingForClapping();
                    break;
            }
        }

        static private void RandomAttackType() 
        { 
            MyRandom rnd = new MyRandom((uint)(129243 * InternalCalls.GetRandomFloat()));
            uint random = rnd.Next(0,5); // random from 0 to 4
            //random = 2;
            if (random == 0)
            {
                attack_type = ClapAttackTypes.TwoHand;
                return;
            }
            if (random == 1)
            {
                attack_type = ClapAttackTypes.OneHandWaiting;
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

            InternalCalls.TransformSetPositionEntity(left_hand_pos.x, left_hand_pos.y, LEFT_HAND_ID);
            InternalCalls.TransformSetPositionEntity(right_hand_pos.x, right_hand_pos.y, RIGHT_HAND_ID);

            // render clapping position particles
            RenderClappingPosParticles();

            // if two hand collide with player, damage player, render particles and disappear
            if (InternalCalls.CompareEntityCategory(LEFT_HAND_ID, "Player") &&
                InternalCalls.CompareEntityCategory(RIGHT_HAND_ID, "Player"))
            {
                DestoryHands();

                // damage player
                return;
            }

            // if one of the hands collides with player, it will push the player to the center
            if (InternalCalls.CompareEntityCategory(LEFT_HAND_ID, "Player"))
            {
                PlayerScript.isGrounded = false;
                PlayerScript.isFirstGrounded = false;
                PlayerScript.AddForcesToPlayer(1000f, 5f, 0.2f);
            }
            else if (InternalCalls.CompareEntityCategory(RIGHT_HAND_ID, "Player"))
            {
                PlayerScript.isGrounded = false;
                PlayerScript.isFirstGrounded = false;
                PlayerScript.AddForcesToPlayer(-1000f, 5f, 0.2f);
            }

            // if left hand and right hand collide each other, destory them
            if (InternalCalls.CompareEntityCategory(LEFT_HAND_ID, "RightHandBoss"))
            {
                DestoryHands();
            }
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

                Vector2D left_offset = new Vector2D(0f, 2500f);
                Vector2D right_offset = new Vector2D(4500f, 0f);

                left_hand_pos = new Vector2D(clapping_pos.x + left_offset.x, clapping_pos.y + left_offset.y);
                right_hand_pos = new Vector2D(clapping_pos.x + right_offset.x, clapping_pos.y + right_offset.y);
                is_one_hand_waiting = true;
                is_one_hand_ready = false;
            }

            if (!is_one_hand_ready)
            {
                Vector2D left_hand_direction = new Vector2D(clapping_pos.x - left_hand_pos.x, clapping_pos.y - left_hand_pos.y);
                if (MathF.Abs(left_hand_direction.x) < 100f && MathF.Abs(left_hand_direction.y) < 100f)
                {
                    is_one_hand_ready = true;
                }
                left_hand_direction = left_hand_direction.Normalize();

                left_hand_pos.x += left_hand_direction.x * one_hand_speed;
                left_hand_pos.y += left_hand_direction.y * one_hand_speed;
            }
            //Console.WriteLine(is_one_hand_ready);
            if (is_one_hand_ready)
            {
                Vector2D right_hand_direction = new Vector2D(clapping_pos.x - right_hand_pos.x, clapping_pos.y - right_hand_pos.y);
                right_hand_direction = right_hand_direction.Normalize();

                right_hand_pos.x += right_hand_direction.x * one_hand_speed * 2f;
                right_hand_pos.y += right_hand_direction.y * one_hand_speed * 2f;
            }

            InternalCalls.TransformSetPositionEntity(left_hand_pos.x, left_hand_pos.y, LEFT_HAND_ID);
            InternalCalls.TransformSetPositionEntity(right_hand_pos.x, right_hand_pos.y, RIGHT_HAND_ID);

            // if two hand collide with player, damage player, render particles and disappear
            if (InternalCalls.CompareEntityCategory(LEFT_HAND_ID, "Player") &&
                InternalCalls.CompareEntityCategory(RIGHT_HAND_ID, "Player"))
            {
                DestoryHands();

                // damage player
                return;
            }

            // if one of the hands collides with player, it will push the player to the center
            if (InternalCalls.CompareEntityCategory(LEFT_HAND_ID, "Player"))
            {
                PlayerScript.isGrounded = false;
                PlayerScript.isFirstGrounded = false;
                PlayerScript.AddForcesToPlayer(1000f, 5f, 0.2f);
            }
            else if (InternalCalls.CompareEntityCategory(RIGHT_HAND_ID, "Player"))
            {
                PlayerScript.isGrounded = false;
                PlayerScript.isFirstGrounded = false;
                PlayerScript.AddForcesToPlayer(-1000f, 5f, 0.2f);
            }

            // if left hand and right hand collide each other, destory them
            if (InternalCalls.CompareEntityCategory(LEFT_HAND_ID, "RightHandBoss"))
            {
                DestoryHands();
            }

        }

        static private void RightHandStays()
        {

        }

        static private void DestoryHands()
        {
            BossBattle.StateChanger();
            InternalCalls.DestroyEntity(RIGHT_HAND_ID);
            InternalCalls.DestroyEntity(LEFT_HAND_ID);

            // render particles
            HandClapParticles();
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
                float size = 50f + 50f * rand; // initial size

                rand = my_rand.NextFloat();
                float size_scale = -30 * rand; // pariticles going smaller

                rand = my_rand.NextFloat();
                float alpha = 0.7f + 0.3f * rand;

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
                    x, y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Enemy Bleeding.txt"
                 );
            }
        }

        static private void RenderClappingPosParticles()
        {
            MyRandom my_rand = new MyRandom((uint)(129248189 * InternalCalls.GetRandomFloat()));
            int particle_count = (int)(my_rand.Next(2, 6)); // random from 2 to 5 particles
            for (int i = 0; i < particle_count; i++)
            {
                float rand = my_rand.NextFloat();
                float dir = 80 + 20 * rand;

                rand = my_rand.NextFloat();
                float size = 10f + 10f * rand; // initial size

                rand = my_rand.NextFloat();
                float size_scale = -20 * rand; // pariticles going smaller

                rand = my_rand.NextFloat();
                float alpha = 0.7f + 0.3f * rand;

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
                        x, y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Enemy Bleeding.txt"
                     );
            }
        }

    }
}
