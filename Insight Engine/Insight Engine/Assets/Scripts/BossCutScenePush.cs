using System.Numerics;
using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
namespace IS
{
    class BossCutScenePush
    {
        static float chase_speed = 14f;
        static Vector2D current_pos = new Vector2D(0,0);
        static private float alpha_value;
        static private float size;
        static public bool triggered;
        static public bool can_move;
        static public bool initial_hit = false;

        static public void Init()
        {
            alpha_value = 0; size = 0;triggered = false; can_move = false; initial_hit = false;
        }

        static public void Update()
        {
            if(triggered)
            {
                int entity_boss = InternalCalls.GetCurrentEntityID();
                if (alpha_value < 1) { alpha_value += InternalCalls.GetDeltaTime(); }
                if (size < 500) { size += InternalCalls.GetDeltaTime() * 500; }
                InternalCalls.SetSpriteAlphaEntity(alpha_value, entity_boss);
                InternalCalls.TransformSetScaleEntity(-size*512/299, size, entity_boss);
            }

            if(!can_move) { return; }

            current_pos = new Vector2D(InternalCalls.GetTransformPosition().x, InternalCalls.GetTransformPosition().y);

            Vector2D direction = new Vector2D(InternalCalls.GetTransformPositionEntity(PlayerScript.PLAYER_ID).x-current_pos.x,
                InternalCalls.GetTransformPositionEntity(PlayerScript.PLAYER_ID).y - current_pos.y);
            direction= direction.Normalize();
            InternalCalls.TransformSetPosition(current_pos.x+direction.x * chase_speed,current_pos.y+ direction.y * chase_speed);

            SimpleArray array = InternalCalls.GetCollidingEntityArray(InternalCalls.GetCurrentEntityID());

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1)
            {
                if(!initial_hit) {
                    PlayerScript.isGrounded = false;
                    PlayerScript.player_pos.x = 6760;
                    PlayerScript.player_pos.y = 2800;
                    PlayerScript.prev_x = 6760;
                    PlayerScript.player_ground_pos.x = 6760;
                    InternalCalls.TransformSetPositionEntity(6760, 2800, PlayerScript.PLAYER_ID);
                    initial_hit = true;

                }
                InternalCalls.ChangeLightType(2);// spazzed out
                CameraScript.camera_shake_intensity = 5f;
                CameraScript.camera_shake_duration = 0.2f;
                CameraScript.CameraShake(0.3f);
                PlayerScript.PLAYER_LIGHT = 2;
                InternalCalls.RigidBodySetVelocityEntity(direction.x * 2000, direction.y * 2000, PlayerScript.PLAYER_ID);

                //InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());
            }



            }

        static public void CleanUp()
        {
        }

    }
}